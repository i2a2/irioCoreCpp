#include <sys/stat.h>
#include <iostream>
#include <pugixml.hpp>

#include "errorsIrio.h"
#include "parserManager.h"


namespace {
	/// Convert GroupResource to string
	const std::unordered_map<irio::GroupResource, std::string> group2str = {
		{irio::GroupResource::Common, "Common"},
		{irio::GroupResource::AI, "AI"},
		{irio::GroupResource::AO, "AO"},
		{irio::GroupResource::AuxAI, "AuxAI"},
		{irio::GroupResource::AuxAO, "AuxAO"},
		{irio::GroupResource::DI, "DI"},
		{irio::GroupResource::DO, "DO"},
		{irio::GroupResource::AuxDI, "AuxDI"},
		{irio::GroupResource::AuxDO, "AuxDO"},
		{irio::GroupResource::DMA, "DMA"},
		{irio::GroupResource::DAQ, "DAQ"},
		{irio::GroupResource::IMAQ, "IMAQ"},
		{irio::GroupResource::IO, "IO"},
		{irio::GroupResource::SG, "SG"},
		{irio::GroupResource::CRIO, "CRIO"},
		{irio::GroupResource::FlexRIO, "FlexRIO"},
	};
}  // namespace

namespace irio {

ParserManager::ParserManager(const bfp::BFP &bfp) : m_bfp(bfp) {}

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

void ParserManager::printInfo(std::ostream &os, const bool onlyErrors) const {
	for (const auto &group : m_groupInfo) {
		const auto foundMap = &group.second.found;
		const auto notFoundMap = &group.second.notFound;
		const auto errorMap = &group.second.error;

		if ((foundMap->size() && !onlyErrors) || notFoundMap->size() ||
			errorMap->size()) {
			os << group2str.at(group.first) << ":" << std::endl;
		}
		if (foundMap->size() && !onlyErrors) {
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
				os << notFound << ", ";
			}
			os << "\b\b " << std::endl;
		}

		if (errorMap->size()) {
			os << "\tError:" << std::endl << "\t\t";
			for (const auto &error : *errorMap) {
				os << error.first << ": " << error.second
					<< std::endl << "\t\t";
			}
			os << std::endl;
		}
	}
}

void ParserManager::printInfoError(std::ostream &os) const {
	for (const auto &group : m_groupInfo) {
		const auto notFoundMap = &group.second.notFound;
		const auto errorMap = &group.second.error;
		if (notFoundMap->size() || errorMap->size()) {
			printInfo(os, true);
		}
	}
}

void ParserManager::printInfoXML(const std::string& filename) const {
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("irioCore");

	for (const auto &group : m_groupInfo) {
		const auto foundMap = &group.second.found;
		const auto notFoundMap = &group.second.notFound;
		const auto errorMap = &group.second.error;

		pugi::xml_node groupNode = root.append_child("Group");
		groupNode.append_attribute("name") =
			group2str.at(group.first).c_str();

		pugi::xml_node foundNode = groupNode.append_child("Found");
		if (foundMap->size()) {
			foundNode.append_attribute("count") = foundMap->size();
			for (const auto &found : *foundMap) {
				pugi::xml_node itemNode = foundNode.append_child("Resource");
				itemNode.text().set(found.c_str());
			}
		}

		pugi::xml_node notFoundNode = groupNode.append_child("NotFound");
		if (notFoundMap->size()) {
			notFoundNode.append_attribute("count") = notFoundMap->size();
			for (const auto &notFound : *notFoundMap) {
				pugi::xml_node itemNode = notFoundNode.append_child("Resource");
				itemNode.text().set(notFound.c_str());
			}
		}

		pugi::xml_node errorNode = groupNode.append_child("Error");
		if (errorMap->size()) {
			errorNode.append_attribute("count") = errorMap->size();
			for (const auto &error : *errorMap) {
				pugi::xml_node itemNode = errorNode.append_child("Resource");
				itemNode.text().set(error.second.c_str());
			}
		}
	}

	std::string directory(filename);
    std::size_t lastSlash = directory.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        directory = directory.substr(0, lastSlash);
    }

    // Create the directory if it doesn't exist
    struct stat st;
    if (stat(directory.c_str(), &st) == -1) {
		mkdir(directory.c_str(), 0777);
    }

	if (doc.save_file(filename.c_str())) {
		std::cout << "Parse log created: " << filename << std::endl;
	} else {
		std::cerr << "Error: Could not save XML to file" << std::endl;
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

bool CustomStringComparator::operator()(const std::string &a,
										const std::string &b) const {
	if (a.size() == b.size()) {
		return a < b;  // If sizes are equal, use lexicographical comparison
	}
	return a.size() < b.size();	 // Otherwise, shorter strings come first
}

GroupInfo::GroupInfo() = default;

}  // namespace irio
