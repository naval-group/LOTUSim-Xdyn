/*
 * CSVObserver.hpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#ifndef CSVOBSERVER_HPP_
#define CSVOBSERVER_HPP_

#include <ostream>
#include <unordered_map>

#include "xdyn/core/Observer.hpp"

class CsvObserver : public Observer
{
    public:
        CsvObserver(const std::string& filename);
        CsvObserver(const std::string& filename, const std::vector<std::string>& data);
        ~CsvObserver();

    private:
        void flush_after_initialization() override;
        void flush_after_write() override;
        void flush_value_during_write() override;

        bool flush_during_write;
        bool output_to_file;
        std::ostream& os;
        std::unordered_map<std::string, double> cache; //!< As observations are made out-of-order, cache them & use requested_serializations to put them back in order

        using Observer::get_serializer;
        using Observer::get_initializer;

        std::function<void()> get_serializer(const double val, const DataAddressing& address) override;
        std::function<void()> get_initializer(const double val, const DataAddressing& address) override;
};

#endif /* CSVOBSERVER_HPP_ */
