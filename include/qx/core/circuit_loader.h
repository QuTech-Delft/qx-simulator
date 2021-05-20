/**
 * @file		cicuit_loader.h
 * @author	Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date		03-10-15
 * @brief		common interface for circuit loader
 */
#ifndef QX_CICUIT_LOADER_H
#define QX_CICUIT_LOADER_H

#include "qx/core/circuit.h"

namespace qx
{
   class circuit_loader
   {
	 public:

	   virtual circuit load(std::string& filename) = 0; 
   };
}


#endif // QX_CICUIT_LOADER_H
