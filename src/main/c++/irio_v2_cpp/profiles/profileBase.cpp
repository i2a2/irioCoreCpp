#include <profiles/profileBase.h>
#include <errorsIrio.h>

namespace iriov2 {

ProfileBase::ProfileBase(const PROFILE_ID &id) :
		profileID(id) {
}

TerminalsAnalog ProfileBase::analog() const {
	throw errors::TerminalNotImplementedError(
			"Analog terminals not enabled for this profile");
}

TerminalscRIO ProfileBase::cRIO() const {
	throw errors::TerminalNotImplementedError(
			"cRIO terminals not enabled for this profile");
}

TerminalsFlexRIO ProfileBase::flexRIO() const {
	throw errors::TerminalNotImplementedError(
			"FlexRIO terminals not enabled for this profile");
}

TerminalsSignalGeneration ProfileBase::signalGeneration() const {
	throw errors::TerminalNotImplementedError(
			"Signal Generation terminals not enabled for this profile");
}

TerminalsDMADAQ ProfileBase::daq() const {
	throw errors::TerminalNotImplementedError(
			"DMA DAQ terminals not enabled for this profile");
}

}  // namespace iriov2
