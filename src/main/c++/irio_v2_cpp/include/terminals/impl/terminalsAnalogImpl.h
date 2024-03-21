#pragma once

#include <memory>
#include <unordered_map>

#include "terminals/impl/terminalsBaseImpl.h"
#include "modules.h"

namespace iriov2 {
/**
 * Class implementing the analog functionality.
 *
 * @ingroup Terminals
 */
class TerminalsAnalogImpl: public TerminalsBaseImpl {
 public:
	TerminalsAnalogImpl(
			ParserManager *parserManager,
			const NiFpga_Session &session,
			const Platform &platform);

	std::int32_t getAIImpl(const std::uint32_t n) const;

	std::int32_t getAOImpl(const std::uint32_t n) const;

	std::int32_t getAOEnableImpl(const std::uint32_t n) const;

	size_t getNumAIImpl() const;

	size_t getNumAOImpl() const;

	void setAOImpl(const std::uint32_t n, const std::int32_t value) const;

	void setAOEnableImpl(const std::uint32_t n, const bool value) const;

	ModulesType getModuleConnectedImpl() const;

	double getCVADCImpl() const;

	double getCVDACImpl() const;

	double getMaxValAOImpl() const;

	double getMinValAOImpl() const;

	CouplingMode getAICouplingModeImpl() const;

	void setAICouplingModeImpl(const CouplingMode &mode) const;

 private:
	void searchModule(const Platform &platform);

	void searchFlexRIOModule();

	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAI;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAO;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAOEnable;

	size_t numAI = 0;
	size_t numAO = 0;

	std::unique_ptr<Module> m_module;
};
}  // namespace iriov2
