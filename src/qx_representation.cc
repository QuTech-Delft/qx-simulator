#include <qx_representation.h>

namespace qx
{

QxRepresentation::QxRepresentation(uint32_t qubits_count)
: qubits_count(qubits_count)
{}


QxRepresentation::~QxRepresentation()
{
  for (auto circuit = circuits_.begin(); circuit != circuits_.end(); ++circuit)
    delete(*circuit);
}

uint32_t QxRepresentation::getQubitsCount()
{
  return qubits_count;
}

void QxRepresentation::setErrorModel(error_model_t model)
{
  error_model = model;
}

error_model_t QxRepresentation::getErrorModel()
{
  return error_model;
}

void QxRepresentation::setErrorProbability(double probability)
{
  error_probability = probability;
}

double QxRepresentation::getErrorProbability()
{
  return error_probability;
}

std::vector<qx::circuit*> &QxRepresentation::circuits()
{
  return circuits_;
}

} // namepsace qx
