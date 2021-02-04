#ifndef OBSERVERS_AND_API_INC_SIMSERVER_HPP_
#define OBSERVERS_AND_API_INC_SIMSERVER_HPP_

#include "ConfBuilder.hpp"
#include "HistoryParser.hpp"
#include "YamlState.hpp"
#include "YamlSimServerInputs.hpp"

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

        std::vector<YamlState> play_one_step(const std::string& raw_yaml);
        std::vector<YamlState> play_one_step(const SimServerInputs& raw_yaml);
        std::vector<YamlState> play_one_step(const YamlSimServerInputs& inputs);

        std::vector<YamlState> step(const SimServerInputs& input, double Dt);

    private :
        XdynForCS(); // Deactivated

        ConfBuilder builder;
        const double dt;
        Sim sim;
        const std::string solver;
};

#endif /* OBSERVERS_AND_API_INC_SIMSERVER_HPP_ */
