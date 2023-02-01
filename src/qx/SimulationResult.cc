#include "qx/SimulationResult.h"

#include "qx/Core.h"
#include <iomanip>
#include <iostream>

namespace qx {

namespace {
class JsonDict {
public:
    template <typename V> void add(std::string key, V value) {
        m.push_back(std::make_pair(key, ValueType(value)));
    }

    std::string to_string(std::size_t level = 0) const {
        std::stringstream ss;

        ss << std::fixed << std::setprecision(6) << "{" << std::endl;

        bool first = true;
        for (auto const &kv : m) {
            if (!first) {
                ss << "," << std::endl;
            } else {
                first = false;
            }

            ss << std::string((level + 1) * 4, ' ') << '"' << kv.first
               << "\": ";
            if (auto const *d = std::get_if<JsonDict>(&kv.second)) {
                ss << d->to_string(level + 1);
            } else if (auto const *d = std::get_if<double>(&kv.second)) {
                ss << *d;
            } else if (auto const *i = std::get_if<std::uint64_t>(&kv.second)) {
                ss << *i;
            }
        }

        ss << std::endl << std::string(level * 4, ' ') << "}";

        return ss.str();
    }

private:
    using ValueType = std::variant<JsonDict, double, std::uint64_t>;
    std::vector<std::pair<std::string, ValueType>> m;
};
} // namespace

std::string SimulationResult::getJsonString() {
    JsonDict info;
    info.add("shots_requested", shots_requested);
    info.add("shots_done", shots_done);

    JsonDict resultsJson;
    for (auto const &kv : results) {
        resultsJson.add(kv.first, kv.second);
    }

    JsonDict stateJson;
    for (auto const &kv : state) {
        JsonDict complex;
        complex.add("real", kv.second.real);
        complex.add("imag", kv.second.imag);

        stateJson.add(kv.first, complex);
    }

    JsonDict totalOutput;
    totalOutput.add("info", info);
    totalOutput.add("results", resultsJson);
    totalOutput.add("state", stateJson);

    return totalOutput.to_string();
}

void SimulationResultAccumulator::append(BasisVector measuredState) {
    assert(measuredStates.size() <= (1 << quantumState.getNumberOfQubits()));
    measuredStates[measuredState]++;
    nMeasurements++;
}

void SimulationResultAccumulator::dump() {
    std::cout << std::setprecision(6) << std::fixed;
    std::cout << "-------------------------------------------" << std::endl;

    std::cout << "Final quantum state" << std::endl;

    forAllNonZeroStates([](auto const &stateString, auto c) {
        std::cout << stateString << "       " << c.real() << " + " << c.imag()
                  << "*i   "
                  << " (p = " << std::norm(c) << ")" << std::endl;
    });

    std::cout << std::endl << "Measurement register averaging" << std::endl;

    for (const auto &kv : measuredStates) {
        const auto &state = kv.first;
        const auto &count = kv.second;
        std::cout << getStateString(state) << "       " << count << "/"
                  << nMeasurements << " ("
                  << static_cast<double>(count) / nMeasurements << ")"
                  << std::endl;
    }
}

SimulationResult SimulationResultAccumulator::get() {
    SimulationResult result;
    result.shots_requested = nMeasurements;
    result.shots_done = nMeasurements;

    if (nMeasurements == 1) {
        // When doing a single shot, it is assumed that one is more interested
        // in norms of the complex amplitudes.

        forAllNonZeroStates([&result](auto stateString, auto c) {
            result.results.push_back(std::make_pair(stateString, std::norm(c)));
        });
    } else {
        assert(nMeasurements > 1);

        for (const auto &kv : measuredStates) {
            const auto &state = kv.first;
            const auto &count = kv.second;

            result.results.push_back(
                std::make_pair(getStateString(state),
                               static_cast<double>(count) / nMeasurements));
        }
    }

    forAllNonZeroStates([&result](auto stateString, auto c) {
        result.state.push_back(std::make_pair(
            stateString,
            SimulationResult::Complex{.real = c.real(), .imag = c.imag()}));
    });

    return result;
}

template <typename F>
void SimulationResultAccumulator::forAllNonZeroStates(F &&f) {
    quantumState.forEach([&f, this](auto const &kv) {
        f(getStateString(kv.first), kv.second);
    });
}

std::string SimulationResultAccumulator::getStateString(BasisVector s) {
    auto str = s.toString();

    return str.substr(str.size() - quantumState.getNumberOfQubits(),
                      str.size());
}

} // namespace qx