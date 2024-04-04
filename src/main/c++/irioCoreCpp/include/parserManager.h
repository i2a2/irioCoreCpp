#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <iostream>

#include "bfp.h"

namespace irio {
/**
 * Represents an error that occurred while parsing a resource.
 */
struct ResourceError {
	std::string resourceName; /**< Resource name that caused the error. */
	std::string errMsg; /**< The error message. */

	/**
	 * Constructs a ResourceError object with the given resource name and error message.
	 * 
	 * @param resName The name of the resource that caused the error.
	 * @param msg The error message.
	 */
	ResourceError(const std::string& resName, const std::string& msg);

	/**
	 * Checks if this ResourceError is equal to another ResourceError.
	 * 
	 * @param other The other ResourceError to compare with.
	 * @return true if the ResourceErrors are equal, false otherwise.
	 */
	bool operator==(const ResourceError& other) const;
};

/**
 * Hash function for the ResourceError struct.
 */
struct ResourceErrorHash {
	/**
	 * Calculates the hash value for a ResourceError object.
	 * 
	 * @param info The ResourceError object to calculate the hash for.
	 * @return The calculated hash value.
	 */
	size_t operator()(const ResourceError& info) const;
};

/**
 * Possible groups of the parsed resources
 */
enum class GroupResource {
	Common,
	AI,
	AO,
	AuxAI,
	AuxAO,
	DI,
	DO,
	AuxDI,
	AuxDO,
	DMA,
	DAQ,
	IMAQ,
	IO,
	SG,
	CRIO,
	FlexRIO
};

/**
 * Custom string comparator for std::set.
 */
struct CustomStringComparator {
	/**
	 * Overloaded function call operator for comparing two strings.
	 *
	 * This function compares two strings, `a` and `b`, and returns true if `a`
	 * is less than `b` in lexicographical order. Otherwise, it returns false.
	 *
	 * @param a The first string to compare.
	 * @param b The second string to compare.
	 * @return true if `a` is less than `b`, false otherwise.
	 */
	bool operator()(const std::string& a, const std::string& b) const;
};

/**
 * Contains information about found, not found, and error resources for a group.
 */
struct GroupInfo {
	std::set<std::string, CustomStringComparator>
		found; /**< Set of found resources. */
	std::set<std::string, CustomStringComparator>
		notFound; /**< Set of not found resources. */
	std::unordered_set<ResourceError, ResourceErrorHash>
		error; /**< Set of resource errors. */

	GroupInfo() = default;
};

/**
 * Responsible for managing the parsing of resources for a bitfile.
 *
 * The ParserManager class provides methods for finding registers, DMAs,
 * register addresses and DMA numbers. It also logs which resources have been
 * found or not. It provides methods for logging resource incompatibilities and
 * printing the problems found.
 * 
 * @ingroup IRIO
 */
class ParserManager {
 public:
	/**
	 * Constructs a ParserManager object with the given BFP object.
	 * 
	 * @param bfp The BFP object to be used for parsing.
	 */
	explicit ParserManager(const bfp::BFP &bfp);

	/**
	 * Finds a register with the given resource name.
	 *
	 * @param resourceName The name of the resource.
	 * @param group The group of the resource. Used for logging.
	 * @param reg Pointer to store the found register.
	 * @param optional Flag indicating if the register is optional. If true and
	 * 					the resource is not found, it will not be logged.
	 * @return True if the register is found, false otherwise.
	 */
	bool findRegister(const std::string &resourceName,
					  const GroupResource &group, bfp::Register *reg,
					  const bool optional = false);

	/**
	 * Finds a DMA with the given resource name.
	 * 
	 * @param resourceName The name of the resource.
	 * @param group The group of the resource. Used for logging.
	 * @param dma Pointer to store the found DMA.
	 * @param optional Flag indicating if the DMA is optional. If true and
	 *					the resource is not found, it will not be logged.
	 * @return True if the DMA is found, false otherwise.
	 */
	bool findDMA(const std::string &resourceName,
				 const GroupResource &group, bfp::DMA *dma,
				 const bool optional = false);

	/**
	 * Finds the address of a register with the given resource name.
	 * 
	 * @param resourceName The name of the resource.
	 * @param group The group of the resource. Used for logging.
	 * @param address Pointer to store the found address.
	 * @param optional Flag indicating if the resource is optional. If true and
	 *					the resource is not found, it will not be logged.
	 * @return True if the address is found, false otherwise.
	 */
	bool findRegisterAddress(const std::string &resourceName,
							 const GroupResource &group, std::uint32_t *address,
							 const bool optional = false);

	/**
	 * Finds the DMA number of a resource with the given name.
	 * 
	 * @param resourceName The name of the resource.
	 * @param group The group of the resource. Used for logging.
	 * @param address Pointer to store the found DMA number.
	 * @param optional Flag indicating if the resource is optional. If true and
	 *					the resource is not found, it will not be logged.
	 * @return True if the DMA number is found, false otherwise.
	 */
	bool findDMANum(const std::string &resourceName,
					const GroupResource &group, std::uint32_t *address,
					const bool optional = false);

	/**
	 * Finds the address of a register with the given resource name. The register
	 * follows an enumeration naming convention.
	 * 
	 * Enumeration convention: resourceName<n> where n is a number
	 * 
	 * @param resourceName The name of the resource.
	 * @param nResource The number of the resource.
	 * @param group The group of the resource.
	 * @param mapInsert Pointer to the enumeration map.
	 * @param optional Flag indicating if the address is optional.
	 * @return True if the address is found, false otherwise.
	 */
	bool findRegisterEnumAddress(const std::string &resourceName,
								 std::uint32_t nResource, const GroupResource &group,
								 std::unordered_map<std::uint32_t, const std::uint32_t> *mapInsert,
								 const bool optional = false);

	/**
	 * Finds the DMA number of a DMA with the given name, The resource
	 * follows an enumeration naming convention.
	 * 
	 * Enumeration convention: resourceName<n> where n is a number
	 * 
	 * @param resourceName The name of the resource.
	 * @param nResource The number of the resource.
	 * @param group The group of the resource.
	 * @param mapInsert Pointer to the enumeration map.
	 * @param optional Flag indicating if the DMA number is optional.
	 * @return True if the DMA number is found, false otherwise.
	 */
	bool findDMAEnumNum(const std::string &resourceName,
						std::uint32_t nResource, const GroupResource &group,
						std::unordered_map<std::uint32_t, const std::uint32_t> *mapInsert,
						const bool optional = false);

	/**
	 * Compares two resource maps and logs any differences.
	 * 
	 * @param mapA The first resource map.
	 * @param nameTermA The name of the first resource map.
	 * @param mapB The second resource map.
	 * @param nameTermB The name of the second resource map.
	 * @param group The group of the resources.
	 */
	void compareResourcesMap(
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapA,
		const std::string &nameTermA,
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapB,
		const std::string &nameTermB,
		const GroupResource &group);

	/**
	 * Logs an error related to a resource.
	 * 
	 * @param resourceName The name of the resource.
	 * @param errMsg The error message.
	 * @param group The group of the resource.
	 */
	void logResourceError(const std::string &resourceName,
						  const std::string &errMsg,
						  const GroupResource &group);

	/**
	 * Checks if an error has occurred during parsing.
	 * @return True if an error has occurred, false otherwise.
	 */
	bool hasErrorOccurred() const;

	/**
	 * Prints resources found, not found and incompatibilities
	 * @param os The output stream to print the information to.
	 */
	void printInfo(std::ostream &os = std::cout) const;

	/**
	 * Prints resources not found and incompatibilities
	 * @param os The output stream to print the error information to.
	 */
	void printInfoError(std::ostream &os = std::cerr) const;

	/**
	 * Prints resources found, not found and incompatibilities in XML format.
	 * @param filename The name of the XML file to write the information to.
	 */
	void printInfoXML(const std::string& filename) const;

 private:
	/**
	 * Logs that a resource has been found.
	 * @param resourceName The name of the resource.
	 * @param group The group of the resource.
	 */
	void logResourceFound(const std::string &resourceName,
						  const GroupResource &group);

	/**
	 * Logs that a resource has not been found.
	 * @param resourceName The name of the resource.
	 * @param group The group of the resource.
	 */
	void logResourceNotFound(const std::string &resourceName,
							 const GroupResource &group);

	/// The BFP object used by the parser manager.
	bfp::BFP m_bfp;
	/// Map to divide information of found resources per group
	std::unordered_map<GroupResource, GroupInfo> m_groupInfo;
	/// True if some resource was not found
	bool m_error;

	/// Convert GroupResource to string
	const std::unordered_map<GroupResource, std::string> m_group2str = {
		{GroupResource::Common, "Common"},
		{GroupResource::AI, "AI"},
		{GroupResource::AO, "AO"},
		{GroupResource::AuxAI, "AuxAI"},
		{GroupResource::AuxAO, "AuxAO"},
		{GroupResource::DI, "DI"},
		{GroupResource::DO, "DO"},
		{GroupResource::AuxDI, "AuxDI"},
		{GroupResource::AuxDO, "AuxDO"},
		{GroupResource::DMA, "DMA"},
		{GroupResource::DAQ, "DAQ"},
		{GroupResource::IMAQ, "IMAQ"},
		{GroupResource::IO, "IO"},
		{GroupResource::SG, "SG"},
		{GroupResource::CRIO, "CRIO"},
		{GroupResource::FlexRIO, "FlexRIO"},
	};
};

}  // namespace irio



