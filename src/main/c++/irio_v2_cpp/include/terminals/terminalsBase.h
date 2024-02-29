#pragma once

#include <unordered_map>
#include <memory>
#include <NiFpga.h>
#include <bfp.h>
#include "platforms.h"

namespace iriov2 {

/**
 * Base class for terminals
 *
 * @ingroup Terminals
 */
class TerminalsBase {
public:
	/**
	 * Base class for terminals
	 * @param session	NiFpga_Session to be used in NiFpga related functions. Must be a valid one.
	 */
	TerminalsBase(const NiFpga_Session &session);

protected:
	const NiFpga_Session m_session;

};

}

