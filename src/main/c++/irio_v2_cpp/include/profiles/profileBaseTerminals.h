#pragma once

#include <bfp.h>
#include <NiFpga.h>

namespace iriov2 {

class ProfileBaseTerminals {
public:
	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param parsedBitfile
	 * @param session
	 */
	ProfileBaseTerminals(const bfp::BFP &parsedBitfile, const NiFpga_Session &session);

	std::pair<std::uint8_t, std::uint8_t> getFPGAVIversion() const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return
	 */
	bool getInitDone() const;
	std::uint32_t getFref() const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return
	 */
	std::uint8_t getDevQualityStatus() const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return
	 */
	std::int16_t getDevTemp() const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return
	 */
	bool getDAQStartStop() const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return
	 */
	bool getDebugMode() const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 */
	void setDAQStart() const;

	/**
	 *
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 */
	void setDAQStop() const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param start
	 */
	void setDAQStartStop(const bool &start) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param debug
	 */
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

