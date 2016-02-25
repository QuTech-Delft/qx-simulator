/**
 * @file       types.h
 * @author     Nader KHAMMASSI - nader.khammassi@gmail.com 
 * @date       26-10-15
 * @brief      
 */

#ifndef QX_INTERFACE_TYPES_H
#define QX_INTERFACE_TYPES_H

#include <vector>
#include <core/gate.h>

namespace qx
{
   /**
    * circuit : gate vector
    */
   typedef std::vector<gate *> circuit_t;

   /**
    * state    // already defined in register
    */
    /*
   typedef enum __state
   {
      __state_high__,    // 1
      __state_low__,     // 0
      __state_unknown__  // X
   } state_t;
   */

} // qx

#endif // QX_INTERFACE_TYPES_H

