/*
 * GRPCForceModel.hpp
 *
 *  Created on: Jun 17, 2019
 *      Author: cady
 */

#ifndef GRPC_INC_GRPCFORCEMODEL_HPP_
#define GRPC_INC_GRPCFORCEMODEL_HPP_

#include "EnvironmentAndFrames.hpp"
#include "ForceModel.hpp"

class GRPCForceModel : public ForceModel
{
    public:
        struct Input
        {
            std::string url;
            std::string name;
            std::string yaml;
            std::string hdb_filename;
            std::string precal_filename;
        };
        GRPCForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env);
        Wrench get_force(const BodyStates& states, const double t, const EnvironmentAndFrames& env, const std::map<std::string,double>& commands) const;
        static Input parse(const std::string& yaml);
        static std::string model_name();
        double get_Tmax() const override;

    private:
        void extra_observations(Observer& observer) const;
        GRPCForceModel(); // Disabled
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;
        GRPCForceModel(const TR1(shared_ptr)<Impl>& pimpl, const std::string& body_name, const EnvironmentAndFrames& env);

};


#endif /* GRPC_INC_GRPCFORCEMODEL_HPP_ */
