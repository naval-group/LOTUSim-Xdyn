/*
 * AiryGRPC.hpp
 */
#ifndef GRPC_INC_AIRYGRPC_HPP_
#define GRPC_INC_AIRYGRPC_HPP_

#include "Airy.hpp"

Airy create_default_spectrum();
void run_xdyn_airy_server(const Airy& wave_spectrum);

#endif /* GRPC_INC_AIRYGRPC_HPP_ */
