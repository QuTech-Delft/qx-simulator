#include <iostream>
#include "qx/simulator.h"

int main() {

    qx::simulator sim;
    sim.set("basic.qasm");
    for (int i = 0; i < 100; i++) {
        sim.execute(0);
        auto c0 = sim.move(0);
        auto c1 = sim.move(1);
        std::cout << c0 << " " << c1 << std::endl;
    }

    std::cout << "quantum state:" << std::endl;
    std::cout << sim.get_state() << std::endl;

    return 0;
}
