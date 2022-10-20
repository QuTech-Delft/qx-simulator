#pragma once

#include "qx/core/circuit.h"
#include "qx/core/gaussian.h"
#include "qx/xpu/timer.h"

namespace qx {
/**
 * \brief common error injector interface
 */
class error_injector {
public:
    virtual std::shared_ptr<qx::circuit> inject(bool) = 0;
    virtual ~error_injector(){};
    virtual void dump() = 0;
};

} // namespace qx
