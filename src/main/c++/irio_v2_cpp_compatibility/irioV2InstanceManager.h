#pragma once

#include <string>
#include <memory>

#include "irio_v2.h"


class IrioV2InstanceManager{
 protected:
	IrioV2InstanceManager() = default;

	static std::unique_ptr<iriov2::IrioV2> singleton;

 public:
	IrioV2InstanceManager(const IrioV2InstanceManager &other) = delete;

	void operator=(const IrioV2InstanceManager&) = delete;

	static iriov2::IrioV2* getInstance(const std::string &bitfilePath,
			const std::string &RIOSerialNumber,
			const std::string &FPGAVIversion);

	static iriov2::IrioV2* getInstance();

	static void destroyInstance();
};


