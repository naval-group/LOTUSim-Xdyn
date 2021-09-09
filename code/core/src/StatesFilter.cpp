#include "yaml.h"
#include "BodyStates.hpp"
#include "History.hpp"
#include "InvalidInputException.hpp"
#include "InternalErrorException.hpp"
#include "StatesFilter.hpp"
#include "YamlBody.hpp"

std::shared_ptr<StateFilter> StateFilter::build(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    std::string type_of_filter;
    node["type of filter"] >> type_of_filter;
    if (type_of_filter != "moving average")
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unknown filter: known state filters are: 'moving average'.");
    }
    double duration_in_seconds = 0;
    node["duration in seconds"] >> duration_in_seconds;
    return std::shared_ptr<StateFilter>(new StateFilter(duration_in_seconds));
}

StateFilter::StateFilter(const double duration_in_seconds_) : duration_in_seconds(duration_in_seconds_) {}

double StateFilter::get_value(const History& h) const
{
    return h.average(duration_in_seconds);
}

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

double StatesFilter::get_filtered_x(const AbstractStates<History>& history) const
{
    return x->get_value(history.x);
}

double StatesFilter::get_filtered_y(const AbstractStates<History>& history) const
{
    return y->get_value(history.y);
}

double StatesFilter::get_filtered_z(const AbstractStates<History>& history) const
{
    return z->get_value(history.z);
}

double StatesFilter::get_filtered_u(const AbstractStates<History>& history) const
{
    return u->get_value(history.u);
}
double StatesFilter::get_filtered_v(const AbstractStates<History>& history) const
{
    return v->get_value(history.v);
}

double StatesFilter::get_filtered_w(const AbstractStates<History>& history) const
{
    return w->get_value(history.w);
}

double StatesFilter::get_filtered_p(const AbstractStates<History>& history) const
{
    return p->get_value(history.p);
}

double StatesFilter::get_filtered_q(const AbstractStates<History>& history) const
{
    return q->get_value(history.q);
}

double StatesFilter::get_filtered_r(const AbstractStates<History>& history) const
{
    return r->get_value(history.r);
}

ssc::kinematics::EulerAngles get_filtered_euler_angle(const std::shared_ptr<StateFilter>& filter, const AbstractStates<History>& history, const YamlRotation& rotations);
ssc::kinematics::EulerAngles get_filtered_euler_angle(const std::shared_ptr<StateFilter>& filter, const AbstractStates<History>& history, const YamlRotation& rotations)
{
    const double qr = filter->get_value(history.qr);
    const double qi = filter->get_value(history.qi);
    const double qj = filter->get_value(history.qj);
    const double qk = filter->get_value(history.qk);
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