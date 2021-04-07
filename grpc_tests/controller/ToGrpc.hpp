#ifndef TOGRPCHPP
#define TOGRPCHPP

#include <vector>
#include <grpcpp/grpcpp.h>
#include "controller.pb.h"

class ToGrpc
{
    public:
        void copy_from_double_vector(const std::vector<double>& origin, ::google::protobuf::RepeatedField< double >* destination) const;
        void copy_from_string_vector(const std::vector<std::string>& origin, ::google::protobuf::RepeatedPtrField< std::string >* destination) const;
};

#endif
