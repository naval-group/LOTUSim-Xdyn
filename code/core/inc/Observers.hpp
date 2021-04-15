/*
 * Observers.hpp
 *
 * \date 21 mars 2014
 *      Author: cec
 */

#ifndef OBSERVERS_HPP_
#define OBSERVERS_HPP_

#include <sstream>
#include <vector>

#include "ContinuousSystem.hpp"

namespace ssc
{
    namespace solver
    {
        template <typename IteratorType, typename Stream> void serialize(Stream& os, const IteratorType& begin, const IteratorType& end)
        {
            if (end-begin>0)
            {
                IteratorType it = begin;
                os << *it;++it;
                for (;it!=end;++it)
                {
                    os << "," << *it;
                }
                os << "\n";
            }
        }

        class NullObserver
        {
            public:
                NullObserver() {}
                void observe(const ContinuousSystem& , const double )
                {

                }
        };

        class DefaultObserver
        {
            public:
                DefaultObserver(std::ostream& os_) : os(os_), initialized(false) {}
                void observe(const ContinuousSystem& sys, const double t)
                {
                    const std::vector<double> x = sys.state;
                    if (not(initialized))
                    {
                        serialize_title(x.size());
                        initialized = true;
                    }
                    std::vector<double> v;
                    v.push_back(t);
                    v.insert(v.end(), x.begin(), x.end());
                    serialize(os, v.begin(), v.end());
                }

            private:

                void serialize_title(const size_t nb_of_states)
                {
                    std::vector<std::string> v;
                    v.push_back("t");
                    for (size_t i = 0 ; i < nb_of_states ; ++i)
                    {
                        std::stringstream ss;
                        ss << "x" << i;
                        v.push_back(ss.str());
                    }
                    serialize(os, v.begin(), v.end());
                }

                std::ostream& os;
                bool initialized;
        };

        class VectorObserver
        {
            public:
                VectorObserver() : v(std::vector<std::pair<double,double> >()) {}
                void observe(const ContinuousSystem& sys, const double t)
                {
                    v.push_back(std::make_pair(t, sys.state[0]));
                }

                std::vector<std::pair<double,double> > get() const
                {
                    return v;
                }

            private:
                std::vector<std::pair<double,double> > v;
        };
    }
}

#endif /* OBSERVERS_HPP_ */
