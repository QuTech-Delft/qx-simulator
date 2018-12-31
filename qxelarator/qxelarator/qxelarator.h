/**
 * @file   qxelarator.h
 * @author Imran Ashraf
 * @brief  header file for qxelarator python interface
 */

#ifndef QXELARATOR_H
#define QXELARATOR_H

#include <iostream>
#include <qx_simulator.h>


class QX
{
public:
    qx::simulator * qx_sim;

    QX()
    {
        try
        {
            qx_sim = new qx::simulator();
        }
        catch (std::exception &e)
        {
            std::cerr << "error creating qx::simulator " << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }

    void set(std::string qasm_file_name)
    {
        qx_sim->set(qasm_file_name);
    }

    void execute()
    {
        qx_sim->execute();
    }

    bool get_measurement_outcome(size_t q)
    {
        return qx_sim->move(q);
    }
    std::string get_state()
    {
        return qx_sim->get_state();
    }

};

#endif
