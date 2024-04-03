#pragma once

#include <memory>
#include "platforms.h"

typedef uintptr_t NiFpga_Session;

namespace irio {

class ParserManager;
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
