/**
 * @file       gate_factory.h
 * @author     Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date       24-10-15
 * @brief      gate factory
 */

#ifndef QX_GATE_FACTORY_H
#define QX_GATE_FACTORY_H


#include <core/gate.h>

namespace qx
{

   /**
    * \brief gate factory
    */
   class gate_factory
   {
      public:

        /**
         * \brief hadamard
         */
        qx::gate * hadamard(uint32_t qubit)
        {
           return new qx::hadamard(qubit);
        }


        /**
         * \brief pauli_x
         */
        qx::gate * pauli_x(uint32_t qubit)
        {
           return new qx::pauli_x(qubit);
        }

        /**
         * \brief pauli_y
         */
        qx::gate * pauli_y(uint32_t qubit)
        {
           return new qx::pauli_y(qubit);
        }

        /**
         * \brief pauli_z
         */
        qx::gate * pauli_z(uint32_t qubit)
        {
           return new qx::pauli_z(qubit);
        }

        /**
         * \brief phase 
         */
        qx::gate * phase_shift(uint32_t qubit)
        {
           return new qx::phase_shift(qubit);
        }

        /**
         * \brief cnot
         */
        qx::gate * cnot(uint32_t ctrl_qubit, uint32_t target_qubit)
        {
           return new qx::cnot(ctrl_qubit, target_qubit);
        }

      private:

   };

} // namespace qx

#endif // QX_GATE_FACTORY_H

