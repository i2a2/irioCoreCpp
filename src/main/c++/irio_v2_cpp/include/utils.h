#pragma once

#include <string>
#include <bfp.h>
#include <NiFpga.h>
#include <unordered_map>
#include <functional>
#include <errorsIrio.h>

namespace iriov2{

namespace utils{
/**
 * Throws an exception if the NiFpga_Status is not success.
 * The exception message includes the specified text along with the error code
 *
 * @throw iriov2::errors::NiFpgaError	Status is not NiFpga_Status_Success
 *
 * @param status	Status to check
 * @param errMsg	Error message to use in the exception if there has been an error
 */
void throwIfNotSuccessNiFpga(const NiFpga_Status &status, const std::string &errMsg = "");

/**
 * Find resources in an specified map that follow the structure name<n>, where n is a number,
 * and insert them in a new map.
 *
 * The function will search maxElem resources, if one is not found, it will continue.
 *
 * For example:\n
 * Given the resourceName "AI" and maxElem 3, if there is AI0 and AI2, although
 * AI1 would not be found, the function will insert AI2 anyway.
 *
 * @tparam T			bfp::Register or bfp::DMA
 * @param mapSearch		Map with the resources to find
 * @param resourceName	Base name of the resource
 * @param maxElem		Maximum number of elements that could be found
 * @param mapInsert		Map on which to insert the address of found resources
 */
template<typename T>
void findAndInsertEnumResources(
		const std::unordered_map<std::string, T> &mapSearch,
		const std::string &resourceName,
		const size_t maxElem,
		std::unordered_map<std::uint32_t, const std::uint32_t> &mapInsert);

/**
 * Wrapper function for \ref findAndInsertEnumResources specific for Registers
 *
 * @param parsedBitfile		Parsed bitfile with the Registers to search
 * @param terminalName		Base name of the register
 * @param maxElem			Maximum number of Registers that could be found
 * @param mapInsert			Map on which to insert the address of the Registers found
 */
void findAndInsertEnumRegisters(
		const bfp::BFP &parsedBitfile,
		const std::string &terminalName,
		const size_t maxElem,
		std::unordered_map<std::uint32_t, const std::uint32_t> &mapInsert);

/**
 * Wrapper function for \ref findAndInsertEnumResources specific for DMAs
 *
 * @param parsedBitfile		Parsed bitfile with the DMA to search
 * @param dmaName			Base name of the DMA
 * @param maxElem			Maximum number of DMAs that could be found
 * @param mapInsert			Map on which to insert the address of the DMAs found
 */
void findAndInsertEnumDMAs(
		const bfp::BFP &parsedBitfile,
		const std::string &dmaName,
		const size_t maxElem,
		std::unordered_map<std::uint32_t, const std::uint32_t> &mapInsert);

/**
 * Searches a map with identifiers as keys and addresses as values and check if the specified identifier (n) exists.
 *
 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
 *
 * @param mapResource	Map with the identifiers as keys and addresses as values
 * @param n				Identifier to find in map
 * @param resourceName	Name of the resource to find
 * @return	Address of the specified enum resource
 */
std::uint32_t getAddressEnumResource(
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapResource,
		const std::uint32_t n,
		const std::string &resourceName);

/**
 * Searches for an array register and reads its contents to a vector
 *
 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
 *
 * @tparam T	The type of the array elements
 * @param parsedBitfile	Parsed bitfile with the array register to search
 * @param session		NiFpga_Session to be used in NiFpga
 *  					related functions
 * @param nameReg		Name of the array register
 * @param vec			Vector to write the elements
 * @param readFunc		NiFpga_ReadArray* function to be used for reading the register
 */
template<typename T>
void findArrayRegReadToVector(const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const std::string &nameReg, std::vector<T> &vec,
		std::function<NiFpga_Status(NiFpga_Session, std::uint32_t, T*, size_t)> readFunc){
	const auto auxMap = parsedBitfile.getRegisters();
	const auto it = auxMap.find(nameReg);
	if(it == auxMap.end()){
		throw errors::ResourceNotFoundError(nameReg + " not found");
	}

	const auto reg = parsedBitfile.getRegister(nameReg);
	vec.resize(reg.numElem);
	const auto status = readFunc(session, reg.address, vec.data(), vec.size());
	utils::throwIfNotSuccessNiFpga(status, "Error reading " + nameReg);
}

}
}
