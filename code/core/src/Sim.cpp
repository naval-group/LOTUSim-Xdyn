/*
 * Sim.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */


#include "Observer.hpp"
#include "Sim.hpp"
#include "update_kinematics.hpp"
#include "SurfaceElevationInterface.hpp"
#include "YamlWaveModelInput.hpp"
#include "InternalErrorException.hpp"

#include <ssc/kinematics.hpp>
#include <ssc/numeric.hpp>

#define SQUARE(x) ((x)*(x))

ssc::kinematics::UnsafeWrench transport_to_origin_of_body_frame(const ssc::kinematics::Wrench& F, ssc::kinematics::KinematicsPtr& k);
ssc::kinematics::UnsafeWrench transport_to_origin_of_body_frame(const ssc::kinematics::Wrench& F, ssc::kinematics::KinematicsPtr& k)
{
    const Wrench F_in_body_frame_at_G(F);
    const Wrench F_in_body_frame_at_origin = F_in_body_frame_at_G.transport_to(ssc::kinematics::Point(F.get_frame()), k);
    return ssc::kinematics::UnsafeWrench(F_in_body_frame_at_origin.get_point(), F_in_body_frame_at_origin.get_force(), F_in_body_frame_at_origin.get_torque());
}

class Sim::Impl
{
    public:
        Impl(const std::vector<BodyPtr>& bodies_,
             const std::vector<ListOfForces>& forces_,
             const EnvironmentAndFrames& env_,
             const StateType& x,
             const ssc::data_source::DataSource& command_listener_) :
                 bodies(bodies_), name2bodyptr(), forces(), env(env_),
                 _dx_dt(StateType(x.size(),0)), command_listener(command_listener_), sum_of_forces_in_body_frame(),
                 sum_of_forces_in_NED_frame(), fictitious_forces_in_body_frame(), fictitious_forces_in_NED_frame()
        {
            size_t i = 0;
            for (auto body:bodies)
            {
                forces[body->get_name()] = forces_.at(i++);
                name2bodyptr[body->get_name()] = body;
            }
        }

        void feed_sum_of_forces(Observer& observer, const std::string& body_name)
        {
            auto sum_forces_body = transport_to_origin_of_body_frame(sum_of_forces_in_body_frame[body_name], env.k);
            feed_force(observer, sum_forces_body, "sum of forces", body_name, body_name);
            feed_force(observer, sum_of_forces_in_NED_frame[body_name], "sum of forces", body_name, "NED");
        }
        void feed_fictitious_forces(Observer& observer, const std::string& body_name)
        {
            feed_force(observer, fictitious_forces_in_body_frame[body_name], "fictitious forces", body_name, body_name);
            feed_force(observer, fictitious_forces_in_NED_frame[body_name], "fictitious forces", body_name, "NED");
        }

        void feed_force(Observer& observer, ssc::kinematics::UnsafeWrench& W, const std::string& force_name, const std::string& body_name, const std::string& frame)
        {
            observer.write(W.X(),DataAddressing({"efforts",body_name,force_name,frame,"Fx"}, "Fx("+force_name+","+body_name+","+frame+")"));
            observer.write(W.Y(),DataAddressing({"efforts",body_name,force_name,frame,"Fy"}, "Fy("+force_name+","+body_name+","+frame+")"));
            observer.write(W.Z(),DataAddressing({"efforts",body_name,force_name,frame,"Fz"}, "Fz("+force_name+","+body_name+","+frame+")"));
            observer.write(W.K(),DataAddressing({"efforts",body_name,force_name,frame,"Mx"}, "Mx("+force_name+","+body_name+","+frame+")"));
            observer.write(W.M(),DataAddressing({"efforts",body_name,force_name,frame,"My"}, "My("+force_name+","+body_name+","+frame+")"));
            observer.write(W.N(),DataAddressing({"efforts",body_name,force_name,frame,"Mz"}, "Mz("+force_name+","+body_name+","+frame+")"));
        }

        ssc::data_source::DataSource& get_command_listener()
        {
            return command_listener;
        }

        std::vector<BodyPtr> bodies;
        std::map<std::string,BodyPtr> name2bodyptr;
        std::map<std::string,std::vector<ForcePtr> > forces;
        EnvironmentAndFrames env;
        StateType _dx_dt;
        ssc::data_source::DataSource command_listener;
        std::map<std::string,ssc::kinematics::UnsafeWrench> sum_of_forces_in_body_frame;
        std::map<std::string,ssc::kinematics::UnsafeWrench> sum_of_forces_in_NED_frame;
        std::map<std::string,ssc::kinematics::UnsafeWrench> fictitious_forces_in_body_frame;
        std::map<std::string,ssc::kinematics::UnsafeWrench> fictitious_forces_in_NED_frame;
};

ssc::data_source::DataSource& Sim::get_command_listener() const
{
    return pimpl->get_command_listener();
}

std::map<std::string,std::vector<ForcePtr> > Sim::get_forces() const
{
    return pimpl->forces;
}

std::vector<BodyPtr> Sim::get_bodies() const
{
    return pimpl->bodies;
}

EnvironmentAndFrames Sim::get_env() const
{
    return pimpl->env;
}

std::vector<std::string> Sim::get_command_names() const
{
    std::vector<std::string> ret;
    for (auto forces:pimpl->forces)
    {
        for (auto force:forces.second)
        {
            const auto command_names = force->get_command_names();
            const auto force_name = force->get_name();
            for (auto command_name:command_names)
            {
                if (not(command_name.empty()))
                {
                    ret.push_back(force_name + "(" + command_name + ")");
                }
            }
        }
    }
    return ret;
}

Sim::Sim(const std::vector<BodyPtr>& bodies,
         const std::vector<ListOfForces>& forces,
         const EnvironmentAndFrames& env,
         const StateType& x,
         const ssc::data_source::DataSource& command_listener) : pimpl(new Impl(bodies, forces, env, x, command_listener))
{
    state = x;
}

StateType Sim::normalize_quaternions(const StateType& all_states
                                    ) const
{
    StateType normalized = all_states;
    for (size_t i = 0 ; i < pimpl->bodies.size() ; ++i)
    {
        const auto norm = std::hypot(std::hypot(std::hypot(*_QR(normalized,i),*_QI(normalized,i)),*_QJ(normalized,i)),*_QK(normalized,i));
        if (not almost_equal(norm,1.0))
        {
            *_QR(normalized,i) /= norm;
            *_QI(normalized,i) /= norm;
            *_QJ(normalized,i) /= norm;
            *_QK(normalized,i) /= norm;
        }
    }
    return normalized;
}

void Sim::force_states(StateType& x, const double t) const
{
    for (auto body: pimpl->bodies)
    {
        body->force_states(x, t);
    }
}

void Sim::dx_dt(const StateType& x, StateType& dxdt, const double t)
{
    for (auto body: pimpl->bodies)
    {
        body->update(pimpl->env,x,t);
        const auto Fext = sum_of_forces(x, body, t);
        body->calculate_state_derivatives(Fext, x, dxdt, t, pimpl->env);
    }
    state = normalize_quaternions(x);
    pimpl->_dx_dt = dxdt;
}

ssc::kinematics::Wrench project_into_NED_frame(const ssc::kinematics::Wrench& F, const ssc::kinematics::RotationMatrix& R);
ssc::kinematics::Wrench project_into_NED_frame(const ssc::kinematics::Wrench& F, const ssc::kinematics::RotationMatrix& R)
{
    return ssc::kinematics::Wrench(ssc::kinematics::Point("NED"),R*F.force,R*F.torque);
}

ssc::kinematics::UnsafeWrench Sim::sum_of_forces(const StateType& x, const BodyPtr& body, const double t)
{
    const Eigen::Vector3d uvw = body->get_uvw(x);
    const Eigen::Vector3d pqr = body->get_pqr(x);
    const auto states = body->get_states();
    pimpl->fictitious_forces_in_body_frame[body->get_name()] = ssc::kinematics::UnsafeWrench(coriolis_and_centripetal(states.G,states.solid_body_inertia.get(),uvw, pqr));
    pimpl->sum_of_forces_in_body_frame[body->get_name()] = pimpl->fictitious_forces_in_body_frame[body->get_name()];
    const auto forces = pimpl->forces[body->get_name()];
    for (auto force:forces)
    {
        const ssc::kinematics::Wrench tau = force->operator()(states, t, pimpl->env, pimpl->command_listener);
        pimpl->sum_of_forces_in_body_frame[body->get_name()] += tau;
    }
    pimpl->sum_of_forces_in_NED_frame[body->get_name()] = project_into_NED_frame(pimpl->sum_of_forces_in_body_frame[body->get_name()],states.get_rot_from_ned_to_body());
    pimpl->fictitious_forces_in_NED_frame[body->get_name()] = project_into_NED_frame(pimpl->fictitious_forces_in_body_frame[body->get_name()],states.get_rot_from_ned_to_body());
    return pimpl->sum_of_forces_in_body_frame[body->get_name()];
}

void Sim::initialize_system_outputs_before_first_observation()
{
    StateType dxdt(13);
    double t = 0;
    if (not(pimpl->bodies.empty()))
    {
        t = pimpl->bodies.at(0)->get_states().u.get_current_time();
    }
    dx_dt(state, dxdt, t);
}

ssc::kinematics::PointMatrix Sim::get_waves(const double t//!< Current instant
                                                  ) const
{
    try
    {
        if (pimpl->env.w.get())
        {
            for (size_t i = 0 ; i < pimpl->bodies.size() ; ++i)
            {
                pimpl->bodies[i]->update_kinematics(state,pimpl->env.k);
            }
            return pimpl->env.w->get_waves_on_mesh(pimpl->env.k, t);
        }
    }
    catch (const ssc::kinematics::KinematicsException& e)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calculating waves on mesh: the output reference frame does not exist (" << e.get_message() << ")");
    }
    return ssc::kinematics::PointMatrix("NED",0);
}

void Sim::output(const StateType& x, Observer& obs, const double t, const std::vector<std::shared_ptr<ssc::solver::DiscreteSystem> >& discrete_systems) const
{
    StateType x_with_forced_states;
    for (auto body: pimpl->bodies)
    {
        x_with_forced_states = body->block_states_if_necessary(x,t);
    }
    const auto normalized_x = normalize_quaternions(x_with_forced_states);
    for (auto forces:pimpl->forces)
    {
        for (auto force:forces.second)
        {
            const auto body_name = forces.first;
            const auto body = pimpl->name2bodyptr[body_name];
            const auto G = body->get_origin(x);
            force->feed(obs,pimpl->env.k, pimpl->command_listener, t);
        }
    }
    for (auto body:pimpl->bodies)
    {
        body->feed(normalized_x, obs, pimpl->env.rot);
        auto dF = body->get_delta_F(pimpl->_dx_dt,pimpl->sum_of_forces_in_body_frame[body->get_name()]);
        obs.write((double)dF(0),DataAddressing(std::vector<std::string>{"efforts",body->get_name(),"blocked states",body->get_name(),"Fx"},std::string("Fx(blocked states,")+body->get_name()+","+body->get_name()+")"));
        obs.write((double)dF(1),DataAddressing(std::vector<std::string>{"efforts",body->get_name(),"blocked states",body->get_name(),"Fy"},std::string("Fy(blocked states,")+body->get_name()+","+body->get_name()+")"));
        obs.write((double)dF(2),DataAddressing(std::vector<std::string>{"efforts",body->get_name(),"blocked states",body->get_name(),"Fz"},std::string("Fz(blocked states,")+body->get_name()+","+body->get_name()+")"));
        obs.write((double)dF(3),DataAddressing(std::vector<std::string>{"efforts",body->get_name(),"blocked states",body->get_name(),"Mx"},std::string("Mx(blocked states,")+body->get_name()+","+body->get_name()+")"));
        obs.write((double)dF(4),DataAddressing(std::vector<std::string>{"efforts",body->get_name(),"blocked states",body->get_name(),"My"},std::string("My(blocked states,")+body->get_name()+","+body->get_name()+")"));
        obs.write((double)dF(5),DataAddressing(std::vector<std::string>{"efforts",body->get_name(),"blocked states",body->get_name(),"Mz"},std::string("Mz(blocked states,")+body->get_name()+","+body->get_name()+")"));
    }
    pimpl->env.feed(obs, t, pimpl->bodies, normalized_x);
    for (auto body:pimpl->bodies)
    {
        pimpl->feed_sum_of_forces(obs, body->get_name());
        pimpl->feed_fictitious_forces(obs, body->get_name());
    }
    for (const auto discrete_system : discrete_systems)
    {
        const auto name = discrete_system->get_name();
        for (const auto output : discrete_system->get_outputs())
        {
            const double value = pimpl->command_listener.get<double>(output);
            obs.write(value, DataAddressing(std::vector<std::string>{"controllers",name}, output));
        }
    }
}

void Sim::set_bodystates(const State& states_history)
{
    pimpl->bodies.at(0)->set_history(pimpl->env, states_history);
    if (not(states_history.x.is_empty()))
    {
        state = states_history.get_StateType(states_history.x.size()-1);
    }
}

void Sim::set_command_listener(const std::map<std::string, double>& new_commands)
{
    for(const auto c : new_commands)
    {
        pimpl->command_listener.set(c.first, c.second);
    }
}

void Sim::set_discrete_state(const std::string &state_name, const double value)
{
    pimpl->command_listener.set(state_name, value);
}

double Sim::get_input_value(const std::string &name) const
{
    return pimpl->command_listener.get<double>(name);
}

double Sim::get_state_value(const std::string &name) const
{
    if (name == "x") {
        return pimpl->bodies.front()->get_states().x();
    } else if (name == "y") {
        return pimpl->bodies.front()->get_states().y();
    } else if (name == "z") {
        return pimpl->bodies.front()->get_states().z();
    } else if (name == "u") {
        return pimpl->bodies.front()->get_states().u();
    } else if (name == "v") {
        return pimpl->bodies.front()->get_states().v();
    } else if (name == "w") {
        return pimpl->bodies.front()->get_states().w();
    } else if (name == "p") {
        return pimpl->bodies.front()->get_states().p();
    } else if (name == "q") {
        return pimpl->bodies.front()->get_states().q();
    } else if (name == "r") {
        return pimpl->bodies.front()->get_states().r();
    } else if (name == "qr") {
        return pimpl->bodies.front()->get_states().qr();
    } else if (name == "qi") {
        return pimpl->bodies.front()->get_states().qi();
    } else if (name == "qj") {
        return pimpl->bodies.front()->get_states().qj();
    } else if (name == "qk") {
        return pimpl->bodies.front()->get_states().qk();
    } else if (name == "phi") {
        return pimpl->bodies.front()->get_states().get_angles().phi;
    } else if (name == "theta") {
        return pimpl->bodies.front()->get_states().get_angles().theta;
    } else if (name == "psi") {
        return pimpl->bodies.front()->get_states().get_angles().psi;
    } else {
        THROW(__PRETTY_FUNCTION__, InternalErrorException,
              "Something is wrong with a 'states' name: '" << name << "' is not a valid state. Examples of valid states: 'x', 'u', 'qr', 'psi', etc.."
              );
    }
}

void Sim::reset_history()
{
    for (auto body:pimpl->bodies)
    {
        body->reset_history();
    }
}
