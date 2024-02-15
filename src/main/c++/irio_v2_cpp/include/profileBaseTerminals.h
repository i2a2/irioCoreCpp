#pragma once

#include <bfp.h>
#include <NiFpga.h>


namespace iriov2{

class ProfileBaseTerminals
{
public:
	ProfileBaseTerminals(const bfp::BFP &parsedBitfile, const NiFpga_Session &session);

	std::pair<std::uint8_t, std::uint8_t> getFPGAVIversion() const;
	bool getInitDone() const;
	std::uint32_t getFref() const;
	std::uint8_t getDevQualityStatus() const;
	std::int16_t getDevTemp() const;
	bool getDAQStartStop() const;
	bool getDebugMode() const;

	void setDAQStart() const;
	void setDAQStop() const;
	void setDAQStartStop(const bool &start) const;
	void setDebugMode(const bool &debug) const;

private:
	std::pair<std::uint8_t, std::uint8_t> m_fpgaviversion;
	std::uint32_t m_fref;

	std::uint32_t m_initdone_addr;
	std::uint32_t m_devqualitystatus_addr;
	std::uint32_t m_devtemp_addr;
	std::uint32_t m_daqstartstop_addr;
	std::uint32_t m_debugmode_addr;

	NiFpga_Session m_session;
};

}

