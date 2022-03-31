#include "CSVLineByLineReaderTest.hpp"
#include "CSVYaml.hpp"

CSVLineByLineReaderTest::CSVLineByLineReaderTest()
    : csv()
{
    csv << "t,some column,rpm_co,beta_co\n";
    csv << "0.2,23,65,78\n";
    csv << "0.2001,27,665,778\n";
    csv << "2.001,28.5,6.65,4.778\n";
    csv << "2.01,2.85,66.5,47.78\n";
    csv.close();
}


CSVLineByLineReaderTest::~CSVLineByLineReaderTest() {}

void CSVLineByLineReaderTest::SetUp() {}

void CSVLineByLineReaderTest::TearDown() {}

CSVLineByLineReader CSVLineByLineReaderTest::get_reader() const
{
    CSVYaml yaml;
    yaml.path = csv.get_filename();
    yaml.separator = ',';
    yaml.time_column = "t";
    yaml.commands["port side propeller(beta)"] = "beta_co";
    yaml.commands["port side propeller(rpm)"] = "rpm_co";
    return CSVLineByLineReader(yaml);
}

TEST_F(CSVLineByLineReaderTest, can_get_values_at_time_values_in_csv)
{
    CSVLineByLineReader reader = get_reader();
    std::unordered_map<std::string, double> values = reader.get_values(0.2);
    ASSERT_DOUBLE_EQ(65, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(78, values["port side propeller(beta)"]);
    values = reader.get_values(0.2001);
    ASSERT_DOUBLE_EQ(665, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(778, values["port side propeller(beta)"]);
    values = reader.get_values(2.001);
    ASSERT_DOUBLE_EQ(6.65, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(4.778, values["port side propeller(beta)"]);
}

TEST_F(CSVLineByLineReaderTest, can_get_correct_value_for_time_between_timestamps)
{
    CSVLineByLineReader reader = get_reader();
    std::unordered_map<std::string, double> values = reader.get_values(0.20005);
    ASSERT_DOUBLE_EQ(65, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(78, values["port side propeller(beta)"]);
    values = reader.get_values(1);
    ASSERT_DOUBLE_EQ(665, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(778, values["port side propeller(beta)"]);
}