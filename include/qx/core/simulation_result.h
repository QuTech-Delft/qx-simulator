#include "linalg.h"
#include "register.h"
#include <fstream>

using State = std::bitset<MAX_QB_N>;

template <typename ResultT>
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

            println(get_state_string(state), "       ", value.represent_for_output());
        }
    }

    void dump_json(const std::string &filename) {
        std::ofstream outfile(filename);

        // No need for a JSON library for such a small output.
        outfile << "{\n";
        outfile << "  \"info\": {\n";
        outfile << "    \"shots_requested\": " << get_shots_requested() << ",\n";
        outfile << "    \"shots_done\": " << get_shots_done() << "\n";
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

            outfile << "    \"" << get_state_string(state) << "\": " << value.json();
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

    virtual std::map<State, ResultT, compare_states> get_result_per_state() = 0;

    virtual std::uint64_t get_shots_requested() = 0;

    virtual std::uint64_t get_shots_done() = 0;

    std::size_t nQubits = 0;

private:
    std::string get_state_string(State s) {
        auto str = s.to_string();

        return str.substr(str.size() - nQubits, str.size());
    }

};

class Fraction {
public:
    Fraction() = default;

    Fraction(std::uint64_t aNumerator, std::uint64_t aDenominator) : numerator(aNumerator), denominator(aDenominator) {}

    std::string represent_for_output() const {
        return std::to_string(numerator) + "/" + std::to_string(denominator) + " (" + std::to_string(numerator / static_cast<double>(denominator)) + ")";
    }

    std::string json() const {
        return std::to_string(numerator / static_cast<double>(denominator));
    }

private:
    std::uint64_t numerator = 0;
    std::uint64_t denominator = 0;
};

class MeasurementAveraging : public SimulationResult<Fraction> {
public:
    MeasurementAveraging(std::size_t nQubits) : SimulationResult(nQubits) {};

    void append(State measured_state) {
        measured_states[measured_state]++;
        n_measurements++;
    }

protected:
    std::string get_title() override {
        return "Measurement register averaging";
    }

    std::uint64_t get_shots_requested() override {
        return n_measurements;
    };

    std::uint64_t get_shots_done() override {
        return n_measurements;
    };

private:
    std::map<State, Fraction, compare_states> get_result_per_state() override {
        std::map<State, Fraction, compare_states> result;

        for (const auto &kv: measured_states) {
            const auto& state = kv.first;
            const auto& count = kv.second;

            assert(n_measurements > 0);

            result[state] = Fraction(count, n_measurements);
        }

        return result;
    }
    
    std::map<State, std::uint64_t, compare_states> measured_states;
    std::uint64_t n_measurements = 0;
};

class ComplexWithProba {
public:
    ComplexWithProba() = default;

    ComplexWithProba(complex_t aComplex) : complex(aComplex) {}

    std::string represent_for_output() const {
        std::stringstream ss;
        ss << complex;
        return ss.str() + " (" + std::to_string(complex.norm()) + ")";
    }

    std::string json() const {
        return std::to_string(complex.norm());    }

    double norm() const {
        return complex.norm();
    }

private:
    complex_t complex{};
};

class ExactQuantumState : public SimulationResult<ComplexWithProba> {
public:
    ExactQuantumState(qx::qu_register &reg) : SimulationResult(reg.size()), reg(reg) {}

    std::string get_title() override {
        return "Complex amplitudes with probabilities";
    }

protected:
    std::map<State, ComplexWithProba, compare_states> get_result_per_state() override {
        std::map<State, ComplexWithProba, compare_states> result;

        State s;
        auto s_number = s.to_ulong();
        auto& data = reg.get_data();
        while (s_number < (1 << nQubits)) {
            ComplexWithProba c(data[s_number]);

            static constexpr double epsilon = 10e-10;
            if (c.norm() > epsilon) {
                result[s] = c;
            }

            s = inc(s);
            s_number = s.to_ulong();
        }
        
        return result;
    }

    std::uint64_t get_shots_requested() override {
        return 1;
    };

    std::uint64_t get_shots_done() override {
        return 1;
    };

private:
    qx::qu_register &reg;
};