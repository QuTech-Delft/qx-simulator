#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "qx/simulator.h"
#include "doctest/doctest.h"

namespace qx {


TEST_CASE("Run rand.qasm") {
    qx::simulator sim;
    sim.set("rand.qasm");

    sim.execute(100);
    auto res = sim.get_measurement_outcome(0);

    (void) res;
}

}