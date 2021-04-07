#ifndef FROMGRPC_HPP
#define FROMGRPC_HPP

#include "controller.pb.h"
#include "Controller.hpp"


class FromGrpc
{
    public:
        QuaternionStates_ to_quaternion_states(const ControllerStatesQuaternion& grpc_quaternion) const;
        EulerStates_ to_euler_states(const ControllerStatesEuler& grpc_euler) const;
};

#endif
