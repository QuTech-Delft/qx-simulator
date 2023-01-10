#pragma once

#include "qx/Simulator.h"

namespace qx {

class QX {
public:
    QX() : simulator(std::make_shared<qx::Simulator>()) {}

    void set_json_output_path(std::string filename) {
        simulator->setJSONOutputPath(filename);
    }

    bool set(std::string qasm_file_name) {
        return simulator->set(qasm_file_name);
    }

    bool set_string(std::string cqasm_data) {
        return simulator->setString(cqasm_data);
    }

    std::string execute(size_t navg = 1) { return simulator->execute(navg); }

private:
    std::shared_ptr<qx::Simulator> simulator;
};

} // namespace qx