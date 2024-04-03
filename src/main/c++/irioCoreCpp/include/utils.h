#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <NiFpga.h>

#include "bfp.h"
#include "errorsIrio.h"

namespace iriov2 {

namespace utils {
/**
 * Throws an exception if the NiFpga_Status is not success.
 * The exception message includes the specified text along with the error code
 *
 * @throw iriov2::errors::NiFpgaError	Status is not NiFpga_Status_Success
 *
 * @param status	Status to check
 * @param errMsg	Error message to use in the exception if there has been an error
 */
void throwIfNotSuccessNiFpga(const NiFpga_Status &status,
		const std::string &errMsg = "");

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
		const std::uint32_t n, const std::string &resourceName);

/**
 * Returns the base name of a given path.
 * 
 * @param path	The path from which to extract the base name.
 * @return The base name of the path.
 */
std::string getBaseName(const std::string& path);

/**
 * Returns a string representation of the current timestamp.
 *
 * @return A string representing the current timestamp.
 */
std::string getTimestamp();

/**
 * Converts an enum class to its underlying type.
 *
 * Helpful for switches when the value
 * may not be inside the valid values for the enum
 *
 * @tparam E Enum class
 * @param e	Enum class attribute to convert
 * @return	Converted value to underlying type
 */
template<typename E>
constexpr auto enum2underlying(E e) -> typename std::underlying_type<E>::type {
	return static_cast<typename std::underlying_type<E>::type>(e);
}

}  // namespace utils
}  // namespace iriov2
