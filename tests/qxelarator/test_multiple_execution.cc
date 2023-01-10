#include "qx/Simulator.h"
#include <iostream>

int main() {

    qx::Simulator sim;
    sim.set("basic.qasm");
    for (int i = 0; i < 100; i++) {
        sim.execute();
        // FIXME: do something
    }

    return 0;
}
