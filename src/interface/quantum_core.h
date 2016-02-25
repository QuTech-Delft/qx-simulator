/**
 * @file       quantum_core.h
 * @author     Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date       24-10-15
 * @brief      
 */

#ifndef QX_QUANTUM_CORE_H
#define QX_QUANTUM_COER_H

#include <interface/types.h>
#include <core/circuit.h>
#include <interface/gate_factory.h>


namespace qx
{
   /**
    * error codes
    */
   #define __qx_success__                     (0x00)
   #define __qx_error_qubit_already_created__ (0x01)
   #define __qx_error_too_many_qubits__       (0x02)
   #define __qx_error_no_qubits_created__     (0x03)
   #define __qx_error_invalid_gate__          (0x04)
   #define __qx_error_invalid_circuit__       (0x05) 


   /**
    * \brief quantumcore interface 
    */
   class quantum_core
   {
      public:


        /**
         * \brief constructor
         */
        quantum_core() : qubit_count(0),
                         qubits_created(false),
                         circuit_created(false)
        {

        }

        /**
         * \brief create_qubit set the number of qubits and throw 
         *   an exception if the number of qubit requires more than
         *   the available memory.
         *  
         *  \param  size : number of qubits
         */
        uint32_t create_qubits(uint32_t size) // throw (unsufficient_memory_exception)
        {
           if (qubits_created)
           {
              println("[x] error : connot recreate qubits : qubits are already created !");
              return -1;
           }

           reg = new qx::qu_register(size);
           c   = new qx::circuit(size);
           qubit_count     = size;
           qubits_created  = true;
           circuit_created = true;

           return 0;
        }


        /**
         * \brief remove_qubit removes the ith qubit
         *   and throw out_of_bound_exception if i is 
         *   greater that qubit number
         * 
         * \param i the target qubit number 
         * \return 0 on success else error code
         *
         *  ( not supported yet ! )
         */
        int32_t remove_qubits(uint32_t i) // throw (out_of_range_exception)
        {
        }


        /**
         * \brief add_gate adds a gate to the circuit
         * 
         * \param g  the gate to append to the circuit
         * \param time_slot the time slot
         * \return 0 on success else error code
         */
        int32_t add_gate(gate * g, uint32_t time_slot=-1)
        {
        }

        /**
         * \brief add_circuit adds a sub_circuit to the circuit
         * 
         * \param c  the subcircuit to append to the circuit
         * \param time_slot the time slot
         * \return 0 on success else error code
         */
        int32_t add_circuit(circuit_t& sc, uint32_t time_slot=-1)
        {
           for (qx::circuit_t::iterator it=sc.begin(); it != sc.end(); ++it)
              c->add(*it);
        }


        /**
         * \brief execute executes the circuit
         * 
         * \return 0 on success else error code
         */
        int32_t execute()
        {
           c->execute(*reg);
        }


        /*
         * \brief get_state measures the state
         *
         * \param qubit qubit identifier
         */
        state_t get_state(uint32_t qubit) // throw (out_of_range_exception)
        {
        }


        /**
         * \brief dump_state
         */
         void print_state( )
         {
            reg->dump();
         }


      private:

        qx::circuit *     c;    //!< circuit 
        qx::qu_register * reg;  //!< quantum register

        uint32_t qubit_count;
        bool qubits_created;
        bool circuit_created;

   };
}

#endif // QX_QUANTUM_CORE_H
