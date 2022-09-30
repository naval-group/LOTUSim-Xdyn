#include "CSVLineByLineReaderTest.hpp"
#include "CSVYaml.hpp"
#include "TempFile.hpp"
#include "xdyn/exceptions/InvalidInputException.hpp"

CSVLineByLineReaderTest::CSVLineByLineReaderTest() : csv(), a(29081981)
{
}

void happy_case(TempFile& csv);
void happy_case(TempFile& csv)
{
    csv << "t,some column,rpm_co,beta_co\n";
    csv << "0.2,23,65,78\n";
    csv << "0.2001,27,665,778\n";
    csv << "2.001,28.5,6.65,4.778\n";
    csv << "2.01,2.85,66.5,47.78\n";
    csv.close();
}

void linear_increasing_time_starting_at_zero(TempFile& csv);
void linear_increasing_time_starting_at_zero(TempFile& csv)
{
    csv << "t,some column,rpm_co,beta_co\n";
    csv << "0.0,1,2,3\n";
    csv << "0.1,2,4,6\n";
    csv << "0.2,3,6,9\n";
    csv << "0.3,4,8,12\n";
    csv << "0.4,5,10,15\n";
    csv.close();
}

void linear_increasing_time_starting_with_negative_times(TempFile& csv);
void linear_increasing_time_starting_with_negative_times(TempFile& csv)
{
    csv << "t,some column,rpm_co,beta_co\n";
    csv << "-0.2,-1,-2,-3\n";
    csv << "-0.1,0,0,0\n";
    csv << "0.0,1,2,3\n";
    csv << "0.1,2,4,6\n";
    csv << "0.2,3,6,9\n";
    csv << "0.3,4,8,12\n";
    csv << "0.4,5,10,15\n";
    csv.close();
}

void single_line(TempFile& csv);
void single_line(TempFile& csv)
{
    csv << "t,some column,rpm_co,beta_co\n";
    csv << "0.2,23,65,78\n";
    csv.close();
}

void empty(TempFile& csv);
void empty(TempFile& csv)
{
    csv << "t,some column,rpm_co,beta_co\n";
    csv.close();
}

void non_decreasing(TempFile& csv);
void non_decreasing(TempFile& csv)
{
    csv << "t,some column,rpm_co,beta_co\n";
    csv << "0.2,23,65,78\n";
    csv << "2.001,28.5,6.65,4.778\n";
    csv << "0.2001,27,665,778\n";
    csv << "2.01,2.85,66.5,47.78\n";
    csv.close();
}

void duplicate_column(TempFile& csv);
void duplicate_column(TempFile& csv)
{
    csv << "t,rpm_co,rpm_co,beta_co\n";
    csv << "0.2,23,65,78\n";
    csv << "0.2001,27,665,778\n";
    csv << "2.001,28.5,6.65,4.778\n";
    csv << "2.01,2.85,66.5,47.78\n";
    csv.close();
}

void non_existent_command_column(TempFile& csv);
void non_existent_command_column(TempFile& csv)
{
    csv << "t,some column,rpm_co,beta\n";
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
    happy_case(csv);
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
    happy_case(csv);
    CSVLineByLineReader reader = get_reader();
    std::unordered_map<std::string, double> values = reader.get_values(0.20005);
    ASSERT_DOUBLE_EQ(65, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(78, values["port side propeller(beta)"]);
    values = reader.get_values(1);
    ASSERT_DOUBLE_EQ(665, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(778, values["port side propeller(beta)"]);
}

TEST_F(CSVLineByLineReaderTest, can_get_correct_value_before_first_timestamp_and_after_last_timestamp)
{
    happy_case(csv);
    CSVLineByLineReader reader = get_reader();
    std::unordered_map<std::string, double> values = reader.get_values(0.005);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(beta)"]);
    values = reader.get_values(3);
    ASSERT_DOUBLE_EQ(66.5, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(47.78, values["port side propeller(beta)"]);
}

void check_positive_linear_spaced_requests(CSVLineByLineReader& reader);
void check_positive_linear_spaced_requests(CSVLineByLineReader& reader)
{
    // Check value at t=0.0
    std::unordered_map<std::string, double> values = reader.get_values(0.0);
    ASSERT_DOUBLE_EQ(2, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(3, values["port side propeller(beta)"]);
    // Check value at t=0.1
    values = reader.get_values(0.1);
    ASSERT_DOUBLE_EQ(4, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(6, values["port side propeller(beta)"]);
    // Check value at t=0.2
    values = reader.get_values(0.2);
    ASSERT_DOUBLE_EQ(6, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(9, values["port side propeller(beta)"]);
    // Check value at t=0.3
    values = reader.get_values(0.3);
    ASSERT_DOUBLE_EQ(8, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(12, values["port side propeller(beta)"]);
    // Check value at t=0.35 (between timestamps 0.3 and 0.4)
    values = reader.get_values(0.35);
    ASSERT_DOUBLE_EQ(8, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(12, values["port side propeller(beta)"]);
    // Check value at t=0.4
    values = reader.get_values(0.4);
    ASSERT_DOUBLE_EQ(10, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(15, values["port side propeller(beta)"]);
    // Check values after last timestamps
    for (size_t i=0;i<10;++i)
    {
        values = reader.get_values(0.4 + static_cast<double>(i));
        ASSERT_DOUBLE_EQ(10, values["port side propeller(rpm)"]);
        ASSERT_DOUBLE_EQ(15, values["port side propeller(beta)"]);
    }
}

TEST_F(CSVLineByLineReaderTest, can_get_correct_value_with_linear_increasing_time_starting_at_zero)
{
    linear_increasing_time_starting_at_zero(csv);
    CSVLineByLineReader reader = get_reader();
    // Before first timestamp, returned values are zero
    std::unordered_map<std::string, double> values = reader.get_values(-1.0);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(beta)"]);
    // Check value at t=0.4
    values = reader.get_values(0.4);
    ASSERT_DOUBLE_EQ(10, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(15, values["port side propeller(beta)"]);
    check_positive_linear_spaced_requests(reader);
}

TEST_F(CSVLineByLineReaderTest, can_get_correct_value_with_linear_increasing_time_starting_with_negative_times)
{
    linear_increasing_time_starting_with_negative_times(csv);
    CSVLineByLineReader reader = get_reader();
    std::unordered_map<std::string, double> values = reader.get_values(-10.0);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(beta)"]);
    values = reader.get_values(-0.2);
    ASSERT_DOUBLE_EQ(-2, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(-3, values["port side propeller(beta)"]);
    values = reader.get_values(-0.1);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(beta)"]);
    check_positive_linear_spaced_requests(reader);
}

TEST_F(CSVLineByLineReaderTest, empty_file)
{
    empty(csv);
    CSVLineByLineReader reader = get_reader();
    std::unordered_map<std::string, double> values;
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        values = reader.get_values(a.random<double>());
        ASSERT_DOUBLE_EQ(0, values["port side propeller(rpm)"]);
        ASSERT_DOUBLE_EQ(0, values["port side propeller(beta)"]);
    }
}

TEST_F(CSVLineByLineReaderTest, file_with_just_one_line)
{
    single_line(csv);
    CSVLineByLineReader reader = get_reader();
    std::unordered_map<std::string, double> values = reader.get_values(0.005);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(beta)"]);
    values = reader.get_values(-1000);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(beta)"]);
    values = reader.get_values(0);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(0, values["port side propeller(beta)"]);

    values = reader.get_values(3);
    ASSERT_DOUBLE_EQ(65, values["port side propeller(rpm)"]);
    ASSERT_DOUBLE_EQ(78, values["port side propeller(beta)"]);
}

TEST_F(CSVLineByLineReaderTest, non_increasing_dates)
{
    non_decreasing(csv);
    CSVLineByLineReader reader = get_reader();
    ASSERT_THROW(reader.get_values(20), InvalidInputException);
}

TEST_F(CSVLineByLineReaderTest, next_date_should_be_correct)
{
    happy_case(csv);
    CSVLineByLineReader reader = get_reader();
    ASSERT_DOUBLE_EQ(0.2, reader.get_next_date());
    reader.get_values(-20);
    ASSERT_DOUBLE_EQ(0.2, reader.get_next_date());
    reader.get_values(0.2);
    ASSERT_DOUBLE_EQ(0.2001, reader.get_next_date());
    reader.get_values(0.2001);
    ASSERT_DOUBLE_EQ(2.001, reader.get_next_date());
    reader.get_values(2.001);
    ASSERT_DOUBLE_EQ(2.01, reader.get_next_date());
    reader.get_values(2.01);
    ASSERT_DOUBLE_EQ(2.01, reader.get_next_date());
    reader.get_values(201);
    ASSERT_DOUBLE_EQ(2.01, reader.get_next_date());
}

TEST_F(CSVLineByLineReaderTest, initial_date_should_be_correct)
{
    happy_case(csv);
    CSVLineByLineReader reader = get_reader();
    for (int i = 0 ; i < 1000; ++i)
    {
        reader.get_values(a.random<double>());
        ASSERT_DOUBLE_EQ(0.2, reader.get_initial_date());
    }
}
