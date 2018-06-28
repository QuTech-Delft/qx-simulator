#ifndef QX_REPRESENTATION_H
#define QX_REPRESENTATION_H

#include <vector>
#include <core/circuit.h>
#include <core/error_model.h>

namespace qx
{

class QxRepresentation
{
  public:
    QxRepresentation(uint32_t) : qubits_count(qubits_count) {}
    ~QxRepresentation()
    {
      for (auto circuit = circuits_.begin(); circuit != circuits_.end(); ++circuit)
        delete(*circuit);
    }


    uint32_t getQubitsCount()
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
    uint32_t      qubits_count;
    error_model_t error_model =  __unknown_error_model__;
    double        error_probability;
    std::vector<qx::circuit*> circuits_;
};

} // namepsace qx

#endif //QX_REPRESENTATION_H
