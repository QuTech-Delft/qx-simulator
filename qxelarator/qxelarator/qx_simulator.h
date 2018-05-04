/**
 * @file    qx_simulator.h
 * @author	Nader Khammassi
 * @date	   23-12-16
 * @brief	qx simulator interface
 */

#ifndef QX_SIMULATOR_H
#define QX_SIMULATOR_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>

#include <stdint.h>
#include <iostream>

#include <xpu.h>
#include <xpu/runtime>

#include <core/circuit.h>
#include <qcode/quantum_code_loader.h>
#include <core/error_model.h>


/**
 * i/o helpers
 */

#ifndef println
#define println(x) std::cout << x << std::endl
#endif

#ifndef error
#define error(x) std::cout << "[x] error : " << x << std::endl;
#endif

using namespace str;  // string utils

namespace qx
{

typedef std::map<std::string,std::string>   map_t;
typedef std::vector<qx::circuit *>          circuits_t;
typedef std::vector<qx::quantum_state_t *>  quantum_states_t;

/**
 * simulator
 */
class simulator
{
protected:
    qx::qu_register * reg;
    std::string file_name;

public:
    simulator() {}

    void set(std::string fname)
    {
      file_name = fname;
    }

    /**
     * execute qasm file
     */
    void execute()
    {
        size_t total_errors = 0;
        qx::circuits_t circuits;

        println("[+] loading circuit from '" << file_name << "' ...");
        qx::quantum_code_parser qcp(file_name);
        qcp.parse();
        // qcp.dump();

        println("[+] creating quantum register... ");
        reg = new qx::qu_register(qcp.qubits());

        // check whether an error model is specified
        if (qcp.get_error_model() == qx::__depolarizing_channel__)
        {
            println("[+] generating noisy circuits...");
            double error_probability = qcp.get_error_probability();
            qx::circuits_t perfect_circuits = qcp.get_circuits();
            for (size_t i=0; i<perfect_circuits.size(); i++)
            {
                println("[>] processing circuit '" << perfect_circuits[i]->id() << "'...");
                size_t iterations = perfect_circuits[i]->get_iterations();
                if (iterations > 1)
                {
                    for (size_t it=0; it<iterations; ++it)
                    {
                        qx::depolarizing_channel dep_ch(perfect_circuits[i], qcp.qubits(), error_probability);
                        qx::circuit * noisy_c = dep_ch.inject(true);
                        println("[+] noisy circuit : " << std::hex << noisy_c);
                        circuits.push_back(noisy_c);
                        total_errors += dep_ch.get_total_errors();
                    }
                }
                else
                {
                    qx::depolarizing_channel dep_ch(perfect_circuits[i], qcp.qubits(), error_probability);
                    qx::circuit * noisy_c = dep_ch.inject(true);
                    circuits.push_back(noisy_c);
                    total_errors += dep_ch.get_total_errors();
                }
            }
            println("[+] total errors injected in all circuits : " << total_errors);

        }
        else
            circuits = qcp.get_circuits();

        // qcp.execute(reg);
        for (size_t i=0; i<circuits.size(); i++)
            circuits[i]->execute(*reg);
    }

    bool move(size_t q)
    {
      return reg->get_measurement(q);
    }

    std::string get_state()
    {
        return reg->get_state();
    }
};
}

#endif // QX_SIMULATOR_H
