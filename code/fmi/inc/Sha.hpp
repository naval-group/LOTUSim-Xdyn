#ifndef CALCULATE_SHA1_HPP_
#define CALCULATE_SHA1_HPP_

#include "YamlRotation.hpp"
#include "YamlEnvironmentalConstants.hpp"
#include "YamlModel.hpp"
#include "YamlBody.hpp"
#include "YamlDynamics.hpp"
#include "YamlDynamics6x6Matrix.hpp"
#include "YamlPoint.hpp"
#include "YamlCoordinates.hpp"
#include "YamlSimulatorInput.hpp"

#include <boost/uuid/sha1.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

/**
 * \brief Evaluates SHA1 of bytes, using boost::uuids::detail::sha1
 *
 * \note
 *
 * Calling boost::uuids::detail::sha1.get_digest() twice does not return the same
 * result. Member up_to_dates is here to keep track of the calls to
 * the 'get_digest' and 'append' methods
 * The specific 'get_digest' prototype is also required to use the hash member
 */
class Sha1
{
    public:
        virtual ~Sha1(){}
        Sha1() : sha1(), up_to_date(false), hash{0,0,0,0,0}
        {}

        template<typename T> void append(const std::vector<T>&v)
        {
            for (size_t i=0;i<v.size();++i)
            {
                append(v[i]);
            }
        }

        template<typename T> void append(const T& v)
        {
            sha1.process_bytes(&v, sizeof(T));
            up_to_date=false;
        }

        void update()
        {
            sha1.get_digest(hash);
            up_to_date=true;
        }

        std::string get()
        {
            std::stringstream ss;
            ss << *this;
            return ss.str();
        }

        friend std::ostream & operator<<(std::ostream& os, Sha1& sha1)
        {
            unsigned int hash[5]={0};
            sha1.get(hash);
            return os << std::hex << std::setfill('0')
                      << std::setw(8) << std::setprecision(8) << hash[0]<< hash[1]<< hash[2]<< hash[3]<< hash[4];
        }

    protected:
        boost::uuids::detail::sha1 sha1;
        bool up_to_date;
        unsigned int hash[5];

    private:
        void get(unsigned int sha1_hash[5])
        {
            if (not up_to_date) update();
            for (size_t i=0;i<5;++i)
            {
                sha1_hash[i]=hash[i];
            }
        }
};

template <> void Sha1::append(const std::string&s);

void sha1_append(Sha1& sha1, const YamlRotation& yaml);
void sha1_append(Sha1& sha1, const YamlEnvironmentalConstants& yaml);
void sha1_append(Sha1& sha1, const YamlModel& yaml);
void sha1_append(Sha1& sha1, const YamlBody& yaml);
void sha1_append(Sha1& sha1, const YamlDynamics& yaml);
void sha1_append(Sha1& sha1, const YamlDynamics6x6Matrix& yaml);
void sha1_append(Sha1& sha1, const YamlPoint& yaml);
void sha1_append(Sha1& sha1, const YamlPosition& yaml);
void sha1_append(Sha1& sha1, const YamlCoordinates& yaml);
void sha1_append(Sha1& sha1, const YamlSimulatorInput& yaml);
void sha1_append(Sha1& sha1, const YamlAngle& yaml);
void sha1_append(Sha1& sha1, const YamlSpeed& yaml);

class Sha: public Sha1
{
    public:
        Sha(const YamlSimulatorInput& i){append(i);}
        Sha(const std::string& s, const YamlSimulatorInput& i){Sha1::append(s);append(i);}
        void append(const YamlSimulatorInput& yaml){sha1_append(*this,yaml);up_to_date=false;}
    private:
        Sha():Sha1(){}
};

#endif
