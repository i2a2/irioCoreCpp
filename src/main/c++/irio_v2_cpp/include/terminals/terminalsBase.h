#pragma once

#include <memory>
#include <NiFpga.h>
#include "bfp.h"
#include "platforms.h"

namespace iriov2 {

class TerminalsBaseImpl;
/**
 * Base class for all terminals
 *
 * @ingroup Terminals
 */
class TerminalsBase {
 public:
	explicit TerminalsBase(std::shared_ptr<TerminalsBaseImpl> impl);

 protected:
	std::shared_ptr<TerminalsBaseImpl> m_impl;
};
}  // namespace iriov2
