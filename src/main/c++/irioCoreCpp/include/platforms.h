#pragma once

#include <stddef.h>
#include <stdint.h>


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

	const size_t maxAI;
	const size_t maxAuxAI;
	const size_t maxAO;
	const size_t maxAuxAO;
	const size_t maxDigital;
	const size_t maxAuxDigital;
	const size_t maxDMA;
	const size_t maxSG;
	const size_t maxModules;

	const PLATFORM_ID platformID;
};

/**
 * Specific implementation for the FlexRIO platform
 * @ingroup Platforms
 */
class PlatformFlexRIO: public Platform {
 private:
	static const size_t MAX_AI = 4;
	static const size_t MAX_AUX_AI = 16;
	static const size_t MAX_AO = 2;
	static const size_t MAX_AUX_AO = 16;
	static const size_t MAX_DIGITAL = 90;
	static const size_t MAX_AUX_DIGITAL = 16;
	static const size_t MAX_DMA = 16;
	static const size_t MAX_SG = 2;
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
	static const size_t MAX_AI = 256;
	static const size_t MAX_AUX_AI = 256;
	static const size_t MAX_AO = 256;
	static const size_t MAX_AUX_AO = 256;
	static const size_t MAX_DIGITAL = 256;
	static const size_t MAX_AUX_DIGITAL = 256;
	static const size_t MAX_DMA = 3;
	static const size_t MAX_SG = 256;
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
	static const size_t MAX_AI = 8;
	static const size_t MAX_AUX_AI = 16;
	static const size_t MAX_AO = 8;
	static const size_t MAX_AUX_AO = 16;
	static const size_t MAX_DIGITAL = 128;
	static const size_t MAX_AUX_DIGITAL = 16;
	static const size_t MAX_DMA = 3;
	static const size_t MAX_SG = 2;
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

}  // namespace iriov2
