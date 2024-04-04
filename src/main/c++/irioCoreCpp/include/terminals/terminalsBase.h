#pragma once

#include <memory>
#include "platforms.h"

#ifdef CODAC_ROOT
typedef uintptr_t NiFpga_Session;
#else
typedef uint32_t NiFpga_Session;
#endif

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
  /**
   * Base class used for terminals.
   *
   * @param impl A shared pointer to the actual terminal implementation
   */
	explicit TerminalsBase(std::shared_ptr<TerminalsBaseImpl> impl);

 protected:
	/// Smart pointer with the terminal implementation
	std::shared_ptr<TerminalsBaseImpl> m_impl;
};
}  // namespace irio
