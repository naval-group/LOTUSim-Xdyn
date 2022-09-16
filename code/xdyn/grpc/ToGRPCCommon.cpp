#include "ToGRPCCommon.hpp"
#include <ssc/exception_handling/Exception.hpp>

void flat_spectrum_response_from_vector_of_flat_discrete_directional_wave_spectra(
    const std::vector<FlatDiscreteDirectionalWaveSpectrum>& spectra,
    SpectrumResponse* spectrum_response)
{
    if (spectrum_response==nullptr)
    {
        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception,
        "Null pointer passed as input for SpectrumResponse in from_discrete_directional_wave_spectra");
    }
    for (const auto& spectrum:spectra)
    {
        for (const auto& a:spectrum.a)
        {
            spectrum_response->add_a(a);
        }
        for (const auto& psi:spectrum.psi)
        {
            spectrum_response->add_psi(psi);
        }
        for (const auto& omega:spectrum.omega)
        {
            spectrum_response->add_omega(omega);
        }
        for (const auto& k:spectrum.k)
        {
            spectrum_response->add_k(k);
        }
        for (const auto& phase:spectrum.phase)
        {
            spectrum_response->add_phase(phase);
        }
    }
}

void copy_from_double_vector(const std::vector<double>& origin, ::google::protobuf::RepeatedField< double >* destination)
{
    *destination = {origin.begin(), origin.end()};
}

void copy_from_string_vector(const std::vector<std::string>& origin, ::google::protobuf::RepeatedPtrField< std::string >* destination)
{
    *destination = {origin.begin(), origin.end()};
}
