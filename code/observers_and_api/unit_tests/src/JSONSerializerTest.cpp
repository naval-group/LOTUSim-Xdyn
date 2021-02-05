#include "JSONSerializer.hpp"
#include "SimServerInputs.hpp"
#include "yaml_data.hpp"

#include "JSONSerializerTest.hpp"

JSONSerializerTest::JSONSerializerTest(): a(ssc::random_data_generator::DataGenerator(42)), yaml(
        "{\"Dt\": 12\n"
        ",\"states\":\n"
        "    [ {\"t\": 9.123 , \"x\": 1.123, \"y\": 2.123, \"z\": 3.123, \"u\": 4.123, \"v\": 5.123, \"w\": 6.123, \"p\": 7.123, \"q\": 8.123, \"r\": 9.123, \"qr\": 10.123, \"qi\": 11.123, \"qj\": 12.123, \"qk\": 13.132}\n"
        "    , {\"t\": 10.123, \"x\": 33.98, \"y\": 34.98, \"z\": 35.98, \"u\": 36.98, \"v\": 37.98, \"w\": 38.98, \"p\": 39.98, \"q\": 40.98, \"r\": 41.98, \"qr\": 42.98, \"qi\": 43.98, \"qj\": 44.98, \"qk\": 45.132}\n"
        "    , {\"t\": 51.123, \"x\": 52.32, \"y\": 53.32, \"z\": 54.32, \"u\": 55.32, \"v\": 56.32, \"w\": 57.32, \"p\": 58.32, \"q\": 59.32, \"r\": 60.32, \"qr\": 61.32, \"qi\": 62.32, \"qj\": 63.32, \"qk\": 64.132}]\n"
        ",\"commands\":\n"
        " {   \"a\": 3,\n"
        "    \"b\": 5,\n"
        "    \"c\": -12.3}}")
{
}

JSONSerializerTest::~JSONSerializerTest()
{
}

TEST_F(JSONSerializerTest, can_parse_simple_history)
{
    double t_list[5] = {0,0.1,0.2,0.3,0.4};
    double x0=90, y0=91, z0=92, u0=93, v0=94, w0=95, p0=96, q0=97, r0=98, qr0=99, qi0=100, qj0=101, qk0=102;
    double state_step = -20;
    size_t size_test = 5;

    YamlSimServerInputs history = deserialize(test_data::dummy_history());

    ASSERT_EQ(1234.5, history.Dt);
    ASSERT_EQ(size_test, history.states.size());

    for(size_t i = 0; i < size_test ; ++i)
    {
        ASSERT_EQ(t_list[i], history.states[i].t);
        ASSERT_EQ(x0+((double)i)*state_step, history.states[i].x);
        ASSERT_EQ(y0+((double)i)*state_step, history.states[i].y);
        ASSERT_EQ(z0+((double)i)*state_step, history.states[i].z);
        ASSERT_EQ(u0+((double)i)*state_step, history.states[i].u);
        ASSERT_EQ(v0+((double)i)*state_step, history.states[i].v);
        ASSERT_EQ(w0+((double)i)*state_step, history.states[i].w);
        ASSERT_EQ(p0+((double)i)*state_step, history.states[i].p);
        ASSERT_EQ(q0+((double)i)*state_step, history.states[i].q);
        ASSERT_EQ(r0+((double)i)*state_step, history.states[i].r);
        ASSERT_EQ(qr0+((double)i)*state_step, history.states[i].qr);
        ASSERT_EQ(qi0+((double)i)*state_step, history.states[i].qi);
        ASSERT_EQ(qj0+((double)i)*state_step, history.states[i].qj);
        ASSERT_EQ(qk0+((double)i)*state_step, history.states[i].qk);
    }
}

TEST_F(JSONSerializerTest, can_parse_commands)
{
    YamlSimServerInputs yinfos = deserialize(test_data::complete_yaml_message_from_gui());
    ASSERT_EQ(yinfos.commands.find("RPM")->second, 1.2);
    ASSERT_EQ(yinfos.commands.find("B1")->second, 0.1);
}

TEST_F(JSONSerializerTest, Dt_should_not_be_compulsory)
{
    YamlSimServerInputs yinfos = deserialize(test_data::simserver_message_without_Dt());
    ASSERT_EQ(yinfos.Dt, 0);
}

TEST_F(JSONSerializerTest, can_parse_Dt)
{
    const SimServerInputs s(deserialize(yaml), 100);
    ASSERT_DOUBLE_EQ(12, s.Dt);
}

TEST_F(JSONSerializerTest, can_parse_commands_into_SimServerInputs)
{
    const SimServerInputs s(deserialize(yaml), 100);
    ASSERT_DOUBLE_EQ(3, s.commands.at("a"));
    ASSERT_DOUBLE_EQ(5, s.commands.at("b"));
    ASSERT_DOUBLE_EQ(-12.3, s.commands.at("c"));
}

TEST_F(JSONSerializerTest, can_parse_state_at_t_into_SimServerInputs)
{
    const SimServerInputs s(deserialize(yaml), 100);
    ASSERT_EQ(13, s.state_at_t.size());
    ASSERT_EQ(52.32, s.state_at_t.at(0));
    ASSERT_EQ(53.32, s.state_at_t.at(1));
    ASSERT_EQ(54.32, s.state_at_t.at(2));
    ASSERT_EQ(55.32, s.state_at_t.at(3));
    ASSERT_EQ(56.32, s.state_at_t.at(4));
    ASSERT_EQ(57.32, s.state_at_t.at(5));
    ASSERT_EQ(58.32, s.state_at_t.at(6));
    ASSERT_EQ(59.32, s.state_at_t.at(7));
    ASSERT_EQ(60.32, s.state_at_t.at(8));
    ASSERT_EQ(61.32, s.state_at_t.at(9));
    ASSERT_EQ(62.32, s.state_at_t.at(10));
    ASSERT_EQ(63.32, s.state_at_t.at(11));
    ASSERT_EQ(64.132, s.state_at_t.at(12));
}

TEST_F(JSONSerializerTest, can_parse_all_states_before_t_into_SimServerInputs)
{
    const SimServerInputs s(deserialize(yaml), 100);
    ASSERT_EQ(2, s.state_history_except_last_point.x.size());
}

TEST_F(JSONSerializerTest, t_should_correspond_to_last_value_in_yaml_into_SimServerInputs)
{
    const SimServerInputs s(deserialize(yaml), 100);
    ASSERT_DOUBLE_EQ(51.123, s.t);
}
