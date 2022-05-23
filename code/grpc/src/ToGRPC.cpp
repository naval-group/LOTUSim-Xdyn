/*
 * ToGRPC.cpp
 *
 *  Created on: Jun 26, 2019
 *      Author: cady
 */


#include "ToGRPC.hpp"
#include "ToGRPCCommon.hpp"
#include "HydroDBParser.hpp"

ToGRPC::ToGRPC(const GRPCForceModel::Input& input_)
    : input(input_)
{}

RequiredWaveInformationRequest ToGRPC::from_required_wave_information(const double t, const double x, const double y, const double z, const std::string& instance_name) const
{
    RequiredWaveInformationRequest request;
    request.set_t(t);
    request.set_x(x);
    request.set_y(y);
    request.set_z(z);
    request.set_instance_name(instance_name);
    return request;
}

SpectrumResponse* ToGRPC::from_discrete_directional_wave_spectra(const std::vector<DiscreteDirectionalWaveSpectrum>& spectra) const
{
    SpectrumResponse* spectrum_response = new SpectrumResponse();
    spectrum_response_from_discrete_directional_wave_spectra(spectra, spectrum_response);
    return spectrum_response;
}

WaveInformation* ToGRPC::from_wave_information(const WaveRequest& wave_request, const double t, const EnvironmentAndFrames& env) const
{
    WaveInformation* wave_information = new WaveInformation();
    if (env.w.use_count())
    {
        if (wave_request.need_spectrum)
        {
            try
            {
                const auto directional_spectra = env.w->get_directional_spectra(wave_request.spectrum.x, wave_request.spectrum.y, wave_request.spectrum.t);
                auto spectrum = from_discrete_directional_wave_spectra(directional_spectra);
                wave_information->set_allocated_spectrum(spectrum);
            }
            catch (const ssc::exception_handling::Exception& e)
            {
                THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which requires a linearized wave directional spectrum. When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
            }
        }
        try
        {
            wave_information->mutable_orbital_velocities()->set_t(t);
            copy_from_double_vector(wave_request.orbital_velocities.x, wave_information->mutable_orbital_velocities()->mutable_x());
            copy_from_double_vector(wave_request.orbital_velocities.y, wave_information->mutable_orbital_velocities()->mutable_y());
            copy_from_double_vector(wave_request.orbital_velocities.z, wave_information->mutable_orbital_velocities()->mutable_z());
            std::vector<double> eta;
            try
            {
                eta = env.w->get_and_check_wave_height(wave_request.orbital_velocities.x, wave_request.orbital_velocities.y, wave_request.orbital_velocities.t);
            }
            catch (const ssc::exception_handling::Exception& e)
            {
                THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which, indirectly, needs wave elevations (to compute the dynamic pressures). When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
            }
            const ssc::kinematics::PointMatrix orbital_velocities = env.w->get_and_check_orbital_velocity(env.g, wave_request.orbital_velocities.x, wave_request.orbital_velocities.y, wave_request.orbital_velocities.z, t, eta);

            std::vector<double> vx(orbital_velocities.m.cols()), vy(orbital_velocities.m.cols()), vz(orbital_velocities.m.cols());
            for (int j = 0 ; j < orbital_velocities.m.cols() ; ++j)
            {
                vx[j] = orbital_velocities.m(0,j);
                vy[j] = orbital_velocities.m(1,j);
                vz[j] = orbital_velocities.m(2,j);
            }
            copy_from_double_vector(vx, wave_information->mutable_orbital_velocities()->mutable_vx());
            copy_from_double_vector(vy, wave_information->mutable_orbital_velocities()->mutable_vy());
            copy_from_double_vector(vz, wave_information->mutable_orbital_velocities()->mutable_vz());
        }
        catch (const ssc::exception_handling::Exception& e)
        {
            THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which needs orbital velocities. When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
        }
        try
        {
            wave_information->mutable_elevations()->set_t(t);
            copy_from_double_vector(wave_request.elevations.x, wave_information->mutable_elevations()->mutable_x());
            copy_from_double_vector(wave_request.elevations.y, wave_information->mutable_elevations()->mutable_y());
            copy_from_double_vector(env.w->get_and_check_wave_height(wave_request.elevations.x, wave_request.elevations.y, wave_request.elevations.t), wave_information->mutable_elevations()->mutable_z());
        }
        catch (const ssc::exception_handling::Exception& e)
        {
            THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which needs wave elevations. When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
        }
        try
        {
            wave_information->mutable_dynamic_pressures()->set_t(t);
            copy_from_double_vector(wave_request.dynamic_pressures.x, wave_information->mutable_dynamic_pressures()->mutable_x());
            copy_from_double_vector(wave_request.dynamic_pressures.y, wave_information->mutable_dynamic_pressures()->mutable_y());
            copy_from_double_vector(wave_request.dynamic_pressures.z, wave_information->mutable_dynamic_pressures()->mutable_z());
            const std::vector<double> eta = env.w->get_and_check_wave_height(wave_request.dynamic_pressures.x, wave_request.dynamic_pressures.y, wave_request.dynamic_pressures.t);
            copy_from_double_vector(env.w->get_and_check_dynamic_pressure(env.rho, env.g, wave_request.dynamic_pressures.x, wave_request.dynamic_pressures.y, wave_request.dynamic_pressures.z, eta, t), wave_information->mutable_dynamic_pressures()->mutable_pdyn());
        }
        catch (const ssc::exception_handling::Exception& e)
        {
            THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which needs dynamic pressures. When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
        }
        try
        {
            wave_information->mutable_orbital_velocities()->set_t(t);
            copy_from_double_vector(wave_request.orbital_velocities.x, wave_information->mutable_orbital_velocities()->mutable_x());
            copy_from_double_vector(wave_request.orbital_velocities.y, wave_information->mutable_orbital_velocities()->mutable_y());
            copy_from_double_vector(wave_request.orbital_velocities.z, wave_information->mutable_orbital_velocities()->mutable_z());
        }
        catch (const ssc::exception_handling::Exception& e)
        {
            THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which needs orbital velocities. When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
        }
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which needs data from a wave model. However, none were defined in the YAML file: please define a wave model in the 'environment models' section of the YAML file.");
    }
    return wave_information;
}

States* ToGRPC::from_state(const BodyStates& state, const double max_history_length, const EnvironmentAndFrames& env) const
{
    const auto qr = state.qr.get_values(max_history_length);
    const auto qi = state.qi.get_values(max_history_length);
    const auto qj = state.qj.get_values(max_history_length);
    const auto qk = state.qk.get_values(max_history_length);
    std::vector<double> phi(qr.size());
    std::vector<double> theta(qr.size());
    std::vector<double> psi(qr.size());
    for (size_t i = 0 ; i < qr.size() ; ++i)
    {
        ssc::kinematics::RotationMatrix R = Eigen::Quaternion<double>(qr[i],qi[i],qj[i],qk[i]).matrix();
        const ssc::kinematics::EulerAngles euler_angles = state.convert(R, env.rot);
        phi[i] = euler_angles.phi;
        theta[i] = euler_angles.theta;
        psi[i] = euler_angles.psi;
    }
    States* ret = new States();

    copy_from_double_vector(state.x.get_dates(max_history_length), ret->mutable_t());
    copy_from_double_vector(state.x.get_values(max_history_length), ret->mutable_x());
    copy_from_double_vector(state.y.get_values(max_history_length), ret->mutable_y());
    copy_from_double_vector(state.z.get_values(max_history_length), ret->mutable_z());
    copy_from_double_vector(state.u.get_values(max_history_length), ret->mutable_u());
    copy_from_double_vector(state.v.get_values(max_history_length), ret->mutable_v());
    copy_from_double_vector(state.w.get_values(max_history_length), ret->mutable_w());
    copy_from_double_vector(state.p.get_values(max_history_length), ret->mutable_p());
    copy_from_double_vector(state.q.get_values(max_history_length), ret->mutable_q());
    copy_from_double_vector(state.r.get_values(max_history_length), ret->mutable_r());
    copy_from_double_vector(qr, ret->mutable_qr());
    copy_from_double_vector(qi, ret->mutable_qi());
    copy_from_double_vector(qj, ret->mutable_qj());
    copy_from_double_vector(qk, ret->mutable_qk());
    copy_from_double_vector(phi, ret->mutable_phi());
    copy_from_double_vector(theta, ret->mutable_theta());
    copy_from_double_vector(psi, ret->mutable_psi());
    copy_from_string_vector(env.rot.convention, ret->mutable_rotations_convention());
    return ret;
}

ForceRequest ToGRPC::from_force_request(States* states, const std::map<std::string, double >& commands, WaveInformation* wave_information, const std::string& instance_name, FilteredStatesAndConvention* filtered_states_and_conventions) const
{
    ForceRequest request;
    request.set_allocated_wave_information(wave_information);
    request.mutable_commands()->insert(commands.begin(), commands.end());
    request.set_allocated_states(states);
    request.set_instance_name(instance_name);
    request.set_allocated_filtered_states(filtered_states_and_conventions);
    return request;
}

AddedMassMatrix* get_added_mass_matrix(const std::shared_ptr<HydroDBParser>& hydro_db_parser);
AddedMassMatrix* get_added_mass_matrix(const std::shared_ptr<HydroDBParser>& hydro_db_parser)
{
    AddedMassMatrix* ma = new AddedMassMatrix();
    const auto Ma = hydro_db_parser->get_added_mass();
    ma->set_ma_11(Ma(0,0));
    ma->set_ma_12(Ma(0,1));
    ma->set_ma_13(Ma(0,2));
    ma->set_ma_14(Ma(0,3));
    ma->set_ma_15(Ma(0,4));
    ma->set_ma_16(Ma(0,5));
    ma->set_ma_21(Ma(1,0));
    ma->set_ma_22(Ma(1,1));
    ma->set_ma_23(Ma(1,2));
    ma->set_ma_24(Ma(1,3));
    ma->set_ma_25(Ma(1,4));
    ma->set_ma_26(Ma(1,5));
    ma->set_ma_31(Ma(2,0));
    ma->set_ma_32(Ma(2,1));
    ma->set_ma_33(Ma(2,2));
    ma->set_ma_34(Ma(2,3));
    ma->set_ma_35(Ma(2,4));
    ma->set_ma_36(Ma(2,5));
    ma->set_ma_41(Ma(3,0));
    ma->set_ma_42(Ma(3,1));
    ma->set_ma_43(Ma(3,2));
    ma->set_ma_44(Ma(3,3));
    ma->set_ma_45(Ma(3,4));
    ma->set_ma_46(Ma(3,5));
    ma->set_ma_51(Ma(4,0));
    ma->set_ma_52(Ma(4,1));
    ma->set_ma_53(Ma(4,2));
    ma->set_ma_54(Ma(4,3));
    ma->set_ma_55(Ma(4,4));
    ma->set_ma_56(Ma(4,5));
    ma->set_ma_61(Ma(5,0));
    ma->set_ma_62(Ma(5,1));
    ma->set_ma_63(Ma(5,2));
    ma->set_ma_64(Ma(5,3));
    ma->set_ma_65(Ma(5,4));
    ma->set_ma_66(Ma(5,5));
    return ma;
}

Array* to_grpc_array(const std::vector<std::vector<double> >& array);
Array* to_grpc_array(const std::vector<std::vector<double> >& array)
{
    Array* ret = new Array();
    for (const auto& line : array)
    {
        if (not(line.empty()))
        {
            Line* grpc_line = ret->add_line();
            copy_from_double_vector(line, grpc_line->mutable_element());
        }
    }
    return ret;
}

WrenchMatrices* to_WrenchMatrices(const std::array<std::vector<std::vector<double> >,6 >& T);
WrenchMatrices* to_WrenchMatrices(const std::array<std::vector<std::vector<double> >,6 >& T)
{
    WrenchMatrices* ret = new WrenchMatrices();
    ret->set_allocated_x(to_grpc_array(T[0]));
    ret->set_allocated_y(to_grpc_array(T[1]));
    ret->set_allocated_z(to_grpc_array(T[2]));
    ret->set_allocated_k(to_grpc_array(T[3]));
    ret->set_allocated_m(to_grpc_array(T[4]));
    ret->set_allocated_n(to_grpc_array(T[5]));
    return ret;
}

FrequencyMatrix* get_added_mass_coeff_matrix(const std::shared_ptr<HydroDBParser>& hydro_db_parser);
FrequencyMatrix* get_added_mass_coeff_matrix(const std::shared_ptr<HydroDBParser>& hydro_db_parser)
{
    FrequencyMatrix* ret = new FrequencyMatrix();
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(0,0), ret->mutable_ma_11());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(0,1), ret->mutable_ma_12());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(0,2), ret->mutable_ma_13());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(0,3), ret->mutable_ma_14());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(0,4), ret->mutable_ma_15());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(0,5), ret->mutable_ma_16());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(1,0), ret->mutable_ma_21());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(1,1), ret->mutable_ma_22());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(1,2), ret->mutable_ma_23());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(1,3), ret->mutable_ma_24());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(1,4), ret->mutable_ma_25());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(1,5), ret->mutable_ma_26());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(2,0), ret->mutable_ma_31());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(2,1), ret->mutable_ma_32());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(2,2), ret->mutable_ma_33());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(2,3), ret->mutable_ma_34());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(2,4), ret->mutable_ma_35());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(2,5), ret->mutable_ma_36());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(3,0), ret->mutable_ma_41());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(3,1), ret->mutable_ma_42());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(3,2), ret->mutable_ma_43());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(3,3), ret->mutable_ma_44());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(3,4), ret->mutable_ma_45());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(3,5), ret->mutable_ma_46());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(4,0), ret->mutable_ma_51());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(4,1), ret->mutable_ma_52());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(4,2), ret->mutable_ma_53());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(4,3), ret->mutable_ma_54());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(4,4), ret->mutable_ma_55());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(4,5), ret->mutable_ma_56());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(5,0), ret->mutable_ma_61());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(5,1), ret->mutable_ma_62());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(5,2), ret->mutable_ma_63());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(5,3), ret->mutable_ma_64());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(5,4), ret->mutable_ma_65());
    copy_from_double_vector(hydro_db_parser->get_added_mass_coeff(5,5), ret->mutable_ma_66());
    return ret;
}

FrequencyMatrix* get_radiation_damping_coeff_matrix(const std::shared_ptr<HydroDBParser>& hydro_db_parser);
FrequencyMatrix* get_radiation_damping_coeff_matrix(const std::shared_ptr<HydroDBParser>& hydro_db_parser)
{
    FrequencyMatrix* ret = new FrequencyMatrix();
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(0,0), ret->mutable_ma_11());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(0,1), ret->mutable_ma_12());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(0,2), ret->mutable_ma_13());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(0,3), ret->mutable_ma_14());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(0,4), ret->mutable_ma_15());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(0,5), ret->mutable_ma_16());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(1,0), ret->mutable_ma_21());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(1,1), ret->mutable_ma_22());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(1,2), ret->mutable_ma_23());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(1,3), ret->mutable_ma_24());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(1,4), ret->mutable_ma_25());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(1,5), ret->mutable_ma_26());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(2,0), ret->mutable_ma_31());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(2,1), ret->mutable_ma_32());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(2,2), ret->mutable_ma_33());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(2,3), ret->mutable_ma_34());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(2,4), ret->mutable_ma_35());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(2,5), ret->mutable_ma_36());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(3,0), ret->mutable_ma_41());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(3,1), ret->mutable_ma_42());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(3,2), ret->mutable_ma_43());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(3,3), ret->mutable_ma_44());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(3,4), ret->mutable_ma_45());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(3,5), ret->mutable_ma_46());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(4,0), ret->mutable_ma_51());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(4,1), ret->mutable_ma_52());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(4,2), ret->mutable_ma_53());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(4,3), ret->mutable_ma_54());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(4,4), ret->mutable_ma_55());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(4,5), ret->mutable_ma_56());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(5,0), ret->mutable_ma_61());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(5,1), ret->mutable_ma_62());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(5,2), ret->mutable_ma_63());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(5,3), ret->mutable_ma_64());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(5,4), ret->mutable_ma_65());
    copy_from_double_vector(hydro_db_parser->get_radiation_damping_coeff(5,5), ret->mutable_ma_66());
    return ret;
}

FilteredStatesAndConvention* ToGRPC::from_filtered_states(const FilteredStates& filtered_states) const
{
    FilteredStatesAndConvention* ret = new FilteredStatesAndConvention();
    ret->set_x(filtered_states.x);
    ret->set_y(filtered_states.y);
    ret->set_z(filtered_states.z);
    ret->set_u(filtered_states.u);
    ret->set_v(filtered_states.v);
    ret->set_w(filtered_states.w);
    ret->set_p(filtered_states.p);
    ret->set_q(filtered_states.q);
    ret->set_r(filtered_states.r);
    ret->set_phi(filtered_states.phi);
    ret->set_theta(filtered_states.theta);
    ret->set_psi(filtered_states.psi);
    copy_from_string_vector(filtered_states.rotation_convention.convention, ret->mutable_rotations_convention());
    return ret;
}

ResultsFromPotentialTheory* get_results_from_potential_theory(const std::shared_ptr<HydroDBParser>& hydro_db_parser);
ResultsFromPotentialTheory* get_results_from_potential_theory(const std::shared_ptr<HydroDBParser>& hydro_db_parser)
{
    ResultsFromPotentialTheory* pot = new ResultsFromPotentialTheory();
    if (hydro_db_parser.use_count())
    {
        pot->set_allocated_ma(get_added_mass_matrix(hydro_db_parser));
        pot->set_allocated_diffraction_module_tables(to_WrenchMatrices(hydro_db_parser->get_diffraction_module_tables()));
        pot->set_allocated_diffraction_phase_tables(to_WrenchMatrices(hydro_db_parser->get_diffraction_phase_tables()));
        copy_from_double_vector(hydro_db_parser->get_diffraction_module_periods(), pot->mutable_diffraction_module_periods());
        copy_from_double_vector(hydro_db_parser->get_diffraction_phase_periods(), pot->mutable_diffraction_phase_periods());
        copy_from_double_vector(hydro_db_parser->get_diffraction_module_psis(), pot->mutable_diffraction_module_psis());
        copy_from_double_vector(hydro_db_parser->get_diffraction_phase_psis(), pot->mutable_diffraction_phase_psis());
        pot->set_allocated_froude_krylov_module_tables(to_WrenchMatrices(hydro_db_parser->get_froude_krylov_module_tables()));
        pot->set_allocated_froude_krylov_phase_tables(to_WrenchMatrices(hydro_db_parser->get_froude_krylov_phase_tables()));
        copy_from_double_vector(hydro_db_parser->get_froude_krylov_module_periods(), pot->mutable_froude_krylov_module_periods());
        copy_from_double_vector(hydro_db_parser->get_froude_krylov_phase_periods(), pot->mutable_froude_krylov_phase_periods());
        copy_from_double_vector(hydro_db_parser->get_froude_krylov_module_psis(), pot->mutable_froude_krylov_module_psis());
        copy_from_double_vector(hydro_db_parser->get_froude_krylov_phase_psis(), pot->mutable_froude_krylov_phase_psis());
        copy_from_double_vector(hydro_db_parser->get_angular_frequencies(), pot->mutable_angular_frequencies());
        pot->set_forward_speed(hydro_db_parser->get_forward_speed());
        pot->set_allocated_added_mass_coeff(get_added_mass_coeff_matrix(hydro_db_parser));
        pot->set_allocated_radiation_damping_coeff(get_radiation_damping_coeff_matrix(hydro_db_parser));
        copy_from_double_vector(hydro_db_parser->get_wave_drift_periods(), pot->mutable_wave_drift_periods());
        copy_from_double_vector(hydro_db_parser->get_wave_drift_psis(), pot->mutable_wave_drift_psis());
        pot->set_allocated_wave_drift_force_tables(to_WrenchMatrices(hydro_db_parser->get_wave_drift_tables()));
    }
    return pot;
}

SetForceParameterRequest ToGRPC::from_yaml(const std::string& yaml, const std::string body_name, const std::string& instance_name, const std::shared_ptr<HydroDBParser>& hydro_db_parser) const
{
    SetForceParameterRequest request;
    request.set_parameters(yaml);
    request.set_body_name(body_name);
    request.set_instance_name(instance_name);
    request.set_allocated_results_from_potential_theory(get_results_from_potential_theory(hydro_db_parser));
    return request;
}
