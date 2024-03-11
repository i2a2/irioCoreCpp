#pragma once

#include <stdint.h>
#include <NiFpga.h>
#include <niflexrio.h>
#include <unordered_set>
#include <unordered_map>
#include <cstring>
#include <string>
#include <memory>
#include "fff.h"



namespace iriov2 {
DECLARE_FAKE_VALUE_FUNC(std::string, searchRIODevice, const std::string);
}  // namespace iriov2

///////////////////////////////////////////////////////////////
///// FAKE FUNCTIONS NiFpga
///////////////////////////////////////////////////////////////

#define DECLARE_FAKE_NIFPGA_FUNC(funcName, ...) DECLARE_FAKE_VALUE_FUNC(NiFpga_Status, funcName, __VA_ARGS__)

DECLARE_FAKE_NIFPGA_FUNC(NiFlexRio_GetAttribute, NiFpga_Session, int32_t, int32_t, void*);

DECLARE_FAKE_NIFPGA_FUNC(NiFpga_Open, const char*, const char*, const char*, uint32_t, NiFpga_Session*);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_Close, NiFpga_Session, uint32_t);

DECLARE_FAKE_NIFPGA_FUNC(NiFpga_ReadBool, NiFpga_Session, uint32_t, NiFpga_Bool*);

DECLARE_FAKE_NIFPGA_FUNC(NiFpga_ReadI8, NiFpga_Session, uint32_t, int8_t*);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_ReadI16, NiFpga_Session, uint32_t, int16_t*);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_ReadI32, NiFpga_Session, uint32_t, int32_t*);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_ReadI64, NiFpga_Session, uint32_t, int64_t*);

DECLARE_FAKE_NIFPGA_FUNC(NiFpga_ReadU8, NiFpga_Session, uint32_t, uint8_t*);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_ReadU16, NiFpga_Session, uint32_t, uint16_t*);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_ReadU32, NiFpga_Session, uint32_t, uint32_t*);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_ReadU64, NiFpga_Session, uint32_t, uint64_t*);

DECLARE_FAKE_NIFPGA_FUNC(NiFpga_WriteBool, NiFpga_Session, uint32_t, NiFpga_Bool);

DECLARE_FAKE_NIFPGA_FUNC(NiFpga_WriteI8, NiFpga_Session, uint32_t, int8_t);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_WriteI16, NiFpga_Session, uint32_t, int16_t);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_WriteI32, NiFpga_Session, uint32_t, int32_t);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_WriteI64, NiFpga_Session, uint32_t, int64_t);

DECLARE_FAKE_NIFPGA_FUNC(NiFpga_WriteU8, NiFpga_Session, uint32_t, uint8_t);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_WriteU16, NiFpga_Session, uint32_t, uint16_t);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_WriteU32, NiFpga_Session, uint32_t, uint32_t);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_WriteU64, NiFpga_Session, uint32_t, uint64_t);

DECLARE_FAKE_NIFPGA_FUNC(NiFpga_ReadArrayU8, NiFpga_Session, uint32_t, uint8_t*, size_t);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_ReadArrayU16, NiFpga_Session, uint32_t, uint16_t*, size_t);

DECLARE_FAKE_NIFPGA_FUNC(NiFpga_ReadFifoU64, NiFpga_Session, uint32_t, uint64_t*, size_t, uint32_t, size_t*);

DECLARE_FAKE_NIFPGA_FUNC(NiFpga_ConfigureFifo, NiFpga_Session, uint32_t, size_t);

DECLARE_FAKE_NIFPGA_FUNC(NiFpga_Run, NiFpga_Session, uint32_t);

DECLARE_FAKE_NIFPGA_FUNC(NiFpga_StartFifo, NiFpga_Session, uint32_t);
DECLARE_FAKE_NIFPGA_FUNC(NiFpga_StopFifo, NiFpga_Session, uint32_t);


enum class ReadFunctions: int {
	NiFpga_ReadBool,
	NiFpga_ReadI8,
	NiFpga_ReadI16,
	NiFpga_ReadI32,
	NiFpga_ReadI64,
	NiFpga_ReadU8,
	NiFpga_ReadU16,
	NiFpga_ReadU32,
	NiFpga_ReadU64,
};

enum class ReadArrayFunctions: int {
	NiFpga_ReadArrayU8,
	NiFpga_ReadArrayU16
};


template<typename T>
void setValueForReg(const ReadFunctions func, const uint32_t reg, const T value);

template<typename T>
void setValueForReg(const ReadArrayFunctions func, const uint32_t reg, const T* value, const size_t size);

template<ReadFunctions R, typename T>
NiFpga_Status fakeReadFunc(NiFpga_Session, uint32_t reg, T* value);

template<ReadArrayFunctions R, typename T>
NiFpga_Status fakeReadArrayFunc(NiFpga_Session, uint32_t reg, T* array, size_t size);

void init_ok_fff_nifpga();

void reset_fff_nifpga();
