#ifndef QX_ERROR_INJECTOR
#define QX_ERROR_INJECTOR

#include <xpu/timer.h>
#include <core/circuit.h>
#include <core/gaussian.h>
#include <core/random.h>

namespace qx
{
   /**
    * \brief common error injector interface
    */
   class error_injector
   {
      public:
         
         virtual qx::circuit *  inject(bool) = 0;
	 virtual                ~error_injector() { };                
	 virtual void           dump() = 0;
   };

}


#endif // QX_ERROR_INJECTOR

