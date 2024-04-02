#pragma once

#include <utility>
#include <string>

#include "irio_v2.h"

class IrioV2NotInitializedError: public std::runtime_error {
 public:
	IrioV2NotInitializedError() :
			std::runtime_error("IrioV2 driver not initialized") {
	}
};

class IrioV2InstanceManager {
 protected:
	IrioV2InstanceManager() = default;

 public:
	IrioV2InstanceManager(const IrioV2InstanceManager &other) = delete;

	void operator=(const IrioV2InstanceManager&) = delete;

	static std::pair<iriov2::IrioV2*, std::uint32_t> createInstance(
			const std::string &bitfilePath, const std::string &RIOSerialNumber,
			const std::string &FPGAVIversion, const bool verbose = true);

	static iriov2::IrioV2* getInstance(const std::string &RIOSerialNumber,
			const std::uint32_t session);

	static void destroyInstance(const std::string &RIOSerialNumber,
			const std::uint32_t session);
};

