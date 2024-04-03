#include <unordered_map>
#include <memory>
#include <mutex>

#include "irioV2InstanceManager.h"

using SessionID = std::uint32_t;
using IrioV2ptr = std::unique_ptr<irio::IrioV2>;

struct ManageDevice {
	std::mutex mutex;
	std::unordered_map<SessionID, IrioV2ptr> mapSessions;
};

std::unordered_map<std::string, ManageDevice> mapDevices;


std::pair<irio::IrioV2*, std::uint32_t> IrioV2InstanceManager::createInstance(
		const std::string &bitfilePath, const std::string &RIOSerialNumber,
		const std::string &FPGAVIversion, const bool verbose) {
	auto irioV2ptr = IrioV2ptr(new irio::IrioV2(bitfilePath, RIOSerialNumber,
												  FPGAVIversion, verbose));
	const auto id = irioV2ptr->getID();

	// If device exists, it will be returned. If not, a new entry is created
	auto mngDev = &mapDevices.emplace(std::piecewise_construct,
	                                  std::forward_as_tuple(RIOSerialNumber),
	                                  std::forward_as_tuple()).first->second;

	std::lock_guard<std::mutex>(mngDev->mutex);
	auto it = mngDev->mapSessions.emplace(id, std::move(irioV2ptr)).first;
	return std::make_pair(it->second.get(), id);
}

irio::IrioV2* IrioV2InstanceManager::getInstance(
		const std::string &RIOSerialNumber,
		const std::uint32_t session) {
	try {
		const auto mngDev = &mapDevices.at(RIOSerialNumber);
		std::lock_guard<std::mutex>(mngDev->mutex);
		return mngDev->mapSessions.at(session).get();
	} catch(std::out_of_range&) {
		throw IrioV2NotInitializedError();
	}
}

void IrioV2InstanceManager::destroyInstance(
		const std::string &RIOSerialNumber,
		const std::uint32_t session) {
	try {
		const auto mngDev = &mapDevices.at(RIOSerialNumber);
		std::lock_guard<std::mutex>(mngDev->mutex);
		mngDev->mapSessions.erase(session);
	} catch(std::out_of_range&) {
		throw IrioV2NotInitializedError();
	}
}
