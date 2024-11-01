/*
 * Body.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "EnvironmentAndFrames.hpp"
#include "Observer.hpp"
#include "xdyn/exceptions/NumericalErrorException.hpp"

Body::Body(const size_t i, const BlockedDOF& blocked_states_, const YamlFilteredStates& filtered_states)
    : states(filtered_states)
    , idx(i)
    , blocked_states(blocked_states_)
    , states_filter(filtered_states)
{
}

Body::Body(const BodyStates& s, const size_t i, const BlockedDOF& blocked_states_, const YamlFilteredStates& filtered_states)
    : states(s)
    , idx(i)
    , blocked_states(blocked_states_)
    , states_filter(filtered_states)
{
}

Body::Body(const size_t i, const BlockedDOF& blocked_states_, const StatesFilter& states_filter_)
    : states(states_filter_)
    , idx(i)
    , blocked_states(blocked_states_)
    , states_filter(states_filter_)
{}

Body::Body(const BodyStates& states_, const size_t i, const BlockedDOF& blocked_states_, const StatesFilter& states_filter_)
    : states(states_)
    , idx(i)
    , blocked_states(blocked_states_)
    , states_filter(states_filter_)
{}


Body::~Body()
{
}

BodyStates Body::get_states() const
{
    return states;
}

#define SQUARE(x) ((x)*(x))

ssc::kinematics::Point Body::get_origin(const StateType& x) const
{
    return ssc::kinematics::Point("NED", *_X(x,idx),
                                         *_Y(x,idx),
                                         *_Z(x,idx));
}

ssc::kinematics::Point Body::get_position_of_body_relative_to_mesh() const
{
    return ssc::kinematics::Point(std::string("mesh(")+states.name+")",
                                  states.x_relative_to_mesh,
                                  states.y_relative_to_mesh,
                                  states.z_relative_to_mesh);
}

ssc::kinematics::Transform Body::get_transform_from_mesh_to_body() const
{
    return ssc::kinematics::Transform(get_position_of_body_relative_to_mesh(), states.mesh_to_body, states.name);
}

ssc::kinematics::Transform Body::get_transform_from_ned_to_body(const StateType& x) const
{
    return ssc::kinematics::Transform(get_origin(x), states.get_rot_from_ned_to(x, idx), states.name);
}

ssc::kinematics::Transform Body::get_transform_from_ned_to_local_ned(const StateType& x) const
{
    return ssc::kinematics::Transform(get_origin(x), std::string("NED(") + states.name + ")");
}

void Body::update_kinematics(const StateType& x, const ssc::kinematics::KinematicsPtr& k) const
{
    k->add(get_transform_from_ned_to_body(x));
    k->add(get_transform_from_ned_to_local_ned(x));
}

StateType Body::block_states_if_necessary(StateType x, const double t) const
{
    blocked_states.force_states(x,t);
    return x;
}

void Body::update_body_states(StateType x, const double t)
{
    blocked_states.force_states(x,t);
    states.x.record(t, *_X(x,idx));
    states.y.record(t, *_Y(x,idx));
    states.z.record(t, *_Z(x,idx));
    states.u.record(t, *_U(x,idx));
    states.v.record(t, *_V(x,idx));
    states.w.record(t, *_W(x,idx));
    states.p.record(t, *_P(x,idx));
    states.q.record(t, *_Q(x,idx));
    states.r.record(t, *_R(x,idx));
    states.qr.record(t, *_QR(x,idx));
    states.qi.record(t, *_QI(x,idx));
    states.qj.record(t, *_QJ(x,idx));
    states.qk.record(t, *_QK(x,idx));
}

void Body::force_states(StateType& x, const double t) const
{
    blocked_states.force_states(x,t);
}

void Body::update_projection_of_z_in_mesh_frame(const double g,
                                                const ssc::kinematics::KinematicsPtr& k)
{
    const ssc::kinematics::Point g_in_NED("NED", 0, 0, g);
    const ssc::kinematics::RotationMatrix ned2mesh = k->get("NED", std::string("mesh(") + states.name + ")").get_rot();
    states.g_in_mesh_frame = ned2mesh*g_in_NED.v;
}

#define CHECK(x,y,t) if (std::isnan(x)) {THROW(__PRETTY_FUNCTION__,NumericalErrorException,"NaN detected in state " << y << ", at t = " << t);}
void Body::update(const EnvironmentAndFrames& env, const StateType& x, const double t)
{
    CHECK(*_X(x,idx),"X",t);
    CHECK(*_Y(x,idx),"Y",t);
    CHECK(*_Z(x,idx),"Z",t);
    CHECK(*_U(x,idx),"U",t);
    CHECK(*_V(x,idx),"V",t);
    CHECK(*_W(x,idx),"W",t);
    CHECK(*_P(x,idx),"P",t);
    CHECK(*_Q(x,idx),"Q",t);
    CHECK(*_R(x,idx),"R",t);
    CHECK(*_QR(x,idx),"QR",t);
    CHECK(*_QI(x,idx),"QI",t);
    CHECK(*_QJ(x,idx),"QJ",t);
    CHECK(*_QK(x,idx),"QK",t);
    update_kinematics(x,env.k);
    update_body_states(x, t);
    update_intersection_with_free_surface(env, t);
    update_projection_of_z_in_mesh_frame(env.g, env.k);
}

void Body::set_history(const EnvironmentAndFrames& env, const State& states)
{
    set_states_history(states);
    if (not(states.x.is_empty()))
    {
        update_kinematics(states.get_StateType(states.x.size()-1), env.k);
        update_intersection_with_free_surface(env, states.x.get_current_time());
    }
    update_projection_of_z_in_mesh_frame(env.g, env.k);
}

void Body::calculate_state_derivatives(const ssc::kinematics::Wrench& sum_of_forces,
                                         const StateType& x,
                                         StateType& dx_dt,
                                         const double t,
                                         const EnvironmentAndFrames& env) const
{
    // du/dt, dv/dt, dw/dt, dp/dt, dq/dt, dr/dt
    Eigen::Map<Eigen::Matrix<double,6,1> > dXdt(_U(dx_dt,idx));

    dXdt = states.inverse_of_the_total_inertia * (sum_of_forces.to_vector());

    // dx/dt, dy/dt, dz/dt
    const ssc::kinematics::RotationMatrix& R = env.k->get("NED", states.name).get_rot();
    const Eigen::Map<const Eigen::Vector3d> uvw(_U(x,idx));
    const Eigen::Vector3d XpYpZp(R*uvw);
    
    std::vector<double> xx {*_X(x,idx)};
    std::vector<double> yy {*_Y(x,idx)};

    Eigen::Vector3d WCurrent;
    if (env.UWCurrent != nullptr)
    {
        if (env.w != nullptr)
        {
            std::vector<double> w_height = env.w->get_and_check_wave_height(xx,yy,t);
            WCurrent = env.UWCurrent->get_UWCurrent((get_origin(x)).v, t, w_height[0]);
        }
        else
        {
            WCurrent = Eigen::Vector3d::Zero();
        }
    }
    else
    {
        WCurrent = Eigen::Vector3d::Zero();
    }

    *_X(dx_dt,idx) = XpYpZp(0) + WCurrent(0);
    *_Y(dx_dt,idx) = XpYpZp(1) + WCurrent(1);
    *_Z(dx_dt,idx) = XpYpZp(2) + WCurrent(2);

    // dqr/dt, dqi/dt, dqj/dt, dqk/dt
    const Eigen::Quaternion<double> q1(*_QR(x,idx),
                                       *_QI(x,idx),
                                       *_QJ(x,idx),
                                       *_QK(x,idx));
    const Eigen::Quaternion<double> q2(0,*_P(x,idx),*_Q(x,idx),*_R(x,idx));
    const Eigen::Quaternion<double>& dq_dt = q1*q2;
    *_QR(dx_dt,idx) = 0.5 * dq_dt.w();
    *_QI(dx_dt,idx) = 0.5 * dq_dt.x();
    *_QJ(dx_dt,idx) = 0.5 * dq_dt.y();
    *_QK(dx_dt,idx) = 0.5 * dq_dt.z();

    blocked_states.force_state_derivatives(dx_dt, t);
}

Eigen::Vector3d Body::get_uvw(const StateType& x) const
{
    return Eigen::Vector3d::Map(_U(x,idx));
}

Eigen::Vector3d Body::get_pqr(const StateType& x) const
{
    return Eigen::Vector3d::Map(_P(x,idx));
}

BlockedDOF::Vector Body::get_delta_F(const StateType& dx_dt, const ssc::kinematics::Wrench& sum_of_other_forces) const
{
    return blocked_states.get_delta_F(dx_dt,states.total_inertia,sum_of_other_forces);
}

void Body::feed(const StateType& x, Observer& observer, const YamlRotation& c) const
{
    observer.write_after_solver_step(*_X(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"X"},std::string("x(")+states.name+")"));
    observer.write_after_solver_step(*_Y(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"Y"},std::string("y(")+states.name+")"));
    observer.write_after_solver_step(*_Z(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"Z"},std::string("z(")+states.name+")"));
    observer.write_after_solver_step(*_U(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"U"},std::string("u(")+states.name+")"));
    observer.write_after_solver_step(*_V(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"V"},std::string("v(")+states.name+")"));
    observer.write_after_solver_step(*_W(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"W"},std::string("w(")+states.name+")"));
    observer.write_after_solver_step(*_P(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"P"},std::string("p(")+states.name+")"));
    observer.write_after_solver_step(*_Q(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"Q"},std::string("q(")+states.name+")"));
    observer.write_after_solver_step(*_R(x,idx), DataAddressing(std::vector<std::string>{"states",states.name,"R"},std::string("r(")+states.name+")"));
    observer.write_after_solver_step(*_QR(x,idx),DataAddressing(std::vector<std::string>{"states",states.name,"Quat","Qr"},std::string("qr(")+states.name+")"));
    observer.write_after_solver_step(*_QI(x,idx),DataAddressing(std::vector<std::string>{"states",states.name,"Quat","Qi"},std::string("qi(")+states.name+")"));
    observer.write_after_solver_step(*_QJ(x,idx),DataAddressing(std::vector<std::string>{"states",states.name,"Quat","Qj"},std::string("qj(")+states.name+")"));
    observer.write_after_solver_step(*_QK(x,idx),DataAddressing(std::vector<std::string>{"states",states.name,"Quat","Qk"},std::string("qk(")+states.name+")"));
    const auto angles = get_angles(x, c);
    observer.write_after_solver_step(angles.phi, DataAddressing(std::vector<std::string>{"states",states.name,"PHI"},std::string("phi(")+states.name+")"));
    observer.write_after_solver_step(angles.theta, DataAddressing(std::vector<std::string>{"states",states.name,"THETA"},std::string("theta(")+states.name+")"));
    observer.write_after_solver_step(angles.psi, DataAddressing(std::vector<std::string>{"states",states.name,"PSI"},std::string("psi(")+states.name+")"));

    const auto filtered_states = get_filtered_states(states, x);
    observer.write_after_solver_step(filtered_states.x, DataAddressing(std::vector<std::string>{"filtered_states",states.name,"X"},std::string("x_filtered(")+states.name+")"));
    observer.write_after_solver_step(filtered_states.y, DataAddressing(std::vector<std::string>{"filtered_states",states.name,"Y"},std::string("y_filtered(")+states.name+")"));
    observer.write_after_solver_step(filtered_states.z, DataAddressing(std::vector<std::string>{"filtered_states",states.name,"Z"},std::string("z_filtered(")+states.name+")"));
    observer.write_after_solver_step(filtered_states.u, DataAddressing(std::vector<std::string>{"filtered_states",states.name,"U"},std::string("u_filtered(")+states.name+")"));
    observer.write_after_solver_step(filtered_states.v, DataAddressing(std::vector<std::string>{"filtered_states",states.name,"V"},std::string("v_filtered(")+states.name+")"));
    observer.write_after_solver_step(filtered_states.w, DataAddressing(std::vector<std::string>{"filtered_states",states.name,"W"},std::string("w_filtered(")+states.name+")"));
    observer.write_after_solver_step(filtered_states.p, DataAddressing(std::vector<std::string>{"filtered_states",states.name,"P"},std::string("p_filtered(")+states.name+")"));
    observer.write_after_solver_step(filtered_states.q, DataAddressing(std::vector<std::string>{"filtered_states",states.name,"Q"},std::string("q_filtered(")+states.name+")"));
    observer.write_after_solver_step(filtered_states.r, DataAddressing(std::vector<std::string>{"filtered_states",states.name,"R"},std::string("r_filtered(")+states.name+")"));
    observer.write_after_solver_step(filtered_states.phi, DataAddressing(std::vector<std::string>{"filtered_states",states.name,"PHI"},std::string("phi_filtered(")+states.name+")"));
    observer.write_after_solver_step(filtered_states.theta, DataAddressing(std::vector<std::string>{"filtered_states",states.name,"THETA"},std::string("theta_filtered(")+states.name+")"));
    observer.write_after_solver_step(filtered_states.psi, DataAddressing(std::vector<std::string>{"filtered_states",states.name,"PSI"},std::string("psi_filtered(")+states.name+")"));
}

std::string Body::get_name() const
{
    return states.name;
}

ssc::kinematics::RotationMatrix Body::get_rot_from_ned_to(const StateType& x) const
{
    return states.get_rot_from_ned_to(x,idx);
}

ssc::kinematics::EulerAngles Body::get_angles(const StateType& all_states, const YamlRotation& c) const
{
    return states.get_angles(all_states, idx, c);
}

std::tuple<double,double,double,double> Body::get_quaternions(const ssc::kinematics::EulerAngles& angle, const YamlRotation& c) const
{
    return states.convert(angle,c);
}

void Body::set_states_history(const AbstractStates<History>& s)
{
    states = s;
}

void Body::reset_history()
{
    states.x.reset();
    states.y.reset();
    states.z.reset();
    states.u.reset();
    states.v.reset();
    states.w.reset();
    states.p.reset();
    states.q.reset();
    states.r.reset();
    states.qr.reset();
    states.qi.reset();
    states.qj.reset();
    states.qk.reset();
}

FilteredStates Body::get_filtered_states() const
{
    return FilteredStates(states_filter, states, states.convention);
}

FilteredStates Body::get_filtered_states(AbstractStates<History> state_history, const StateType& x) const
{
    const double t = state_history.x.get_current_time();
    state_history.x.record(t, *_X(x,idx));
    state_history.y.record(t, *_Y(x,idx));
    state_history.z.record(t, *_Z(x,idx));
    state_history.u.record(t, *_U(x,idx));
    state_history.v.record(t, *_V(x,idx));
    state_history.w.record(t, *_W(x,idx));
    state_history.p.record(t, *_P(x,idx));
    state_history.q.record(t, *_Q(x,idx));
    state_history.r.record(t, *_R(x,idx));
    state_history.qr.record(t, *_QR(x,idx));
    state_history.qi.record(t, *_QI(x,idx));
    state_history.qj.record(t, *_QJ(x,idx));
    state_history.qk.record(t, *_QK(x,idx));
    return FilteredStates(states_filter, state_history, states.convention);
}