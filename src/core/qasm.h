/**
 * @file		qasm.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		02-10-15
 * @copyright  none - confidential
 * @brief		qasm loader
 */
#ifndef QX_QASM_LOADER_H
#define QX_QASM_LOADER_H

#include <circuit.h>

namespace qx
{
   class qasm_loader
   {
	 public:

	   circuit load(std::string& filename)
	   {
		 qx::circuit c;

		 // load circuit from qasm file

	   }
   };
}

#endif // QX_QASM_LOADER_H

