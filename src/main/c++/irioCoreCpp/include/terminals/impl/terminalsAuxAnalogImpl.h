#pragma once

#include <unordered_map>

#include "terminals/impl/terminalsBaseImpl.h"

namespace irio {

/**
 * Class implementing the auxiliary analog
 * functionality
 *
 * @ingroup Terminals
 */
class TerminalsAuxAnalogImpl: public TerminalsBaseImpl {
 public:
	TerminalsAuxAnalogImpl(
			ParserManager *parserManager,
			const NiFpga_Session &session,
			const Platform &platform);

	std::int32_t getAuxAIImpl(const std::uint32_t n) const;

	std::int32_t getAuxAOImpl(const std::uint32_t n) const;

	size_t getNumAuxAIImpl() const;

	size_t getNumAuxAOImpl() const;

	void setAuxAOImpl(const std::uint32_t n, const std::int32_t value) const;

	std::int64_t getAuxAI64Impl(const std::uint32_t n) const;

	std::int64_t getAuxAO64Impl(const std::uint32_t n) const;

	size_t getNumAuxAI64Impl() const;

	size_t getNumAuxAO64Impl() const;

	void setAuxAO64Impl(const std::uint32_t n, const std::int64_t value) const;

 private:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxAI;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxAO;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxAI64;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxAO64;
};

}  // namespace irio
