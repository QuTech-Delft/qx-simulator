/**
 * @file		gaussian.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com
 * @date		06-10-15
 * @brief
 */

#pragma once

#include <iostream>
#include <random>

/**
 * \brief random number generator
 *
 *   random number distribution that produces floating-point
 *   values according to a normal distribution, which is
 *   described by the following probability density function:
 *
 *   p(x|u,sigma) = [ 1/(sigma*sqrt(2*PI) ] . exp( (x-u)^2 / 2.sigma^2)
 *
 *   this distribution produces random numbers around the distribution
 *   mean "u" with a specific standard deviation "sigma"
 */
namespace qx {
namespace gaussian {
class random {
public:
    /**
     * \brief ctor
     */
    inline random(double mean = 0.0f, double deviation = 1.0f);

    /**
     * next
     */
    inline double next();

    /**
     * mean setter
     */
    inline void set_mean(double mean);

    /**
     * deviation setter
     */
    inline void set_std_deviation(double deviation);

private:
    double mean;
    double deviation;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;
};

} // namespace gaussian
} // namespace qx
