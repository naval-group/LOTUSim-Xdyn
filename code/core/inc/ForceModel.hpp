/*
 * ForceModel.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef FORCEMODEL_HPP_
#define FORCEMODEL_HPP_

#include <map>

#include <boost/optional/optional.hpp>

#include <ssc/kinematics.hpp>

#include "yaml-cpp/exceptions.h"
#include "InvalidInputException.hpp"
#include "YamlBody.hpp"
#include "Wrench.hpp"

#include "EnvironmentAndFrames.hpp"
#include "YamlPosition.hpp"

namespace ssc { namespace data_source { class DataSource;}}
struct BodyStates;
struct YamlRotation;

class ForceModel;
typedef TR1(shared_ptr)<ForceModel> ForcePtr;
typedef std::vector<ForcePtr> ListOfForces;
typedef std::function<boost::optional<ForcePtr>(const YamlModel&, const std::string&, const EnvironmentAndFrames&)> ForceParser;

class Observer;

// SFINAE test for 'parse' method
template<typename T>
struct HasParse
{
    typedef char yes[1];
    typedef char no [2];
    template<typename U> static yes &check(decltype(&U::parse));
    template<typename U> static no &check(...);
    static const bool value = sizeof(check<T>(0)) == sizeof(yes);
};

/** \brief These force models read commands from a DataSource.
 *  \details Provides facilities to the derived classes to retrieve the commands
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 *  \section ex1 Example
 *  \snippet model_wrappers/unit_tests/src/ForceModelTest.cpp ForceModelTest example
 *  \section ex2 Expected output
 *  \snippet model_wrappers/unit_tests/src/ForceModelTest.cpp ForceModelTest expected output
 */
class ForceModel
{
    public:
        ForceModel(const std::string& name, const std::vector<std::string>& commands, const YamlPosition& internal_frame, const std::string& body_name_, const EnvironmentAndFrames& env);
        ForceModel(const std::string& name, const std::vector<std::string>& commands, const std::string& body_name_, const EnvironmentAndFrames& env);
        virtual ~ForceModel() = default;
        ssc::kinematics::Wrench operator()(const BodyStates& states, const double t, const EnvironmentAndFrames& env, ssc::data_source::DataSource& command_listener);
        ssc::kinematics::Wrench operator()(const BodyStates& states, const double t, const EnvironmentAndFrames& env);
        virtual Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const = 0;
        std::string get_name() const;
        virtual double get_Tmax() const; // Can be overloaded if model needs access to History (not a problem, just has to say how much history to keep)
        std::string get_body_name() const;
        virtual bool is_a_surface_force_model() const;
        std::vector<std::string> get_command_names() const;

        template<typename ForceType>
        static typename boost::enable_if<HasParse<ForceType>, ForceParser>::type build_parser()
        {
            auto parser = [](
                const YamlModel& yaml,
                const std::string& body_name,
                const EnvironmentAndFrames& env) -> boost::optional<ForcePtr>
                {
                    boost::optional<ForcePtr> ret;
                    if (yaml.model == ForceType::model_name())
                    {
                        std::string context = "Invalid input data for model '" + ForceType::model_name() + "'.";
                        try
                        {
                            ret.reset(ForcePtr(new ForceType(ForceType::parse(yaml.yaml), body_name, env)));
                        }
                        catch (const InvalidInputException& exception)
                        {
                            THROW(__PRETTY_FUNCTION__, InvalidInputException, context << std::endl << exception.get_message() << std::endl << "Model containing error is defined line "
                                    << yaml.index_of_first_line_in_global_yaml << " of the YAML file." << std::endl);
                        }
                        catch (const YAML::Exception& exception)
                        {
                            const size_t line_number = yaml.index_of_first_line_in_global_yaml;
                            THROW(__PRETTY_FUNCTION__, InvalidInputException, context << std::endl << "Model containing error is defined line "
                                    << line_number << " of the YAML file." << std::endl << "The error was: " << exception.msg);
                        }
                    }
                    return ret;
                    };
            return parser;
        }

        template <typename ForceType>
        static typename boost::disable_if<HasParse<ForceType>, ForceParser>::type build_parser()
        {
            auto parser = [](const YamlModel& yaml, const std::string& body, const EnvironmentAndFrames& env) -> boost::optional<ForcePtr>
            {
                boost::optional<ForcePtr> ret;
                if (yaml.model == ForceType::model_name())
                {
                    ret.reset(ForcePtr(new ForceType(body, env)));
                }
                return ret;
            };
            return parser;
        }

        void feed(Observer& observer, ssc::kinematics::KinematicsPtr& k, ssc::data_source::DataSource& command_listener, const double t) const;
        virtual void extra_observations(Observer& observer) const;
    protected:
        std::vector<std::string> commands;
        std::string name;
        std::string body_name;

    private:
        ForceModel(); // Deactivated
        double get_command(const std::string& command_name, ssc::data_source::DataSource& command_listener, const double t) const;
        std::map<std::string,double> get_commands(ssc::data_source::DataSource& command_listener, const double t) const;
        void can_find_internal_frame(const ssc::kinematics::KinematicsPtr& k) const;
        bool is_cached(const double t, const std::vector<double>& states) const; // Used to check if we need to re-evaluate the force model or just used the cached version

        bool has_internal_frame;
        std::string known_reference_frame;
        ssc::kinematics::Wrench latest_force_in_body_frame;
        double date_of_latest_force_in_body_frame; //!< For memoization: this is used so that we can observe the force before integration without calculating the model unnecessarily
        std::vector<double> state_used_for_last_evaluation; //!< For memoization: this is used so that we can observe the force before integration without calculating the model unnecessarily
};

#endif /* FORCEMODEL_HPP_ */
