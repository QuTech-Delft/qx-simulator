#ifndef QX_ERROR_MODEL   
#define QX_ERROR_MODEL   

#include "qx/core/error_injector.h"
#include "qx/core/depolarizing_channel.h"

namespace qx
{
   /**
    * error_model_t
    */
   typedef enum __error_model_t
   {
      __depolarizing_channel__,
      __amplitude_damping__,
      __unknown_error_model__
   } error_model_t;


   qx::circuit * noisy_dep_ch(qx::circuit * c, double p, size_t& total_errors)
   {
      if (c)
      {
         qx::depolarizing_channel dep_ch(c, c->get_qubit_count(), p);
         qx::circuit * noisy_c = dep_ch.inject(false); //true);
         total_errors += dep_ch.get_total_errors();
         return noisy_c;
      }
      return NULL;
   }

};


#endif // QX_ERROR_MODEL

