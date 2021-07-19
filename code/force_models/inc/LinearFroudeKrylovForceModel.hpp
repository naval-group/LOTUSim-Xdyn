/*
 * LinearFroudeKrylovForceModel.hpp
 *
 */

#ifndef LINEARFROUDEKRYLOVFORCEMODEL_HPP_
#define LINEARFROUDEKRYLOVFORCEMODEL_HPP_

#include "AbstractRaoForceModel.hpp"

/** \brief Linear Froude-Krylov forces, from RAO
 *  \details
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 *  \section ex1 Example
 *  \snippet model_wrappers/unit_tests/src/LinearFroudeKrylovForceModelTest.cpp LinearFroudeKrylovForceModelTest example
 *  \section ex2 Expected output
 *  \snippet model_wrappers/unit_tests/src/LinearFroudeKrylovForceModelTest.cpp LinearFroudeKrylovForceModelTest expected output
 */
class LinearFroudeKrylovForceModel : public AbstractRaoForceModel
{
    public:
        typedef YamlRAO Input;
        LinearFroudeKrylovForceModel(const Input& data, const std::string& body_name, const EnvironmentAndFrames& env);
        LinearFroudeKrylovForceModel(const Input& data, const std::string& body_name, const EnvironmentAndFrames& env, const std::string& hdb_file_contents);
        static Input parse(const std::string& yaml);
        static std::string model_name();

    private:
        LinearFroudeKrylovForceModel();
};

#endif /* LINEARFROUDEKRYLOVFORCEMODEL_HPP_ */
