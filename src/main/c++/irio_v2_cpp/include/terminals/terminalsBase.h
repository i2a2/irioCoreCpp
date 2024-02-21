#pragma once

#include <NiFpga.h>

namespace iriov2 {

/**
 * Base class for terminals
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

