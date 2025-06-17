// /*
//  * NocUWCurrentModel.hpp
//  *
//  *  Created on: 13 may 2024
//  *      Author: julien.prodhon
//  */

// // Naval Group Far East - CoE
// // Creating files for UW current class

// #ifndef ENVIRONMENT_MODELS_INC_NOCUWCurrentMODEL_HPP_
// #define ENVIRONMENT_MODELS_INC_NOCUWCurrentMODEL_HPP_

// #include <string>

// #include "UWCurrentModel.hpp"

// class NocUWCurrentModel : public UWCurrentModel
// {
// public:
//     struct Input
//     {
//         Input();
//         double orientation;
//         double mean_velocity;
//         double upper_depth;
//         double lower_depth;
//     };
//     NocUWCurrentModel(const Input& input);     
//     NocUWCurrentModel(int); 
//     virtual ~NocUWCurrentModel();

//     virtual Eigen::Vector3d get_UWCurrent(const Eigen::Vector3d& position, const double t, const double) const override;

//     static std::string model_name();
//     static Input parse(const std::string&);

// private:
//     double orientation;
//     double mean_velocity; // mean
//     double upper_depth; // depth 1
//     double lower_depth; // depth 2
// };

// #endif /* ENVIRONMENT_MODELS_INC_NOCUWCurrentMODEL_HPP_ */
