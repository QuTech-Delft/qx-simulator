#ifndef QX_REPRESENTATION_H
#define QX_REPRESENTATION_H

#include <vector>
#include <stdint.h>
#include "qx/core/error_model.h"

namespace qx
{

   class QxRepresentation
   {
      public:
         QxRepresentation(size_t count) : qubits_count(count) {}
         ~QxRepresentation()
         {
            for (auto circuit = circuits_.begin(); circuit != circuits_.end(); ++circuit)
               delete(*circuit);
         }


         size_t getQubitsCount()
         {
            return qubits_count;
         }


         error_model_t getErrorModel(void)
         {
            return error_model;
         }

         void setErrorModel(error_model_t model)
         {
            error_model = model;
         }


         void setErrorProbability(double probability)
         {
            error_probability = probability;
         }

         double getErrorProbability(void)
         {
            return error_probability;
         }


         std::vector<qx::circuit*> &circuits(void)
         {
            return circuits_;
         }

      private:
         size_t      qubits_count;
         error_model_t error_model =  __unknown_error_model__;
         double        error_probability;
         std::vector<qx::circuit*> circuits_;
   };

} // namepsace qx

#endif //QX_REPRESENTATION_H
