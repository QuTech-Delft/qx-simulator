#include "qx/simulator.h"
#include <iostream>

int main() {

    qx::simulator sim;
    sim.set("basic.qasm");
    // FIXME: should be way more iterations, but this causes CI problems
    for (int i = 0; i < 1; i++) {
        sim.execute(0);
        auto c0 = sim.get_measurement_outcome(0);
        auto c1 = sim.get_measurement_outcome(1);
        std::cout << c0 << " " << c1 << std::endl;
    }

    std::cout << "quantum state:" << std::endl;
    std::cout << sim.get_state() << std::endl;

    return 0;
}
