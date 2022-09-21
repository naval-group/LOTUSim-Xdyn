/*
 * CsvObserver.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "CsvObserver.hpp"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>


CsvObserver::CsvObserver(const std::string& filename) :
        Observer(),
        flush_during_write(true),
        output_to_file(not(filename.empty())),
        os(output_to_file ? *(new std::ofstream(filename)) : std::cout),
        cache()
{
    os << std::scientific;
}

CsvObserver::CsvObserver(const std::string& filename, const std::vector<std::string>& d) :
        Observer(d),
        flush_during_write(true),
        output_to_file(not(filename.empty())),
        os(output_to_file ? *(new std::ofstream(filename)) : std::cout),
        cache()
{
    os << std::scientific;
}

CsvObserver::~CsvObserver()
{
    if (output_to_file) delete(&os);
}

std::function<void()> CsvObserver::get_serializer(const double val, const DataAddressing& d)
{
    return [this,val,d](){cache[d.name] = val;};
}

std::function<void()> CsvObserver::get_initializer(const double, const DataAddressing& address)
{
    return [this,address](){std::string title = address.name;boost::replace_all(title, ",", " ");os << title;};
}

void CsvObserver::flush_after_initialization()
{
    os << std::endl;
    flush_during_write = false;
}

void CsvObserver::flush_after_write()
{
    const size_t n = requested_serializations.size();
    if (!n) return;
    for (size_t i = 0 ; i < n-1 ; ++i)
    {
        os << cache[requested_serializations[i]] << ",";
    }
    os << cache[requested_serializations.back()];
    os << std::endl;
}

void CsvObserver::flush_value_during_write()
{
    if (flush_during_write) os << ',';
}
