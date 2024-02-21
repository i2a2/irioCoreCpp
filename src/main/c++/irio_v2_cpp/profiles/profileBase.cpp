#include <profiles/profileBase.h>
#include <errorsIrio.h>

namespace iriov2 {
ProfileBase::ProfileBase(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const ProfileBase::PROFILE_ID &id) :
		ProfileBaseTerminals(parsedBitfile, session), profileID(id) {
}

const std::shared_ptr<const TerminalsAnalog> ProfileBase::analog() const {
	throw errors::TerminalNotImplementedError("Analog terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsDigital> ProfileBase::digital() const {
	throw errors::TerminalNotImplementedError("Digital terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsAuxAnalog> ProfileBase::auxAnalog() const {
	throw errors::TerminalNotImplementedError("Aux Analog terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsAuxDigital> ProfileBase::auxDigital() const {
	throw errors::TerminalNotImplementedError("Aux Digital terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalscRIO> ProfileBase::cRIO() const {
	throw errors::TerminalNotImplementedError("cRIO terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsFlexRIO> ProfileBase::flexRIO() const {
	throw errors::TerminalNotImplementedError("FlexRIO terminals not enabled for this profile");
}

const std::shared_ptr<const TerminalsSignalGeneration> ProfileBase::signalGeneration() const {
	throw errors::TerminalNotImplementedError("Signal Generation terminals not enabled for this profile");
}

}
