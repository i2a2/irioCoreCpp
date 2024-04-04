#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cstdint>

namespace irio {

/**
 * Enumeration representing different platform IDs.
 */
enum class PLATFORM_ID: std::uint8_t{
	FlexRIO = 0, /**< FlexRIO platform ID */
	cRIO = 1, /**< cRIO platform ID */
	RSeries = 2 /**< R Series platform ID */
};

/**
 * Generic Platform class. Stores maximum number of resources.
 *
 * @ingroup Platforms
 */
class Platform {
 public:
	/**
	 * Stores maximum number of resources.
	 * @param ai			Max number of AI terminals
	 * @param auxAI			Max number of AuxAI terminals
	 * @param ao			Max number of AO terminals
	 * @param auxAO			Max number of AuxAO terminals
	 * @param digital		Max number of Digital terminals
	 * @param auxDigital	Max number of AuxDigital terminals
	 * @param dma			Max number of DMAs
	 * @param sg			Max number of Signal Generators
	 * @param modules		Max number of modules
	 * @param id			Platform identifier
	 */
	Platform(const size_t ai, const size_t auxAI, const size_t ao,
			const size_t auxAO, const size_t digital, const size_t auxDigital,
			const size_t dma, const size_t sg, const size_t modules,
			const PLATFORM_ID id) :
			maxAI(ai), maxAuxAI(auxAI), maxAO(ao), maxAuxAO(auxAO), maxDigital(
					digital), maxAuxDigital(auxDigital), maxDMA(dma), maxSG(sg),
					maxModules(modules), platformID(id) {
	}

	/// Max number of Analog Input terminals
	const size_t maxAI;
	/// Max number of Aux Analog Input terminals
	const size_t maxAuxAI;
	/// Max number of Analog Output terminals
	const size_t maxAO;
	/// Max number of Aux Analog Output terminals
	const size_t maxAuxAO;
	/// Max number of Digital terminals
	const size_t maxDigital;
	/// Max number of Aux Digital terminals
	const size_t maxAuxDigital;
	/// Max number of DMAs
	const size_t maxDMA;
	/// Max number of Signal Generators
	const size_t maxSG;
	/// Max number of modules
	const size_t maxModules;

	/// Platform identifier
	const PLATFORM_ID platformID;
};

/**
 * Specific implementation for the FlexRIO platform
 * @ingroup Platforms
 */
class PlatformFlexRIO: public Platform {
 private:
	/// Max number of Analog Input terminals for this platform
	static const size_t MAX_AI = 4;
	/// Max number of Aux Analog Input terminals for this platform
	static const size_t MAX_AUX_AI = 16;
	/// Max number of Analog Output terminals for this platform
	static const size_t MAX_AO = 2;
	/// Max number of Aux Analog Output terminals for this platform
	static const size_t MAX_AUX_AO = 16;
	/// Max number of Digital terminals for this platform
	static const size_t MAX_DIGITAL = 90;
	/// Max number of Aux Digital terminals for this platform
	static const size_t MAX_AUX_DIGITAL = 16;
	/// Max number of DMAs for this platform
	static const size_t MAX_DMA = 16;
	/// Max number of Signal Generators for this platform
	static const size_t MAX_SG = 2;
	/// Max number of modules supported by this platform
	static const size_t MAX_MODULES = 1;

 public:
	/**
	 * Specific implementation for the FlexRIO platform
	 */
	PlatformFlexRIO() :
			Platform(MAX_AI, MAX_AUX_AI, MAX_AO, MAX_AUX_AO, MAX_DIGITAL,
					MAX_AUX_DIGITAL, MAX_DMA, MAX_SG, MAX_MODULES,
					PLATFORM_ID::FlexRIO) {
	}
};

/**
 * Specific implementation for the cRIO platform
 * @ingroup Platforms
 */
class PlatformCRIO: public Platform {
 private:
	/// Max number of Analog Input terminals for this platform
	static const size_t MAX_AI = 256;
	/// Max number of Aux Analog Input terminals for this platform
	static const size_t MAX_AUX_AI = 256;
	/// Max number of Analog Output terminals for this platform
	static const size_t MAX_AO = 256;
	/// Max number of Aux Analog Output terminals for this platform
	static const size_t MAX_AUX_AO = 256;
	/// Max number of Digital terminals for this platform
	static const size_t MAX_DIGITAL = 256;
	/// Max number of Aux Digital terminals for this platform
	static const size_t MAX_AUX_DIGITAL = 256;
	/// Max number of DMAs for this platform
	static const size_t MAX_DMA = 3;
	/// Max number of Signal Generators for this platform
	static const size_t MAX_SG = 256;
	/// Max number of modules supported by this platform
	static const size_t MAX_MODULES = 16;

 public:
	/**
	 * Specific implementation for the cRIO platform
	 */
	PlatformCRIO() :
			Platform(MAX_AI, MAX_AUX_AI, MAX_AO, MAX_AUX_AO, MAX_DIGITAL,
					MAX_AUX_DIGITAL, MAX_DMA, MAX_SG, MAX_MODULES,
					PLATFORM_ID::cRIO) {
	}
};

/**
 * Specific implementation for the R Series platform
 * @ingroup Platforms
 */
class PlatformRSeries: public Platform {
 private:
	/// Max number of Analog Input terminals for this platform
	static const size_t MAX_AI = 8;
	/// Max number of Aux Analog Input terminals for this platform
	static const size_t MAX_AUX_AI = 16;
	/// Max number of Analog Output terminals for this platform
	static const size_t MAX_AO = 8;
	/// Max number of Aux Analog Output terminals for this platform
	static const size_t MAX_AUX_AO = 16;
	/// Max number of Digital terminals for this platform
	static const size_t MAX_DIGITAL = 128;
	/// Max number of Aux Digital terminals for this platform
	static const size_t MAX_AUX_DIGITAL = 16;
	/// Max number of DMAs for this platform
	static const size_t MAX_DMA = 3;
	/// Max number of Signal Generators for this platform
	static const size_t MAX_SG = 2;
	/// Max number of modules supported by this platform
	static const size_t MAX_MODULES = 1;

 public:
	/**
	 * Specific implementation for the RSeries platform
	 */
	PlatformRSeries() :
			Platform(MAX_AI, MAX_AUX_AI, MAX_AO, MAX_AUX_AO, MAX_DIGITAL,
					MAX_AUX_DIGITAL, MAX_DMA, MAX_SG, MAX_MODULES,
					PLATFORM_ID::RSeries) {
	}
};

}  // namespace irio
