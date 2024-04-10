#pragma once

#include <utility>
#include <string>

#include "irioCoreCpp.h"

class IrioNotInitializedError: public std::runtime_error {
 public:
	IrioNotInitializedError() :
			std::runtime_error("Irio driver not initialized") {
	}
};

class IrioInstanceManager {
 protected:
	IrioInstanceManager() = default;

 public:
	IrioInstanceManager(const IrioInstanceManager &other) = delete;

	void operator=(const IrioInstanceManager&) = delete;

	static std::pair<irio::Irio*, std::uint32_t> createInstance(
			const std::string &bitfilePath, const std::string &RIOSerialNumber,
			const std::string &FPGAVIversion, const bool verbose = true);

	static irio::Irio* getInstance(const std::string &RIOSerialNumber,
			const std::uint32_t session);

	static void destroyInstance(const std::string &RIOSerialNumber,
			const std::uint32_t session);
};

