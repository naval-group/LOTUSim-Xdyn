/*
 * DiffractionForceModel.hpp
 *
 */

#ifndef DIFFRACTIONFORCEMODEL_HPP_
#define DIFFRACTIONFORCEMODEL_HPP_

#include "AbstractRaoForceModel.hpp"

/** \brief Diffraction forces
 *  \details
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 *  \section ex1 Example
 *  \snippet model_wrappers/unit_tests/src/DiffractionForceModelTest.cpp DiffractionForceModelTest example
 *  \section ex2 Expected output
 *  \snippet model_wrappers/unit_tests/src/DiffractionForceModelTest.cpp DiffractionForceModelTest expected output
 */
class DiffractionForceModel : public AbstractRaoForceModel
{
    public:
        typedef YamlRAO Input;
        DiffractionForceModel(const Input& data, const std::string& body_name, const EnvironmentAndFrames& env);
        DiffractionForceModel(const Input& data, const std::string& body_name, const EnvironmentAndFrames& env, const std::string& hdb_file_contents);
        static Input parse(const std::string& yaml);
        static std::string model_name();

    private:
        DiffractionForceModel();
};

#endif /* DIFFRACTIONFORCEMODEL_HPP_ */
