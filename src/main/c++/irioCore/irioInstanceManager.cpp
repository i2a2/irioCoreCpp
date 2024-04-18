#include <unordered_map>
#include <memory>
#include <mutex>

#include "irioInstanceManager.h"

using SessionID = std::uint32_t;
using IrioPtr = std::unique_ptr<irio::Irio>;

struct ManageDevice {
	std::mutex mutex;
	std::unordered_map<SessionID, IrioPtr> mapSessions;
};

std::unordered_map<std::string, ManageDevice> mapDevices;


std::pair<irio::Irio*, std::uint32_t> IrioInstanceManager::createInstance(
		const std::string &bitfilePath, const std::string &RIOSerialNumber,
		const std::string &FPGAVIversion, const bool verbose) {
	auto irioptr = IrioPtr(new irio::Irio(bitfilePath, RIOSerialNumber,
												  FPGAVIversion, verbose));
	const auto id = irioptr->getID();

	// If device exists, it will be returned. If not, a new entry is created
	auto mngDev = &mapDevices.emplace(std::piecewise_construct,
	                                  std::forward_as_tuple(RIOSerialNumber),
	                                  std::forward_as_tuple()).first->second;

	std::lock_guard<std::mutex> mtx(mngDev->mutex);
	auto it = mngDev->mapSessions.emplace(id, std::move(irioptr)).first;
	return std::make_pair(it->second.get(), id);
}

irio::Irio* IrioInstanceManager::getInstance(
		const std::string &RIOSerialNumber,
		const std::uint32_t session) {
	try {
		const auto mngDev = &mapDevices.at(RIOSerialNumber);
		std::lock_guard<std::mutex> mtx(mngDev->mutex);
		return mngDev->mapSessions.at(session).get();
	} catch(std::out_of_range&) {
		throw IrioNotInitializedError();
	}
}

void IrioInstanceManager::destroyInstance(
		const std::string &RIOSerialNumber,
		const std::uint32_t session) {
	try {
		const auto mngDev = &mapDevices.at(RIOSerialNumber);
		std::lock_guard<std::mutex> mtx(mngDev->mutex);
		mngDev->mapSessions.erase(session);
	} catch(std::out_of_range&) {
		throw IrioNotInitializedError();
	}
}
