#pragma once

#include <typeindex>
#include <unordered_map>
#include <memory>

#include "terminals/terminals.h"


namespace iriov2 {

/**
 * Value of DevProfile when DAQ
 */
constexpr std::uint8_t PROFILE_VALUE_DAQ = 0;
/**
 * Value of DevProfile when IMAQ
 */
constexpr std::uint8_t PROFILE_VALUE_IMAQ = 1;
/**
 * Value of DevProfile when DAQGPU
 */
constexpr std::uint8_t PROFILE_VALUE_DAQGPU = 2;
/**
 * Value of DevProfile when IMAQGPU
 */
constexpr std::uint8_t PROFILE_VALUE_IMAQGPU = 3;
/**
 * Value of DevProfile when IO
 */
constexpr std::uint8_t PROFILE_VALUE_IO = 1;


/**
 * All possible types of profiles
 */
enum class PROFILE_ID {
	FLEXRIO_CPUDAQ = 0,/**< Data acquisition in CPU profile for FlexRIO devices */
	FLEXRIO_CPUIMAQ, /**< Image acquisition in CPU profile for FlexRIO devices */
	FLEXRIO_GPUDAQ, /**< Data acquisition in GPU profile for FlexRIO devices */
	FLEXRIO_GPUIMAQ,/**< Image acquisition in GPU profile for FlexRIO devices */
	CRIO_DAQ, /**< Data acquisition profile for cRIO devices */
	CRIO_IO, /**< Point by Point profile for cRIO devices */
	R_DAQ /**< Data acquisition profile for R Series devices */
};

/**
 * Base profile class.
 *
 * Allows the user to access terminals for read/write operations.
 * If a terminal group is not in the profile,
 * an \ref iriov2::errors::TerminalNotImplementedError exception will be thrown.
 *
 * @ingroup Profiles
 */
class ProfileBase {
 public:
	/**
	 * Allows the user to access terminals for read/write operations.
	 *
	 * @param id	Identification of the profile
	 */
	explicit ProfileBase(const PROFILE_ID &id);

	virtual ~ProfileBase() = default;


	template<typename T>
	T getTerminal() const;

	/**
	 * Profile type
	 */
	const PROFILE_ID profileID;

 protected:
	template<typename T>
	void addTerminal(T terminal);

 private:
	std::unordered_map<std::type_index,
			std::unique_ptr<TerminalsBase>> m_mapTerminals;
};

}  // namespace iriov2
