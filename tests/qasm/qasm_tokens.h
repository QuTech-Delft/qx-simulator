/**
 * @file		qasm_tokens.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		19-10-15
 * @brief		
 */

namespace qx
{
   namespace qasm
   {
	 typedef enum 
	 {
	    // identifier
	    __id_token__,		// id
	    __val_token__,		// val
	    // qubit definition
	    __qubit_token__,	// qubit
	    __cbit_token__, 	// cbit
	    // measure, zero
	    __measure_token__,   // measure 
	    __zero_token__,	     // zero
	    // single-qubit gates
	    __hadamard_token__,	// H
	    __pauli_x_token__,	// X    
	    __pauli_y_token__,	// Y
	    __pauli_z_token__,	// Z   
	    __phase_token__,	// S  
	    __sqrt_phase_token__,// T  
	    // ctrl-gates
	    __cnot_token__,		// cnot
	    __toffoli_token__,	// toffoli 
	    __ctrl_z_token__,	// c-z
	    // swap
	    __swap_token__,		// swap  
	    // wire 
	    __nop_token__,		// nop
	    // specials 
	    __error_token__  	// illegal character
	 } qasm_token_t;
   } // qasm
} // qx
