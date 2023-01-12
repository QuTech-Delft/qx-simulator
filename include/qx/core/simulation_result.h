#include "linalg.h"
#include "register.h"
#include <variant>

using State = std::bitset<MAX_QB_N>;

class JsonDict {
public:
    template <typename V>
    void add(std::string key, V value) {
        m.push_back(std::make_pair(key, ValueType(value)));
    }

    std::string to_string(std::size_t level = 0) const {
        std::stringstream ss;

        ss << std::fixed << std::setprecision(6) << "{" << std::endl;
        
        bool first = true;
        for (auto const& kv: m) {
            if (!first) {
                ss << "," << std::endl;
            } else {
                first = false;
            }

            ss << std::string((level + 1) * 4, ' ') << '"' << kv.first << "\": ";
            if (auto const* d = std::get_if<JsonDict>(&kv.second)) {
                ss << d->to_string(level + 1);
            } else if (auto const* d = std::get_if<double>(&kv.second)) {
                ss << *d;
            } else if (auto const* i = std::get_if<std::uint64_t>(&kv.second)) {
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

struct SimulationResult {
    struct Complex {
        double real = 0;
        double imag = 0;
    };
    
    std::uint64_t shots_requested = 0;
    std::uint64_t shots_done = 0;

    std::vector<std::pair<std::string, double>> results;
    std::vector<std::pair<std::string, Complex>> state;
    
    std::string getJsonString() {
        JsonDict info;
        info.add("shots_requested", shots_requested);
        info.add("shots_done", shots_done);

        JsonDict resultsJson;
        for (auto const& kv: results) {
            resultsJson.add(kv.first, kv.second);
        }

        JsonDict stateJson;
        for (auto const& kv: state) {
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
};

class SimulationResultAccumulator {
public:
    SimulationResultAccumulator(qx::qu_register &reg) : reg(reg) {};

    void append(State measured_state) {
        measured_states[measured_state]++;
        n_measurements++;
    }

    void dump() {
        std::setprecision(6);
        std::cout << std::setprecision(6) << std::fixed;
        println("-------------------------------------------");

        println("Final quantum state");

        forAllNonZeroStates([](auto stateString, auto c) {
            println(stateString, "       ", c, " (", c.norm(), ")");
        });
        println();

        println("Measurement register averaging");

        for (const auto &kv: measured_states) {
            const auto& state = kv.first;
            const auto& count = kv.second;
            println(get_state_string(state), "       ", count, "/", n_measurements, " (", static_cast<double>(count) / n_measurements, ")");
        }
    }

    SimulationResult get() {
        SimulationResult result;
        result.shots_requested = n_measurements;
        result.shots_done = n_measurements;

        if (n_measurements == 1) {
            // When doing a single shot, it is assumed that one is more interested in norms of the complex amplitudes.

            forAllNonZeroStates([&result](auto stateString, auto c) {
                result.results.push_back(std::make_pair(stateString, c.norm()));
            });
        } else {
            assert(n_measurements > 1);

            for (const auto &kv: measured_states) {
                const auto& state = kv.first;
                const auto& count = kv.second;

                result.results.push_back(std::make_pair(get_state_string(state), static_cast<double>(count) / n_measurements));
            }
        }
        
        forAllNonZeroStates([&result](auto stateString, auto c) {
            result.state.push_back(std::make_pair(stateString, SimulationResult::Complex{ .real = c.re, .imag = c.im }));
        });

        return result;
    }

private:
    struct compare_states {
        bool operator()(const State& a, const State& b) const {
            return a.to_ulong() < b.to_ulong();
        }
    };

    template <typename Op>
    void forAllNonZeroStates(Op op) {
        State state;
        auto s_number = state.to_ulong();
        auto& data = reg.get_data();
        while (s_number < (1 << reg.size())) {
            static constexpr double epsilon = 10e-10;
            auto probability = data[s_number].norm();
            if (probability > epsilon) {
                op(get_state_string(state), data[s_number]);
            }

            state = inc(state);
            s_number = state.to_ulong();
        }
    }

    std::string get_state_string(State s) {
        auto str = s.to_string();

        return str.substr(str.size() - reg.size(), str.size());
    }

    qx::qu_register &reg;
    std::map<State, std::uint64_t, compare_states> measured_states;
    std::uint64_t n_measurements = 0;
};
