#pragma once

#include <stddef.h>

#define FLEXRIO_PLATFORM_VALUE 0
#define CRIO_PLATFORM_VALUE 1
#define RSERIES_PLATFORM_VALUE 2

namespace iriov2{

class Platform
{
public:
	Platform(size_t ai, size_t auxAI, size_t ao, size_t auxAO,
	             size_t digital, size_t auxDigital, size_t dma,
	             size_t sg, size_t modules)
	        : maxAI(ai), maxAuxAI(auxAI), maxAO(ao), maxAuxAO(auxAO),
	          maxDigital(digital), maxAuxDigital(auxDigital),
	          maxDMA(dma), maxSG(sg), maxModules(modules) {}

	const size_t maxAI;
	const size_t maxAuxAI;
	const size_t maxAO;
	const size_t maxAuxAO;
	const size_t maxDigital;
	const size_t maxAuxDigital;
	const size_t maxDMA;
	const size_t maxSG;
	const size_t maxModules;
};

class FlexRIO: public Platform
{
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
	FlexRIO(): Platform(MAX_AI, MAX_AUX_AI, MAX_AO, MAX_AUX_AO,
			MAX_DIGITAL, MAX_AUX_DIGITAL, MAX_DMA, MAX_SG, MAX_MODULES){}
};

class CRIO: public Platform
{
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
	CRIO(): Platform(MAX_AI, MAX_AUX_AI, MAX_AO, MAX_AUX_AO,
			MAX_DIGITAL, MAX_AUX_DIGITAL, MAX_DMA, MAX_SG, MAX_MODULES){}
};

class RSeries: public Platform
{
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
	RSeries(): Platform(MAX_AI, MAX_AUX_AI, MAX_AO, MAX_AUX_AO,
			MAX_DIGITAL, MAX_AUX_DIGITAL, MAX_DMA, MAX_SG, MAX_MODULES){}
};

}
