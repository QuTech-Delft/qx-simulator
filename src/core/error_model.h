#ifndef QX_ERROR_MODEL   
#define QX_ERROR_MODEL   

#include <core/error_injector.h>
#include <core/depolarizing_channel.h>

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

};


#endif // QX_ERROR_MODEL

