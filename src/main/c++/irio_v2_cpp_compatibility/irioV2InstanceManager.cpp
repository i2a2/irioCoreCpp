#include "irioV2InstanceManager.h"

std::unique_ptr<iriov2::IrioV2>
	IrioV2InstanceManager::singleton(nullptr);

iriov2::IrioV2* IrioV2InstanceManager::getInstance(
		const std::string &bitfilePath, const std::string &RIOSerialNumber,
		const std::string &FPGAVIversion) {
	if (!singleton) {
		singleton.reset(
				new iriov2::IrioV2(bitfilePath, RIOSerialNumber,
						FPGAVIversion));
	}

	return singleton.get();
}

iriov2::IrioV2* IrioV2InstanceManager::getInstance() {
	if(!singleton) {
		throw IrioV2NotInitializedError();
	}
	return singleton.get();
}

void IrioV2InstanceManager::destroyInstance() {
	singleton.reset(nullptr);
}
