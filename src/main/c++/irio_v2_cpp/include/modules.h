#pragma once

#include <vector>
#include <unordered_map>
#include <cmath>

#include "errorsIrio.h"

namespace iriov2{

/**
 * Possible types of modules
 *
 * @ingroup Modules
 */
enum class ModulesType: std::uint32_t{
	NoModule = 0,                /**< No module connected or the board */
    FlexRIO_NI5761=0x109374C6,/**< FlexRIO_NI5761 */
    FlexRIO_NI5781=0x1093748A,/**< FlexRIO_NI5781 */
    FlexRIO_NI6581=0x10937418,/**< FlexRIO_NI6581 */
    FlexRIO_NI5734=0x10937595,/**< FlexRIO_NI5734 */
	cRIO_NI92xx=1,               /**< cRIO_NI92xx */
};

/**
 * Coupling AI mode
 *
 * @ingroup Modules
 */
enum class CouplingMode: std::uint8_t{
    None = 0,/**< None */
    AC,      /**< AC */
    DC       /**< DC */
};

/**
 * Manage conversion information for the ADC/DAC of a module
 *
 * @ingroup Modules
 */
class Module{
public:
	/**
	 * Constructor.
	 * Set parameters for all supported coupling modes
	 * and identifies the module
	 *
	 * @param id	Module identifier
	 */
    Module(const ModulesType &id = ModulesType::NoModule);

    /**
     * Sets a new coupling mode, changing the conversion constants
     * accordingly
     *
     * @throw iriov2::errors::UnsupportedAICouplingForModule	The \p mode is not supported by the module
     *
     * @param mode	New mode to configure
     */
    void setCouplingMode(const CouplingMode &mode);

    /**
     * Returns the actual configured coupling mode
     *
     * @return	Actual coupling mode
     */
    CouplingMode getCouplingMode() const;

    /**
     * Conversion to Volts of analog inputs
     *
     * @return Constant for Conversion to Volts of analog inputs
     */
    double getCVADC() const;

    /**
     * Conversion from Volts for analog outputs
     *
     * @return Constant for conversion from Volts for analog outputs
     */
    double getCVDAC() const;

    /**
     * Maximum value to be written in an analog output
     *
     * @return Maximum value to be written in an analog output
     */
    double getMaxValAO() const;

    /**
     * Minimum value to be written in an analog output
     *
     * @return Minimum value to be written in an analog output
     */
    double getMinValAO() const;

    /**
     * Module identifier
     */
    const ModulesType moduleID;

protected:
    /**
     * Parameters for conversion
     */
    struct ConfigParams
    {
    	/**
    	 * Conversion to Volts of analog inputs
    	 */
        double CVADC = 0;
        /**
         * Conversion from Volts for analog outputs
         */
        double CVDAC = 0;
        /**
         * Maximum value to be written in an analog output
         */
        double maxValAO = 0;
        /**
         * Minimum value to be written in an analog output
         */
        double minValAO = 0;

        /**
         * Default constructor. Sets all values to 0
         */
        ConfigParams() = default;

        /**
         * Initializes conversion parameters
         *
         * @param cvadc	Conversion to Volts of analog inputs
         * @param cvdac	Conversion from Volts for analog outputs
         * @param maxAO	Maximum value to be written in an analog output
         * @param minAO	Minimum value to be written in an analog output
         */
		ConfigParams(
				const double &cvadc,
				const double &cvdac,
				const double &maxAO,
				const double &minAO) :
				CVADC(cvadc), CVDAC(cvdac),
				maxValAO(maxAO),minValAO(minAO) {
		}
    };

    std::unordered_map<CouplingMode, const ConfigParams> availableConfigs;

private:
    CouplingMode m_couplingMode;
    ConfigParams m_currentConfig;
};

/**
 * Conversion parameters for FlexRIO Module NI5761
 *
 * @ingroup ModulesFlexRIO
 */
class ModuleNI5761: public Module{
public:
	/**
	 * Conversion parameters for FlexRIO Module NI5761
	 */
    ModuleNI5761();

private:
    const ConfigParams m_configAC{1.035/8191, 8191/1.035, 1.0, -1.0};
    const ConfigParams m_configDC{0.635/8191, 8191/0.635, 0.635, -0.635};
};

/**
 * Conversion parameters for FlexRIO Module NI6581
 *
 * @ingroup ModulesFlexRIO
 */
class ModuleNI6581: public Module{
public:
	/**
	 * Conversion parameters for FlexRIO Module NI6581
	 */
    ModuleNI6581();

private:
    const ConfigParams m_configNone{1.0, 1.0, 1.0, -1.0};
};

/**
 * Conversion parameters for FlexRIO Module NI5734
 *
 * @ingroup ModulesFlexRIO
 */
class ModuleNI5734: public Module{
public:
	/**
	 * Conversion parameters for FlexRIO Module NI5734
	 */
    ModuleNI5734();

private:
    const ConfigParams m_configNone{2.0/65536, 65536/2.0, 1.0, -1.0};
};

/**
 * Conversion parameters for FlexRIO Module NI5781
 *
 * @ingroup ModulesFlexRIO
 */
class ModuleNI5781: public Module{
public:
	/**
	 * Conversion parameters for FlexRIO Module NI5781
	 */
    ModuleNI5781();

private:
    const ConfigParams m_configDC{2.0/16384, 65536.0, 0.5, -0.5};
};

/**
 * Conversion parameters for cRIO Module 9205/9264
 *
 * @ingroup ModulesCRIO
 */
class ModuleNI92xx: public Module {
public:
	/**
	 * Conversion parameters for cRIO Module 9205/9264
	 */
    ModuleNI92xx();

private:
    const ConfigParams m_configDC{std::pow(2,-21), std::pow(2,15), 10, -10};
};

}
