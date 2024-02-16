#include <profiles/profileBase.h>

namespace iriov2 {
ProfileBase::ProfileBase(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const ProfileBase::PROFILE_ID &id) :
		ProfileBaseTerminals(parsedBitfile, session), profileID(id) {
}

const std::shared_ptr<const TerminalsAnalog> ProfileBase::analog() {
	throw std::runtime_error("Analog terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsDigital> ProfileBase::digital() {
	throw std::runtime_error("Digital terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsAuxAnalog> ProfileBase::auxAnalog() {
	throw std::runtime_error("Aux Analog terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsAuxDigital> ProfileBase::auxDigital() {
	throw std::runtime_error("Aux Digital terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalscRIO> ProfileBase::cRIO() {
	throw std::runtime_error("cRIO terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsFlexRIO> ProfileBase::flexRIO() {
	throw std::runtime_error("FlexRIO terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsSignalGeneration> ProfileBase::signalGeneration() {
	throw std::runtime_error("Signal Generation terminals not enabled for this profile");
}

}
