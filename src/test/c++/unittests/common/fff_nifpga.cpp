#include "fff_nifpga.h"
#include <string>
#include <platforms.h>

DEFINE_FFF_GLOBALS

#define DEFINE_FAKE_NIFPGA_FUNC(funcName, ...) DEFINE_FAKE_VALUE_FUNC(NiFpga_Status, funcName, __VA_ARGS__)


namespace irio {
DEFINE_FAKE_VALUE_FUNC(std::string, searchRIODevice, const std::string);
}  // namespace iriov2


DEFINE_FAKE_NIFPGA_FUNC(NiFlexRio_GetAttribute, NiFpga_Session, int32_t, int32_t, void*);

DEFINE_FAKE_NIFPGA_FUNC(NiFpga_Open, const char*, const char*, const char*, uint32_t, NiFpga_Session*);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_Close, NiFpga_Session, uint32_t);

DEFINE_FAKE_NIFPGA_FUNC(NiFpga_ReadBool, NiFpga_Session, uint32_t, NiFpga_Bool*);

DEFINE_FAKE_NIFPGA_FUNC(NiFpga_ReadI8, NiFpga_Session, uint32_t, int8_t*);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_ReadI16, NiFpga_Session, uint32_t, int16_t*);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_ReadI32, NiFpga_Session, uint32_t, int32_t*);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_ReadI64, NiFpga_Session, uint32_t, int64_t*);

DEFINE_FAKE_NIFPGA_FUNC(NiFpga_ReadU8, NiFpga_Session, uint32_t, uint8_t*);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_ReadU16, NiFpga_Session, uint32_t, uint16_t*);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_ReadU32, NiFpga_Session, uint32_t, uint32_t*);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_ReadU64, NiFpga_Session, uint32_t, uint64_t*);

DEFINE_FAKE_NIFPGA_FUNC(NiFpga_WriteBool, NiFpga_Session, uint32_t, NiFpga_Bool);

DEFINE_FAKE_NIFPGA_FUNC(NiFpga_WriteI8, NiFpga_Session, uint32_t, int8_t);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_WriteI16, NiFpga_Session, uint32_t, int16_t);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_WriteI32, NiFpga_Session, uint32_t, int32_t);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_WriteI64, NiFpga_Session, uint32_t, int64_t);

DEFINE_FAKE_NIFPGA_FUNC(NiFpga_WriteU8, NiFpga_Session, uint32_t, uint8_t);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_WriteU16, NiFpga_Session, uint32_t, uint16_t);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_WriteU32, NiFpga_Session, uint32_t, uint32_t);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_WriteU64, NiFpga_Session, uint32_t, uint64_t);

DEFINE_FAKE_NIFPGA_FUNC(NiFpga_ReadArrayU8, NiFpga_Session, uint32_t, uint8_t*, size_t);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_ReadArrayU16, NiFpga_Session, uint32_t, uint16_t*, size_t);

DEFINE_FAKE_NIFPGA_FUNC(NiFpga_ReadFifoU64, NiFpga_Session, uint32_t, uint64_t*, size_t, uint32_t, size_t*);

DEFINE_FAKE_NIFPGA_FUNC(NiFpga_ConfigureFifo, NiFpga_Session, uint32_t, size_t);

DEFINE_FAKE_NIFPGA_FUNC(NiFpga_Run, NiFpga_Session, uint32_t);

DEFINE_FAKE_NIFPGA_FUNC(NiFpga_StartFifo, NiFpga_Session, uint32_t);
DEFINE_FAKE_NIFPGA_FUNC(NiFpga_StopFifo, NiFpga_Session, uint32_t);



std::unordered_map<ReadFunctions, std::unordered_map<uint32_t, std::shared_ptr<uint8_t>>> mapValuesReadReg;
std::unordered_map<ReadArrayFunctions, std::unordered_map<uint32_t, std::shared_ptr<uint8_t>>> mapValuesReadArrayReg;


template<typename T>
void setValueForReg(const ReadFunctions func, const uint32_t reg, const T value){
	auto it = mapValuesReadReg.find(func);
	if(it == mapValuesReadReg.end()){
		mapValuesReadReg[func] = {};
		it = mapValuesReadReg.find(func);
	}

	const auto itAux = it->second.find(reg);
	if(itAux == it->second.end()){
		it->second.emplace(reg, new uint8_t[sizeof(T)]);
	}else{
		itAux->second.reset(new uint8_t[sizeof(T)]);
	}
	auto aux = it->second.find(reg)->second;
	std::memcpy(aux.get(), &value, sizeof(T));
}

template<>
void setValueForReg<irio::PLATFORM_ID>(const ReadFunctions func, const uint32_t reg, const irio::PLATFORM_ID value){
	setValueForReg(func, reg, static_cast<std::uint8_t>(value));
}
template void setValueForReg<uint8_t>(const ReadFunctions func, const uint32_t reg, const uint8_t value);
template void setValueForReg<uint16_t>(const ReadFunctions func, const uint32_t reg, const uint16_t value);
template void setValueForReg<uint32_t>(const ReadFunctions func, const uint32_t reg, const uint32_t value);
template void setValueForReg<uint64_t>(const ReadFunctions func, const uint32_t reg, const uint64_t value);
template void setValueForReg<int8_t>(const ReadFunctions func, const uint32_t reg, const int8_t value);
template void setValueForReg<int16_t>(const ReadFunctions func, const uint32_t reg, const int16_t value);
template void setValueForReg<int32_t>(const ReadFunctions func, const uint32_t reg, const int32_t value);
template void setValueForReg<int64_t>(const ReadFunctions func, const uint32_t reg, const int64_t value);

template<typename T>
void setValueForReg(const ReadArrayFunctions func, const uint32_t reg, const T* value, const size_t size){
	auto it = mapValuesReadArrayReg.find(func);
	if(it == mapValuesReadArrayReg.end()){
		mapValuesReadArrayReg[func] = {};
		it = mapValuesReadArrayReg.find(func);
	}

	const auto itAux = it->second.find(reg);
	if(itAux == it->second.end()){
		it->second.emplace(reg, new uint8_t[size*sizeof(T)]);
	}else{
		itAux->second.reset(new uint8_t[size*sizeof(T)]);
	}
	auto aux = it->second.find(reg)->second;
	std::memcpy(aux.get(), value, sizeof(T)*size);
}
template void setValueForReg<uint8_t>(const ReadArrayFunctions func, const uint32_t reg, const uint8_t* value, const size_t size);
template void setValueForReg<uint16_t>(const ReadArrayFunctions func, const uint32_t reg, const uint16_t* value, const size_t size);


template<ReadFunctions R, typename T>
NiFpga_Status fakeReadFunc(NiFpga_Session, uint32_t reg, T* value){
	const auto regIt = mapValuesReadReg.find(R);
	if(regIt != mapValuesReadReg.end()){
		const auto valIt = regIt->second.find(reg);
		if(valIt != regIt->second.end()){
			std::memcpy(value, valIt->second.get(), sizeof(T));
		}else{
			std::memset(value, 0, sizeof(T));
		}
	}
	return NiFpga_Status_Success;
}
template NiFpga_Status fakeReadFunc<ReadFunctions::NiFpga_ReadBool, uint8_t>(NiFpga_Session, uint32_t reg, uint8_t* value);
template NiFpga_Status fakeReadFunc<ReadFunctions::NiFpga_ReadU8, uint8_t>(NiFpga_Session, uint32_t reg, uint8_t* value);
template NiFpga_Status fakeReadFunc<ReadFunctions::NiFpga_ReadU16, uint16_t>(NiFpga_Session, uint32_t reg, uint16_t* value);
template NiFpga_Status fakeReadFunc<ReadFunctions::NiFpga_ReadU32, uint32_t>(NiFpga_Session, uint32_t reg, uint32_t* value);
template NiFpga_Status fakeReadFunc<ReadFunctions::NiFpga_ReadU64, uint64_t>(NiFpga_Session, uint32_t reg, uint64_t* value);
template NiFpga_Status fakeReadFunc<ReadFunctions::NiFpga_ReadI8, int8_t>(NiFpga_Session, uint32_t reg, int8_t* value);
template NiFpga_Status fakeReadFunc<ReadFunctions::NiFpga_ReadI16, int16_t>(NiFpga_Session, uint32_t reg, int16_t* value);
template NiFpga_Status fakeReadFunc<ReadFunctions::NiFpga_ReadI32, int32_t>(NiFpga_Session, uint32_t reg, int32_t* value);
template NiFpga_Status fakeReadFunc<ReadFunctions::NiFpga_ReadI64, int64_t>(NiFpga_Session, uint32_t reg, int64_t* value);


template<ReadArrayFunctions R, typename T>
NiFpga_Status fakeReadArrayFunc(NiFpga_Session, uint32_t reg, T* array, size_t size){
	const auto regIt = mapValuesReadArrayReg.find(R);
	if(regIt != mapValuesReadArrayReg.end()){
		const auto valIt = regIt->second.find(reg);
		if(valIt != regIt->second.end()){
			std::memcpy(array, valIt->second.get(), sizeof(T)*size);
		}else{
			std::memset(array, 0, sizeof(T)*size);
		}
	}
	return NiFpga_Status_Success;
}
template NiFpga_Status fakeReadArrayFunc<ReadArrayFunctions::NiFpga_ReadArrayU8, uint8_t>(NiFpga_Session, uint32_t reg, uint8_t* array, size_t size);
template NiFpga_Status fakeReadArrayFunc<ReadArrayFunctions::NiFpga_ReadArrayU16, uint16_t>(NiFpga_Session, uint32_t reg, uint16_t* array, size_t size);

void init_ok_fff_nifpga(){
	NiFpga_Open_fake.custom_fake = [](const char*, const char*, const char*, uint32_t, NiFpga_Session* session){
		*session = 42;
		return NiFpga_Status_Success;
	};

	NiFlexRio_GetAttribute_fake.custom_fake = [](NiFpga_Session, int32_t, int32_t, void* value){
		*reinterpret_cast<uint32_t*>(value) = 0;
		return NiFpga_Status_Success;
	};

	NiFpga_ReadBool_fake.custom_fake = fakeReadFunc<ReadFunctions::NiFpga_ReadBool, NiFpga_Bool>;
	NiFpga_ReadI8_fake.custom_fake = fakeReadFunc<ReadFunctions::NiFpga_ReadI8, int8_t>;
	NiFpga_ReadU8_fake.custom_fake = fakeReadFunc<ReadFunctions::NiFpga_ReadU8, uint8_t>;
	NiFpga_ReadI16_fake.custom_fake = fakeReadFunc<ReadFunctions::NiFpga_ReadI16, int16_t>;
	NiFpga_ReadU16_fake.custom_fake = fakeReadFunc<ReadFunctions::NiFpga_ReadU16, uint16_t>;
	NiFpga_ReadI32_fake.custom_fake = fakeReadFunc<ReadFunctions::NiFpga_ReadI32, int32_t>;
	NiFpga_ReadU32_fake.custom_fake = fakeReadFunc<ReadFunctions::NiFpga_ReadU32, uint32_t>;
	NiFpga_ReadI64_fake.custom_fake = fakeReadFunc<ReadFunctions::NiFpga_ReadI64, int64_t>;
	NiFpga_ReadU64_fake.custom_fake = fakeReadFunc<ReadFunctions::NiFpga_ReadU64, uint64_t>;

	NiFpga_ReadArrayU8_fake.custom_fake = fakeReadArrayFunc<ReadArrayFunctions::NiFpga_ReadArrayU8, uint8_t>;
	NiFpga_ReadArrayU16_fake.custom_fake = fakeReadArrayFunc<ReadArrayFunctions::NiFpga_ReadArrayU16, uint16_t>;

	NiFpga_ReadFifoU64_fake.custom_fake = [](NiFpga_Session, uint32_t,
			uint64_t* data, size_t numberOfElements, uint32_t, size_t* elementsRemaining) {
		std::memset(data, numberOfElements, 7);
		if(elementsRemaining)
			*elementsRemaining = 0;

		return NiFpga_Status_Success;
	};

	NiFpga_ConfigureFifo_fake.return_val = NiFpga_Status_Success;
	NiFpga_StartFifo_fake.return_val = NiFpga_Status_Success;
	NiFpga_StopFifo_fake.return_val = NiFpga_Status_Success;
}

void reset_fff_nifpga(){
	RESET_FAKE(NiFpga_Open);
	RESET_FAKE(NiFpga_Close);
	RESET_FAKE(NiFpga_ReadI8);
	RESET_FAKE(NiFpga_ReadI16);
	RESET_FAKE(NiFpga_ReadI32);
	RESET_FAKE(NiFpga_ReadI64);
	RESET_FAKE(NiFpga_ReadU8);
	RESET_FAKE(NiFpga_ReadU16);
	RESET_FAKE(NiFpga_ReadU32);
	RESET_FAKE(NiFpga_ReadU64);
	RESET_FAKE(NiFpga_WriteI8);
	RESET_FAKE(NiFpga_WriteI16);
	RESET_FAKE(NiFpga_WriteI32);
	RESET_FAKE(NiFpga_WriteI64);
	RESET_FAKE(NiFpga_WriteU8);
	RESET_FAKE(NiFpga_WriteU16);
	RESET_FAKE(NiFpga_WriteU32);
	RESET_FAKE(NiFpga_WriteU64);
	RESET_FAKE(NiFpga_ReadArrayU8);
	RESET_FAKE(NiFpga_ReadArrayU16);
	RESET_FAKE(NiFpga_ReadFifoU64);
	RESET_FAKE(NiFpga_ConfigureFifo);
	RESET_FAKE(NiFpga_Run);
	RESET_FAKE(NiFpga_StartFifo);
	RESET_FAKE(NiFpga_StopFifo);

	mapValuesReadArrayReg.clear();
	mapValuesReadReg.clear();
}
