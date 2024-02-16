#include <profiles/profileBase.h>

namespace iriov2 {
ProfileBase::ProfileBase(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const ProfileBase::PROFILE_ID &id) :
		ProfileBaseTerminals(parsedBitfile, session), profileID(id) {
}

const std::shared_ptr<const TerminalsAnalog> ProfileBase::analog() const {
	throw std::runtime_error("Analog terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsDigital> ProfileBase::digital() const {
	throw std::runtime_error("Digital terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsAuxAnalog> ProfileBase::auxAnalog() const {
	throw std::runtime_error("Aux Analog terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsAuxDigital> ProfileBase::auxDigital() const {
	throw std::runtime_error("Aux Digital terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalscRIO> ProfileBase::cRIO() const {
	throw std::runtime_error("cRIO terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsFlexRIO> ProfileBase::flexRIO() const {
	throw std::runtime_error("FlexRIO terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsSignalGeneration> ProfileBase::signalGeneration() const {
	throw std::runtime_error("Signal Generation terminals not enabled for this profile");
}

}
