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

/**
 * \brief Evaluates SHA1 of bytes, using boost::uuids::detail::sha1
 *
 * \note
 *
 * Calling boost::uuids::detail::sha1.get_digest() twice does not return the same
 * result. Member isUpToDate is here to keep track of the calls to
 * the 'get_digest' and 'append' methods
 * The specific 'get_digest' prototype is also required to use the hash member
 */
class Sha1
{
    public:
        virtual ~Sha1(){}
        Sha1():sha1(),isUpToDate(false),hash{0,0,0,0,0}{}
        template<typename T> Sha1(const std::vector<T>&v):sha1(),isUpToDate(false),hash{0,0,0,0,0}{append(v);}
        template<typename T> Sha1(const T&v):sha1(),isUpToDate(false),hash{0,0,0,0,0}{append(v);}
        Sha1(const std::vector<std::string>&v):sha1(),isUpToDate(false),hash{0,0,0,0,0}{append(v);}
        Sha1(const std::string&s):sha1(),isUpToDate(false),hash{0,0,0,0,0}{append(s);}

        template<typename T> void append(const std::vector<T>&v) {sha1.process_bytes(&v[0], v.size() * sizeof(T));isUpToDate=false;}
        template<typename T> void append(const T& v) {sha1.process_bytes(&v, sizeof(T));isUpToDate=false;}
        void append(const std::vector<std::string>&v) {for(size_t i=0;i<v.size();++i)sha1.process_bytes(v[i].c_str(), v[i].size());isUpToDate=false;}
        void append(const std::string&s){sha1.process_bytes(s.c_str(), s.size());isUpToDate=false;}

        void update(){sha1.get_digest(hash);isUpToDate=true;}

        void get(unsigned int sha1_hash[5]){if(not isUpToDate){update();} for(size_t i=0;i<5;++i) sha1_hash[i]=hash[i];}

        friend std::ostream & operator<<(std::ostream& os, Sha1& sha1)
        {
            unsigned int hash[5]={0};
            sha1.get(hash);
            return os << hash[0]<< hash[1]<< hash[2]<< hash[3]<< hash[4];
        }
    protected:
        boost::uuids::detail::sha1 sha1;
        bool isUpToDate;
        unsigned int hash[5];
};

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

class YamlSha1: public Sha1
{
    public:
        YamlSha1(const YamlSimulatorInput& i){append(i);}
        YamlSha1(const std::string& s, const YamlSimulatorInput& i){Sha1::append(s);append(i);}
        void append(const YamlSimulatorInput& yaml){sha1_append(*this,yaml);isUpToDate=false;}
    private:
        YamlSha1():Sha1(){}
};

#endif
