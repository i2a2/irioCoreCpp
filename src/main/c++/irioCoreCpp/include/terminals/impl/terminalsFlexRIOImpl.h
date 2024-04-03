#pragma once

#include "terminals/impl/terminalsBaseImpl.h"

namespace irio {
/**
 * Class implementing the functionality
 * specific to FlexRIO
 *
 * @ingroup Terminals
 * @ingroup FlexRIO
 */
class TerminalsFlexRIOImpl: public TerminalsBaseImpl {
 public:
	TerminalsFlexRIOImpl(ParserManager *parserManager,
						const NiFpga_Session &session);

	bool getRIOAdapterCorrect() const;

	std::uint32_t getInsertedIOModuleID() const;

 private:
	std::uint32_t m_rioadaptercorrect_addr;
	std::uint32_t m_insertediomoduleid_addr;
};

}  // namespace irio
