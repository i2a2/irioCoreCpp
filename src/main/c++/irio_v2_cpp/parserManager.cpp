#include <iostream>

#include "errorsIrio.h"
#include "parserManager.h"

namespace  iriov2 {

ParserManager::ParserManager(const bfp::BFP &bfp) : m_bfp(bfp),
		m_error(false) { }

bool ParserManager::findRegister(const std::string &resourceName,
								const GroupResource &group,
								bfp::Register *reg,
								const bool optional) {
	try {
		*reg = m_bfp.getRegister(resourceName);
		logResourceFound(resourceName, group);
	} catch (errors::ResourceNotFoundError &) {
		if(!optional)
			logResourceNotFound(resourceName, group);
		return false;
	}

	return true;
}

bool ParserManager::findDMA(const std::string &resourceName,
								const GroupResource &group,
								bfp::DMA *dma,
								const bool optional) {
	try {
		*dma = m_bfp.getDMA(resourceName);
		logResourceFound(resourceName, group);
	} catch (errors::ResourceNotFoundError &) {
		if(!optional)
			logResourceNotFound(resourceName, group);
		return false;
	}

	return true;
}

bool ParserManager::findRegisterAddress(const std::string &resourceName,
									   const GroupResource &group,
									   std::uint32_t *address,
									   const bool optional) {
	bfp::Register reg;
	const auto ret = findRegister(resourceName, group, &reg, optional);
	*address = reg.getAddress();

	return ret;
}

bool ParserManager::findDMANum(const std::string &resourceName,
									   const GroupResource &group,
									   std::uint32_t *address,
									   const bool optional) {
	bfp::DMA dma;
	const auto ret = findDMA(resourceName, group, &dma, optional);
	*address = dma.getDMANumber();

	return ret;
}

void ParserManager::logResourceFound(const std::string &resourceName,
									const GroupResource &group) {
	const auto it = &m_groupInfo.emplace(group, GroupInfo()).first->second;
	it->found.emplace(resourceName);
}

void ParserManager::logResourceNotFound(const std::string &resourceName,
									   const GroupResource &group) {
	const auto it = &m_groupInfo.emplace(group, GroupInfo()).first->second;
	it->notFound.emplace(resourceName);
	m_error = true;
}

void ParserManager::logResourceError(const std::string &resourceName,
		const std::string &errMsg,
		const GroupResource &group) {
	const auto it = &m_groupInfo.emplace(group, GroupInfo()).first->second;
	it->error.emplace(resourceName, errMsg);
	m_error = true;
}

bool ParserManager::hasErrorOccurred() const {
	return m_error;
}

void ParserManager::printInfo(std::ostream &os) const {
	for(const auto& group : m_groupInfo) {
		const auto foundMap = &group.second.found;
		const auto notFoundMap = &group.second.notFound;
		const auto errorMap = &group.second.error;

		os << m_group2str.at(group.first) << ":" << std::endl;
		if (foundMap->size()) {
			os << "\tFound (" << foundMap->size() << " resources):" << std::endl
			   << "\t\t";
			for (const auto &found : *foundMap) {
				os << found << ", ";
			}
			os << "\b\b " << std::endl;
		}

		if (notFoundMap->size()) {
			os << "\tNot found:" << std::endl << "\t\t";
			for (const auto &notFound : *notFoundMap) {
				os << notFound << ", " << std::endl;
			}
			os << "\b\b " << std::endl;
		}

		if (errorMap->size()) {
			os << "\tError:" << std::endl << "\t\t";
			for (const auto &error : *errorMap) {
				os << error.errMsg << std::endl << "\t\t";
			}
			os << std::endl;
		}
	}
}

void ParserManager::printInfoError(std::ostream &os) const {
	for(const auto& group : m_groupInfo) {
		const auto notFoundMap = &group.second.notFound;
		const auto errorMap = &group.second.error;
		if(notFoundMap->size() || errorMap->size()) {
			os << m_group2str.at(group.first) << ":" << std::endl;

			if(notFoundMap->size()) {
				os << "\tNot found:" << std::endl << "\t\t";
				for(const auto& notFound : *notFoundMap) {
					os << notFound << ", " << std::endl;
				}
			}

			if(errorMap->size()) {
				os << "\tError:" << std::endl << "\t\t";
				for(const auto& error : *errorMap) {
					os << error.errMsg << std::endl << "\t\t";
				}
				os << std::endl;
			}
		}
	}
}

bool ParserManager::findRegisterEnumAddress(const std::string &resourceName,
		std::uint32_t nResource, const GroupResource &group,
		std::unordered_map<std::uint32_t, const std::uint32_t> *mapInsert,
		const bool optional) {
	std::uint32_t address;
	if (findRegisterAddress(resourceName + std::to_string(nResource), group,
			&address, optional)) {
		mapInsert->emplace(nResource, address);
		return true;
	} else {
		return false;
	}
}

bool ParserManager::findDMAEnumNum(const std::string &resourceName,
		std::uint32_t nResource, const GroupResource &group,
		std::unordered_map<std::uint32_t, const std::uint32_t> *mapInsert,
		const bool optional) {
	std::uint32_t address;
	if (findDMANum(resourceName + std::to_string(nResource), group,
			&address, optional)) {
		mapInsert->emplace(nResource, address);
		return true;
	} else {
		return false;
	}
}

void ParserManager::compareResourcesMap(
	const std::unordered_map<std::uint32_t, const std::uint32_t> &mapA,
	const std::string &nameTermA,
	const std::unordered_map<std::uint32_t, const std::uint32_t> &mapB,
	const std::string &nameTermB,
	const GroupResource &group) {
	// Check resources in mapA
    for (const auto& pair : mapA) {
        if (mapB.find(pair.first) == mapB.end()) {
			logResourceNotFound(nameTermB+std::to_string(pair.first), group);
        }
    }

    // Check resources in mapB
    for (const auto& pair : mapB) {
        if (mapA.find(pair.first) == mapA.end()) {
			logResourceNotFound(nameTermA+std::to_string(pair.first), group);
        }
    }
}

ResourceError::ResourceError(const std::string &resName,
		const std::string &msg) : resourceName(resName),
				errMsg(msg) { }

bool ResourceError::operator==(const ResourceError &other) const {
	 return (resourceName == other.resourceName) &&
			 (errMsg == other.errMsg);
}

bool CustomStringComparator::operator()(const std::string &a,
										const std::string &b) const {
	if (a.size() == b.size()) {
		return a < b;  // If sizes are equal, use lexicographical comparison
	}
	return a.size() < b.size();	 // Otherwise, shorter strings come first
}

}  // namespace iriov2
