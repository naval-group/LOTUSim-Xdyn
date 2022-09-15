/*
 * GRPCForceModelTest.cpp
 *
 *  Created on: Jun 17, 2019
 *      Author: cady
 */

#include "GRPCForceModel.hpp"
#include "GRPCForceModelTest.hpp"
#include "xdyn/exceptions/InvalidInputException.hpp"
#include "yaml_data.hpp"


GRPCForceModelTest::GRPCForceModelTest()
{
}

GRPCForceModelTest::~GRPCForceModelTest()
{
}

void GRPCForceModelTest::SetUp()
{
}

void GRPCForceModelTest::TearDown()
{
}

TEST_F(GRPCForceModelTest, can_parse_reference_frame)
{
    const auto input = GRPCForceModel::parse(test_data::gRPC_force_model());
    ASSERT_EQ("force-model:9002", input.url);
}

TEST_F(GRPCForceModelTest, can_parse_name)
{
    const auto input = GRPCForceModel::parse(test_data::gRPC_force_model());
    ASSERT_EQ("parametric oscillator", input.name);
}

TEST_F(GRPCForceModelTest, can_parse_rest_of_yaml_parameters)
{
    const auto input = GRPCForceModel::parse(test_data::gRPC_force_model());
    ASSERT_EQ("c: 1\n"
              "k: 60\n"
              "model: grpc\n"
              "name: parametric oscillator\n"
              "url: force-model:9002"
              , input.yaml);
}

TEST_F(GRPCForceModelTest, can_parse_hdb)
{
    const auto input = GRPCForceModel::parse(test_data::gRPC_force_model()+"hdb: some_hdb.hdb\n");
    ASSERT_EQ("some_hdb.hdb", input.hdb_filename);
}

TEST_F(GRPCForceModelTest, can_parse_precal)
{
    const auto input = GRPCForceModel::parse(test_data::gRPC_force_model()+"raodb: some_precal.ini\n");
    ASSERT_EQ("some_precal.ini", input.precal_filename);
}

TEST_F(GRPCForceModelTest, should_throw_if_both_hdb_and_precal_are_defined_in_any_order)
{
    ASSERT_THROW(GRPCForceModel::parse(test_data::gRPC_force_model()+"hdb: some_hdb.hdb\n"+"raodb: some_precal.ini\n"), InvalidInputException);
    ASSERT_THROW(GRPCForceModel::parse(test_data::gRPC_force_model()+"raodb: some_precal.ini\n"+"hdb: some_hdb.hdb\n"), InvalidInputException);
}