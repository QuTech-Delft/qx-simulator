#include "qx/SimulationResult.hpp"

#include "qx/Core.hpp"
#include <iomanip>
#include <iostream>
#include <variant>


namespace qx {

namespace {
class JsonDict {
public:
    template <typename V> void add(std::string key, V value) {
        m.emplace_back(std::move(key), ValueType(value));
    }

    [[nodiscard]] std::string to_string(std::size_t level = 0) const {
        std::stringstream ss;

        ss << std::fixed << std::setprecision(config::OUTPUT_DECIMALS) << "{"
           << std::endl;

        bool first = true;
        for (auto const &kv : m) {
            if (!first) {
                ss << "," << std::endl;
            } else {
                first = false;
            }

            ss << std::string((level + 1) * 4, ' ') << '"' << kv.first
               << "\": ";
            if (auto const *jd = std::get_if<JsonDict>(&kv.second)) {
                ss << jd->to_string(level + 1);
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
        complex.add("norm", kv.second.norm);

        stateJson.add(kv.first, complex);
    }

    JsonDict totalOutput;
    totalOutput.add("info", info);
    totalOutput.add("results", resultsJson);
    totalOutput.add("state", stateJson);

    return totalOutput.to_string();
}

void SimulationResultAccumulator::append(BasisVector measuredState) {
    assert(measuredStates.size() <= (1u << quantumState.getNumberOfQubits()));
    measuredStates[measuredState]++;
    nMeasurements++;
}

std::ostream &operator<<(std::ostream &os, SimulationResult const &r) {
    os << std::setprecision(config::OUTPUT_DECIMALS) << std::fixed;
    os << "-------------------------------------------" << std::endl;

    os << "Final quantum state" << std::endl;

    for (auto const &kv : r.state) {
        auto const &stateString = kv.first;
        auto const &amplitude = kv.second;
        os << stateString << "       " << amplitude.real << " + "
           << amplitude.imag << "*i   "
           << " (p = " << amplitude.norm << ")" << std::endl;
    }

    os << std::endl << "Measurement register averaging" << std::endl;

    for (const auto &kv : r.results) {
        const auto &stateString = kv.first;
        const auto &count = kv.second;
        os << stateString << "       " << count << "/" << r.shots_done << " ("
           << static_cast<double>(count) / static_cast<double>(r.shots_done) << ")" << std::endl;
    }
    return os;
}

SimulationResult SimulationResultAccumulator::get() {
    SimulationResult simulationResult;
    simulationResult.shots_requested = nMeasurements;
    simulationResult.shots_done = nMeasurements;

    assert(nMeasurements > 0);

    for (const auto &kv : measuredStates) {
        const auto &state = kv.first;
        const auto &count = kv.second;

        simulationResult.results.emplace_back(getStateString(state), count);
    }

    forAllNonZeroStates([&simulationResult](auto stateString, auto c) {
        simulationResult.state.push_back(std::make_pair(
            stateString, SimulationResult::Complex{.real = c.real(),
                                                   .imag = c.imag(),
                                                   .norm = std::norm(c)}));
    });

    return simulationResult;
}

template <typename F>
void SimulationResultAccumulator::forAllNonZeroStates(F &&f) {
    quantumState.forEach(
        [&f, this](auto const &kv) { f(getStateString(kv.first), kv.second); });
}

std::string SimulationResultAccumulator::getStateString(BasisVector s) {
    auto str = s.toString();

    return str.substr(str.size() - quantumState.getNumberOfQubits(),
                      str.size());
}

} // namespace qx