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

void ParserManager::logResourceMismatch(const std::string &resourceName,
									   const std::string &relatedResource,
									   const GroupResource &group) {
	const auto it = &m_groupInfo.emplace(group, GroupInfo()).first->second;
	it->mismatch.emplace(relatedResource, resourceName);
	m_error = true;
}

bool ParserManager::hasErrorOccurred() const {
	return m_error;
}

void ParserManager::printInfo() const {
	for(const auto& group : m_groupInfo) {
		std::cout << m_group2str.at(group.first) << ":" << std::endl;
		std::cout << "\tFound:" << std::endl << "\t\t";
		for(const auto& found : group.second.found) {
			std::cout << found << ", ";
		}
		std::cout << std::endl;
		std::cout << "\tNot found:" << std::endl << "\t\t";
		for(const auto& notFound : group.second.notFound) {
			std::cout << notFound << ", " << std::endl;
		}
		std::cout << std::endl;
		std::cout << "\tMismatch:" << std::endl << "\t\t";
		for(const auto& mismatch : group.second.mismatch) {
			std::cout << "Resource " << mismatch.resourceNotMissing
					<< " found when resource " << mismatch.resourceMissing
					<< " either does not exist or its value does not match"
					<< " the number of resources found." << std::endl << "\t\t";
		}
		std::cout << std::endl;
	}
}

void ParserManager::printInfoError() const {
	for(const auto& group : m_groupInfo) {
		const auto notFoundMap = &group.second.notFound;
		const auto mismatchMap = &group.second.mismatch;
		if(notFoundMap->size() || mismatchMap->size()) {
			std::cout << m_group2str.at(group.first) << ":" << std::endl;

			if(notFoundMap->size()) {
				std::cout << "\tNot found:" << std::endl << "\t\t";
				for(const auto& notFound : *notFoundMap) {
					std::cout << notFound << ", " << std::endl;
				}
			}

			if(mismatchMap->size()) {
				std::cout << "\tMismatch:" << std::endl << "\t\t";
				for(const auto& mismatch : *mismatchMap) {
					std::cout << "Resource " << mismatch.resourceNotMissing
							<< " found when resource " << mismatch.resourceMissing
							<< " either does not exist or its value does not match"
							<< " the number of resources found." << std::endl << "\t\t";
				}
				std::cout << std::endl;
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


MismatchInfo::MismatchInfo(const std::string &resMissing,
		const std::string &resNotMissing) : resourceMissing(resMissing),
				resourceNotMissing(resNotMissing) { }

bool MismatchInfo::operator==(const MismatchInfo &other) const {
	 return (resourceMissing == other.resourceMissing) &&
			 (resourceNotMissing == other.resourceNotMissing);
}


}  // namespace iriov2
