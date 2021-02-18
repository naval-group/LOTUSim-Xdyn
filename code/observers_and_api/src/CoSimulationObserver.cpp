#include "CoSimulationObserver.hpp"

std::vector<std::string> add_CS_data_to_optional_data(const std::vector<std::string>& optional_data, const std::string& body_name);
std::vector<std::string> add_CS_data_to_optional_data(const std::vector<std::string>& optional_data, const std::string& body_name)
{
    std::vector<std::string> base({
        "t",
        "x("+body_name+")",
        "y("+body_name+")",
        "z("+body_name+")",
        "u("+body_name+")",
        "v("+body_name+")",
        "w("+body_name+")",
        "p("+body_name+")",
        "q("+body_name+")",
        "r("+body_name+")",
        "qr("+body_name+")",
        "qi("+body_name+")",
        "qj("+body_name+")",
        "qk("+body_name+")",
        "phi("+body_name+")",
        "theta("+body_name+")",
        "psi("+body_name+")"});
    base.insert(base.end(), optional_data.begin(), optional_data.end());
    return base;
}

CoSimulationObserver::CoSimulationObserver(const std::vector<std::string>& extra_fields_to_serialize, const std::string& body_name_):
        Observer(add_CS_data_to_optional_data(extra_fields_to_serialize, body_name_)),
        current_state(),
        states(),
        body_name(body_name_)
{
}

CoSimulationObserver::~CoSimulationObserver()
{
}

std::vector<YamlState> CoSimulationObserver::get() const
{
    return states;
}

std::function<void()> CoSimulationObserver::get_serializer(const double val, const DataAddressing& address)
{
    return [this, val, address]()
    {
        if(address.name=="t")
        {
            current_state.t = val;
        }
        else if(address.address.at(0)=="states" && address.address.at(1)==body_name)
        {
            if(address.address.at(2)=="X")
            {
                current_state.x = val;
            }
            else if(address.address.at(2)=="Y")
            {
                current_state.y = val;
            }
            else if(address.address.at(2)=="Z")
            {
                current_state.z = val;
            }
            else if(address.address.at(2)=="U")
            {
                current_state.u = val;
            }
            else if(address.address.at(2)=="V")
            {
                current_state.v = val;
            }
            else if(address.address.at(2)=="W")
            {
                current_state.w = val;
            }
            else if(address.address.at(2)=="P")
            {
                current_state.p = val;
            }
            else if(address.address.at(2)=="Q")
            {
                current_state.q = val;
            }
            else if(address.address.at(2)=="R")
            {
                current_state.r = val;
            }
            else if(address.address.at(2)=="PHI")
            {
                current_state.phi = val;
            }
            else if(address.address.at(2)=="THETA")
            {
                current_state.theta = val;
            }
            else if(address.address.at(2)=="PSI")
            {
                current_state.psi = val;
            }
            else if(address.address.at(2)=="Quat")
            {
                if(address.address.at(3)=="Qr")
                {
                    current_state.qr = val;
                }
                else if(address.address.at(3)=="Qi")
                {
                    current_state.qi = val;
                }
                else if(address.address.at(3)=="Qj")
                {
                    current_state.qj = val;
                }
                else if(address.address.at(3)=="Qk")
                {
                    current_state.qk = val;
                }
            }
        }
        else
        {
            current_state.extra_observations[address.name] = val;
        }
    };
}

std::function<void()> CoSimulationObserver::get_initializer(const double, const DataAddressing&)
{
    return [](){};
}

void CoSimulationObserver::flush_after_initialization()
{
}

void CoSimulationObserver::flush_after_write()
{
    states.push_back(current_state);
    current_state = YamlState();
}

void CoSimulationObserver::flush_value_during_write()
{
}
