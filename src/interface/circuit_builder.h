/**
 * @file		circuit_builder.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		21-10-15
 * @brief		
 */

#include <core/circuit.h>

namespace qx
{
   class circuit_builder
   {
	 public: 
	   
	  
	   /**
	    * \brief constructor
	    */ 
	   circuit_builder()
	   {
		 c = new qx::circuit(0);
	   }


	   /**
	    * \brief create_qubit set the number of qubits and throw 
	    *   an exception if the number of qubit requires more than
	    *   the available memory.
	    *  
	    *  \param  size : number of qubits
	    */
	   void create_qubit(uint32_t size) throw (unsufficient_memory_exception)
	   {
		 c->set_qubit_count(size);
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
	   int32_t remove_qubits(uint32_t i) throw (out_of_range_exception)
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
	    * \brief execute executes the circuit
	    * 
	    * \return 0 on success else error code
	    */
	   int32_t execute()
	   {
	   }


	   /*
	    * \brief get_state measures the state
	    *
	    * \param qubit qubit identifier
	    */
	   bitset get_state(uint32_t qubit) throw (out_of_range_exception)
	   {
	   }


	 private:
	   
	   qx::circuit * c;  //!< circuit object

   };
}

