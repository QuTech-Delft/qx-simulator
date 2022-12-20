#include "qx/simulator.h"
#include <iostream>

int main() {
    qx::simulator sim;
    sim.set("basic.qasm");

    for (int i = 0; i < 10000; i++) {
        sim.execute(0);
        auto c0 = sim.get_measurement_outcome(0);
        auto c1 = sim.get_measurement_outcome(1);
        std::cout << c0 << " " << c1 << std::endl;
    }

    std::cout << "quantum state:" << std::endl;
    std::cout << sim.get_state() << std::endl;

    return 0;
}
