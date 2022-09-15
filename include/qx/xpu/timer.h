/**
 * @file    timer.h
 * @author  Jeroen van Straten
 * @date    20-05-2021
 * @brief   Cross-platform C++11 replacement for xpu/timer
 */

#pragma once

#include <chrono>

namespace xpu {

class timer {
private:
    /**
     * The clock source to use.
     */
    using Clock = std::chrono::high_resolution_clock;

    /**
     * Time points for the clock source.
     */
    using TimePoint = Clock::time_point;

    /**
     * Start time.
     */
    TimePoint start_time;

    /**
     * Stop time.
     */
    TimePoint stop_time;

    /**
     * Elapsed time in seconds.
     */
    double elapsed_time = 0.0;

public:
    /**
     * Starts the timer, clearing the current elapsed time.
     */
    inline void start() {
        start_time = Clock::now();
        elapsed_time = 0.0;
    }

    /**
     * Stops the timer, setting the elapsed time to the time since start() was
     * called.
     */
    inline void stop() {
        stop_time = Clock::now();
        elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                           stop_time - start_time)
                           .count() *
                       1e-9;
    }

    /**
     * Returns the elapsed time.
     */
    inline double elapsed() { return elapsed_time; }

    /**
     * Returns the current time in seconds.
     */
    inline double current() {
        auto now = std::chrono::system_clock::now().time_since_epoch();
        auto nanos =
            std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
        return (double)nanos * 1e-9;
    }
};

} // namespace xpu
