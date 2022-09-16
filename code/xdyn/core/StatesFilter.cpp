#include "yaml.h"
#include "xdyn/core/BodyStates.hpp"
#include "xdyn/hdb_interpolators/History.hpp"
#include "xdyn/exceptions/InvalidInputException.hpp"
#include "xdyn/exceptions/InternalErrorException.hpp"
#include "xdyn/core/StatesFilter.hpp"
#include "xdyn/external_data_structures/YamlBody.hpp"

FilteredStates::FilteredStates(const StatesFilter& filters, const AbstractStates<History>& states, const YamlRotation& rot)
    : AbstractStates<double>()
    , phi(filters.get_filtered_phi(states, rot))
    , theta(filters.get_filtered_theta(states, rot))
    , psi(filters.get_filtered_psi(states, rot))
    , rotation_convention(rot)
{
    x = filters.get_filtered_x(states);
    y = filters.get_filtered_y(states);
    z = filters.get_filtered_z(states);
    u = filters.get_filtered_u(states);
    v = filters.get_filtered_v(states);
    w = filters.get_filtered_w(states);
    p = filters.get_filtered_p(states);
    q = filters.get_filtered_q(states);
    r = filters.get_filtered_r(states);
    qr = filters.get_filtered_qr(states);
    qi = filters.get_filtered_qi(states);
    qj = filters.get_filtered_qj(states);
    qk = filters.get_filtered_qk(states);
}

class MovingAverage : public StateFilter
{
    public:
        MovingAverage(const double duration_in_seconds_) : duration_in_seconds(duration_in_seconds_)
        {}

        double filter(const History& h) const
        {
            return h.average(duration_in_seconds);
        }

        double get_Tmax() const
        {
            return duration_in_seconds;
        }

    private:
        const double duration_in_seconds;
};

std::shared_ptr<StateFilter> StateFilter::build(const std::string& yaml)
{
    if (yaml.empty())
    {
        return std::shared_ptr<StateFilter>(new MovingAverage(0));
    }
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    std::string type_of_filter;
    node["type of filter"] >> type_of_filter;

    if (type_of_filter == "moving average")
    {
        double duration_in_seconds = 0;
        node["duration in seconds"] >> duration_in_seconds;
        return std::shared_ptr<StateFilter>(new MovingAverage(duration_in_seconds));
    }
    THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unknown filter '" << type_of_filter << "': known state filters are: 'moving average'.");
    return std::shared_ptr<StateFilter>(new MovingAverage(0));
}

StateFilter::StateFilter() {}

StateFilter::~StateFilter() {}


StatesFilter::StatesFilter(const YamlFilteredStates& input)
    : x(StateFilter::build(input.x))
    , y(StateFilter::build(input.y))
    , z(StateFilter::build(input.z))
    , u(StateFilter::build(input.u))
    , v(StateFilter::build(input.v))
    , w(StateFilter::build(input.w))
    , p(StateFilter::build(input.p))
    , q(StateFilter::build(input.q))
    , r(StateFilter::build(input.r))
    , phi(StateFilter::build(input.phi))
    , theta(StateFilter::build(input.theta))
    , psi(StateFilter::build(input.psi))
{}

double StatesFilter::get_Tmax() const
{
    double Tmax = x->get_Tmax();
    Tmax = std::max(Tmax, y->get_Tmax());
    Tmax = std::max(Tmax, z->get_Tmax());
    Tmax = std::max(Tmax, u->get_Tmax());
    Tmax = std::max(Tmax, v->get_Tmax());
    Tmax = std::max(Tmax, w->get_Tmax());
    Tmax = std::max(Tmax, p->get_Tmax());
    Tmax = std::max(Tmax, q->get_Tmax());
    Tmax = std::max(Tmax, r->get_Tmax());
    Tmax = std::max(Tmax, phi->get_Tmax());
    Tmax = std::max(Tmax, theta->get_Tmax());
    return std::max(Tmax, psi->get_Tmax());
}

double StatesFilter::get_filtered_x(const AbstractStates<History>& history) const
{
    return x->filter(history.x);
}

double StatesFilter::get_filtered_y(const AbstractStates<History>& history) const
{
    return y->filter(history.y);
}

double StatesFilter::get_filtered_z(const AbstractStates<History>& history) const
{
    return z->filter(history.z);
}

double StatesFilter::get_filtered_u(const AbstractStates<History>& history) const
{
    return u->filter(history.u);
}
double StatesFilter::get_filtered_v(const AbstractStates<History>& history) const
{
    return v->filter(history.v);
}

double StatesFilter::get_filtered_w(const AbstractStates<History>& history) const
{
    return w->filter(history.w);
}

double StatesFilter::get_filtered_p(const AbstractStates<History>& history) const
{
    return p->filter(history.p);
}

double StatesFilter::get_filtered_q(const AbstractStates<History>& history) const
{
    return q->filter(history.q);
}

double StatesFilter::get_filtered_r(const AbstractStates<History>& history) const
{
    return r->filter(history.r);
}


double StatesFilter::get_filtered_qr(const AbstractStates<History>& history) const
{
    return history.qr();
}

double StatesFilter::get_filtered_qi(const AbstractStates<History>& history) const
{
    return history.qi();
}

double StatesFilter::get_filtered_qj(const AbstractStates<History>& history) const
{
    return history.qj();
}

double StatesFilter::get_filtered_qk(const AbstractStates<History>& history) const
{
    return history.qk();
}

ssc::kinematics::EulerAngles get_filtered_euler_angle(const std::shared_ptr<StateFilter>& filter, const AbstractStates<History>& history, const YamlRotation& rotations);
ssc::kinematics::EulerAngles get_filtered_euler_angle(const std::shared_ptr<StateFilter>& filter, const AbstractStates<History>& history, const YamlRotation& rotations)
{
    const double qr = filter->filter(history.qr);
    const double qi = filter->filter(history.qi);
    const double qj = filter->filter(history.qj);
    const double qk = filter->filter(history.qk);
    const ssc::kinematics::RotationMatrix R = Eigen::Quaternion<double>(qr, qi, qj, qk).matrix();
    return  BodyStates::convert(R, rotations);
}

double StatesFilter::get_filtered_phi(const AbstractStates<History>& history, const YamlRotation& rotations) const
{
    return get_filtered_euler_angle(phi, history, rotations).phi;
}

double StatesFilter::get_filtered_theta(const AbstractStates<History>& history, const YamlRotation& rotations) const
{
    return get_filtered_euler_angle(theta, history, rotations).theta;
}

double StatesFilter::get_filtered_psi(const AbstractStates<History>& history, const YamlRotation& rotations) const
{
    return get_filtered_euler_angle(psi, history, rotations).psi;
}