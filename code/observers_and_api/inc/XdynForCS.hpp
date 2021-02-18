#ifndef OBSERVERS_AND_API_INC_SIMSERVER_HPP_
#define OBSERVERS_AND_API_INC_SIMSERVER_HPP_

#include "ConfBuilder.hpp"
#include "YamlState.hpp"
#include "YamlSimServerInputs.hpp"
#include "SimServerInputs.hpp"

class XdynForCS
{
    public :
        XdynForCS(const std::string& yaml_model,
                  const std::string& solver,
                  const double dt);

        XdynForCS(const std::string& yaml_model,
                  const VectorOfVectorOfPoints& mesh,
                  const std::string& solver,
                  const double dt);

        std::vector<YamlState> handle(const YamlSimServerInputs& request);
        std::vector<YamlState> handle(const SimServerInputs& request);
        std::vector<YamlState> play_one_step(const SimServerInputs& request);
        double get_Tmax() const;

    private :
        XdynForCS(); // Deactivated

        ConfBuilder builder;
        const double dt;
        Sim sim;
        const std::string solver;
};

#endif /* OBSERVERS_AND_API_INC_SIMSERVER_HPP_ */
