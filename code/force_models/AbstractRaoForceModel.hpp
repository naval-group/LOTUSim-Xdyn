/*
 * AbstractRaoForceModel.hpp
 *
 */

#ifndef ABSTRACTRAOFORCEMODEL_HPP_
#define ABSTRACTRAOFORCEMODEL_HPP_

#include <ssc/macros.hpp>

#include "xdyn/core/ForceModel.hpp"
#include "xdyn/core/EnvironmentAndFrames.hpp"
#include "PhaseModuleRAOEvaluator.hpp"
#include "external_data_structures/YamlRAO.hpp"

/** \brief Abstracts the commonalities between linear Froude-Krylov & diffraction force models
 */
class AbstractRaoForceModel : public ForceModel
{
    public:
        typedef YamlRAO Input;
        AbstractRaoForceModel(const Input& data, const std::string& body_name, const EnvironmentAndFrames& env);
        AbstractRaoForceModel(const Input& data, const std::string& body_name, const EnvironmentAndFrames& env, const std::string& hdb_file_contents);
        Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const override;
        static Input parse(const std::string& yaml, const YamlRAO::TypeOfRao& type_of_rao);

    protected:
        static std::string get_model_name(const YamlRAO::TypeOfRao& type_of_rao);

    private:
        AbstractRaoForceModel();
        TR1(shared_ptr)<PhaseModuleRAOEvaluator> pimpl;

};

#endif /* ABSTRACTRAOFORCEMODEL_HPP_ */
