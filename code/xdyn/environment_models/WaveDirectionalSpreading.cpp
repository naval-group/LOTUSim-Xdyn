/*
 * WaveDirectionalSpreading.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */
#include "WaveDirectionalSpreading.hpp"
#include "SumOfWaveDirectionalSpreadings.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI
#include <algorithm>    // std::sort

WaveDirectionalSpreading::WaveDirectionalSpreading() : psi0(0)
{
}

WaveDirectionalSpreading::WaveDirectionalSpreading(const double psi0_) : psi0(psi0_)
{
}

WaveDirectionalSpreading::~WaveDirectionalSpreading()
{
}

std::vector<std::pair<int,int>> WaveDirectionalSpreading::build_coprimes(const size_t n) const
{
    std::vector<std::pair<int,int>> coprimes;
    // https://en.wikipedia.org/wiki/Coprime_integers#Generating_all_coprime_pairs
    std::pair<int,int> current_coprime;
    int even_index = 0;
    int odd_index = 0;
    std::vector<std::pair<int,int>> even_coprimes = {{2,1}};
    std::vector<std::pair<int,int>> odd_coprimes = {{3,1}};
    while (coprimes.size() < (unsigned int)n)
    {
        bool even = (coprimes.size() % 2 == 0);
        if (even) 
        {
            current_coprime = even_coprimes.at(even_index);
            even_index += 1;
        }
        else 
        {
            current_coprime = odd_coprimes.at(odd_index);
            odd_index += 1;
        }
        int m = current_coprime.first;
        int n = current_coprime.second;
        if (even)
        {
            even_coprimes.push_back(std::make_pair(2*m-n,m));
            even_coprimes.push_back(std::make_pair(m+2*n,n));
            even_coprimes.push_back(std::make_pair(2*m+n,m));
        }
        else
        {
            odd_coprimes.push_back(std::make_pair(2*m-n,m));
            odd_coprimes.push_back(std::make_pair(m+2*n,n));
            odd_coprimes.push_back(std::make_pair(2*m+n,m));
        }
        int i = 0;
        while (coprimes.size() < (unsigned int)n && i < 8)
        {
            coprimes.push_back(current_coprime);
        }
    }
    return coprimes;
}

std::vector<double> WaveDirectionalSpreading::get_directions(const size_t n_angles, //!< Number of angles to return
                                                             const bool periodic    //!< Space periodic waves or not
                                                             ) const
{
    if (!periodic)
    {
        std::vector<double> psi(n_angles, 0);
        for (size_t i = 0 ; i < n_angles ; ++i)
        {
            psi[i] = 2*PI*double(i)/double(n_angles);
        }
        return psi;
    }
    else
    {
        std::vector<std::pair<int,int>> coprimes = build_coprimes(n_angles);
        std::vector<double> psi {0, PI/4, PI/2, 3*PI/4, PI, 5*PI/4, 3*PI/2, 7*PI/4};
        int index = 0;
        std::pair<int,int> current_coprime;
        while (psi.size() < n_angles)
        {
            current_coprime = coprimes.at(8*index);
            int m = current_coprime.first;
            int n = current_coprime.second;
            std::vector<double> new_range {std::acos(n/sqrt(m*m+n*n)), std::acos(m/sqrt(m*m+n*n)), 
                                           std::acos(-n/sqrt(m*m+n*n)), std::acos(-m/sqrt(m*m+n*n)),
                                           2*PI-std::acos(-m/sqrt(m*m+n*n)), 2*PI-std::acos(-n/sqrt(m*m+n*n)),
                                           2*PI-std::acos(m/sqrt(m*m+n*n)), 2*PI-std::acos(n/sqrt(m*m+n*n))};
            int j = 0;
            while(psi.size() < n_angles && j < 8)
            {
                psi.push_back(new_range[j]);
                j += 1;
            }
            index += 1;
        }
        std::sort(psi.begin(), psi.end());
        return psi;
    }
}

SumOfWaveDirectionalSpreadings WaveDirectionalSpreading::operator+(const WaveDirectionalSpreading& w) const
{
    return SumOfWaveDirectionalSpreadings(*this, w);
}
