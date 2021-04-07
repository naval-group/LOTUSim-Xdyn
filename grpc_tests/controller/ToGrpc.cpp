#include "ToGrpc.hpp"

void ToGrpc::copy_from_double_vector(const std::vector<double>& origin, ::google::protobuf::RepeatedField< double >* destination) const
{
    *destination = {origin.begin(), origin.end()};
}

void ToGrpc::copy_from_string_vector(const std::vector<std::string>& origin, ::google::protobuf::RepeatedPtrField< std::string >* destination) const
{
    *destination = {origin.begin(), origin.end()};
}