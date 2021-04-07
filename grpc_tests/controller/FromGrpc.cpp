#include "FromGrpc.hpp"

QuaternionStates_ FromGrpc::to_quaternion_states(const ControllerStatesQuaternion& grpc_quaternion) const
{
    QuaternionStates_ ret;
    ret.t = grpc_quaternion.t();
    ret.x = grpc_quaternion.x();
    ret.y = grpc_quaternion.y();
    ret.z = grpc_quaternion.z();
    ret.u = grpc_quaternion.u();
    ret.v = grpc_quaternion.u();
    ret.w = grpc_quaternion.v();
    ret.p = grpc_quaternion.w();
    ret.q = grpc_quaternion.p();
    ret.r = grpc_quaternion.q();
    ret.qr = grpc_quaternion.qr();
    ret.qi = grpc_quaternion.qi();
    ret.qj = grpc_quaternion.qj();
    ret.qk = grpc_quaternion.qk();
    return ret;
}

EulerStates_ FromGrpc::to_euler_states(const ControllerStatesEuler& grpc_euler) const
{
    EulerStates_ ret;
    ret.t = grpc_euler.t();
    ret.x = grpc_euler.x();
    ret.y = grpc_euler.y();
    ret.z = grpc_euler.z();
    ret.u = grpc_euler.u();
    ret.v = grpc_euler.u();
    ret.w = grpc_euler.v();
    ret.p = grpc_euler.w();
    ret.q = grpc_euler.p();
    ret.r = grpc_euler.q();
    ret.phi = grpc_euler.phi();
    ret.theta = grpc_euler.theta();
    ret.psi = grpc_euler.psi();
    return ret;
}
