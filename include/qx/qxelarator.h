/**
 * @file   qxelarator.h
 * @author Imran Ashraf
 * @brief  header file for qxelarator python interface
 */

#ifndef QXELARATOR_H
#define QXELARATOR_H

#include "qx/simulator.h"
#include <iostream>

class QX {
public:
    std::shared_ptr<qx::simulator> qx_sim;

    QX() {
        try {
            qx_sim = std::make_shared<qx::simulator>();
        } catch (std::exception &e) {
            std::cerr << "error creating qx::simulator " << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }

    void set(std::string qasm_file_name) { qx_sim->set(qasm_file_name); }

    void execute(size_t navg = 0) { qx_sim->execute(navg); }

    bool get_measurement_outcome(size_t q) { return qx_sim->move(q); }
    
    std::string get_state() { return qx_sim->get_state(); }
};

#endif
