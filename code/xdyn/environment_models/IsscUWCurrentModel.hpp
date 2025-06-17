// /*
//  * IsscUWCurrentModel.hpp
//  *
//  *  Created on: 13 may 2024
//  *      Author: julien.prodhon
//  */

// // Naval Group Far East - CoE
// // Creating files for UW current class

// #ifndef ENVIRONMENT_MODELS_INC_ISSCUWCurrentMODEL_HPP_
// #define ENVIRONMENT_MODELS_INC_ISSCUWCurrentMODEL_HPP_

// #include <string>

// #include "UWCurrentModel.hpp"

// class IsscUWCurrentModel : public UWCurrentModel
// {
// public:
//     struct Input
//     {
//         Input();
//         double orientation;
//         double mean_velocity;
//         double upper_depth;
//         double tide_velocity;
//         double U10;
//     };
//     IsscUWCurrentModel(const Input& input);     
//     IsscUWCurrentModel(int);
//     virtual ~IsscUWCurrentModel();

//     virtual Eigen::Vector3d get_UWCurrent(const Eigen::Vector3d& position, const double t, const double) const override;

//     static std::string model_name();
//     static Input parse(const std::string&);

// private:
//     double orientation;
//     double mean_velocity; // mean
//     double upper_depth; // depth 0
//     double tide_velocity;
//     double U10; // for wave
// };

// #endif /* ENVIRONMENT_MODELS_INC_ISSCUWCurrentMODEL_HPP_ */
