#include "linalg.h"
#include "register.h"
#include <fstream>

using State = std::bitset<MAX_QB_N>;

class SimulationResult {
public:
    SimulationResult(std::size_t nQubits) : nQubits(nQubits) {};

    void dump() {
        std::setprecision(9);
        println("-------------------------------------------");
        println(get_title());
        auto per_state = get_result_per_state();
        for (const auto &kv: per_state) {
            const auto& state = kv.first;
            const auto& value = kv.second;

            println(get_state_string(state), "   ", value);
        }
    }

    void dump_json(const std::string &filename) {
        std::ofstream outfile(filename);

        // No need for a JSON library for such a small output.
        outfile << "{\n";
        outfile << "  \"info\": {\n";
        outfile << "    \"shots_requested\": " << get_shots_requested() << ",\n";
        outfile << "    \"shots_measured\": " << get_shots_measured() << "\n";
        outfile << "  },\n";
        outfile << "  \"results\": {\n";

        auto per_state = get_result_per_state();
        bool first = true;
        for (const auto &kv: per_state) {
            const auto& state = kv.first;
            const auto& value = kv.second;

            if (!first) {
                outfile << ",\n";
            }

            first = false;

            outfile << "    \"" << get_state_string(state) << "\": " << value;
        }
        outfile << "\n  }\n";
        outfile << "}\n";
    };

protected:
    struct compare_states {
        bool operator()(const State& a, const State& b) const {
            return a.to_ulong() < b.to_ulong();
        }
    };

    virtual std::string get_title() = 0;

    virtual std::map<State, double, compare_states> get_result_per_state() = 0;

    virtual std::uint64_t get_shots_requested() = 0;

    virtual std::uint64_t get_shots_measured() = 0;

    std::size_t nQubits = 0;

private:
    std::string get_state_string(State s) {
        auto str = s.to_string();

        std::reverse(str.begin(), str.end());
        return str.substr(0, nQubits);
    }

};

class MeasurementAveraging : public SimulationResult {
public:
    MeasurementAveraging(std::size_t nQubits) : SimulationResult(nQubits) {};

    void append(State measured_state) {
        measured_states[measured_state]++;
        n_measurements++;
    }

protected:
    std::string get_title() override {
        return "Measurement averaging";
    }

    std::uint64_t get_shots_requested() override {
        return n_measurements;
    };

    std::uint64_t get_shots_measured() override {
        return n_measurements;
    };

private:
    std::map<State, double, compare_states> get_result_per_state() override {
        std::map<State, double, compare_states> result;

        for (const auto &kv: measured_states) {
            const auto& state = kv.first;
            const auto& count = kv.second;

            assert(n_measurements > 0);
            double av = static_cast<double>(count) / n_measurements;

            result[state] = av;
        }

        return result;
    }
    
    std::map<State, std::uint64_t, compare_states> measured_states;
    std::uint64_t n_measurements = 0;
};

class ExactQuantumState : public SimulationResult {
public:
    ExactQuantumState(qx::qu_register &reg) : SimulationResult(reg.size()), reg(reg) {}

    std::string get_title() override {
        return "Expected measurement probabilities";
    }

protected:
    std::map<State, double, compare_states> get_result_per_state() override {
        std::map<State, double, compare_states> result;

        State s;
        auto s_number = s.to_ulong();
        auto& data = reg.get_data();
        while (s_number < (1 << nQubits)) {
            auto prob = data[s_number].norm();

            static constexpr double epsilon = 10e-10;
            if (prob > epsilon) {
                result[s] = prob;
            }

            s = inc(s);
            s_number = s.to_ulong();
        }
        
        return result;
    }

    std::uint64_t get_shots_requested() override {
        return 1;
    };

    std::uint64_t get_shots_measured() override {
        // Since we provide the exact quantum state without measurement, this is 0.
        return 0;
    };

private:
    qx::qu_register &reg;
};