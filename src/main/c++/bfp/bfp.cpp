#include <iostream>
#include <algorithm>
#include <pugixml.hpp>

#include "bfp.h"
#include "errorsIrio.h"

namespace irio {
namespace bfp {

inline std::string removeSpaces(const std::string &s) {
	std::string aux = s;
	aux.erase(remove_if(aux.begin(), aux.end(), isspace), aux.end());
	return aux;
}

std::unordered_map<std::string, bfp::Register> parseRegisters(
		const pugi::xml_node &node, const std::uint32_t &baseAddress,
		const bool warnUnsupported) {
	std::unordered_map<std::string, bfp::Register> mapRet;

	for (const auto &regNode : node.children("Register")) {
		// Skip if internal register
		if (regNode.child("Internal").text().as_bool())
			continue;

		bfp::Register aux = bfp::Register::processRegister(regNode,
				baseAddress);
		// Skip unsupported types
		if (aux.getElemType() != bfp::ElemTypes::Unsupported) {
			std::string name = aux.getName();
			name = removeSpaces(name);
			mapRet.insert({ name, aux });
		} else if (warnUnsupported) {
			std::cerr << "WARNING: Skipping register " << aux.getName()
					<< ". Unsupported type." << std::endl;
		}
	}

	return mapRet;
}

std::unordered_map<std::string, bfp::DMA> parseDMA(const pugi::xml_node &node) {
	std::unordered_map<std::string, bfp::DMA> mapRet;

	for (const auto &dmaNode : node.children("Channel")) {
		bfp::DMA aux = bfp::DMA::processDMA(dmaNode);
		std::string name = aux.getName();
		name = removeSpaces(name);
		mapRet.insert({ name, aux });
	}

	return mapRet;
}

BFP::BFP(const std::string &bitfile, const bool warnUnsupported) :
		m_bitfilePath(bitfile) {
	pugi::xml_document doc;
	pugi::xml_parse_result resParse = doc.load_file(bitfile.c_str());

	if (resParse.status != 0) {
		throw errors::BFPParseBitfileError(bitfile, resParse.description());
	}

	try {
		m_signature =
				doc.select_node("/Bitfile/SignatureRegister").node().text().as_string();
		m_baseAddress =
				doc.select_node("//NiFpga/BaseAddressOnDevice").node().text().as_uint();
		m_bitfileVersion =
				doc.select_node("/Bitfile/BitfileVersion").node().text().as_string();

		m_regMap = parseRegisters(
				doc.select_node("/Bitfile/VI/RegisterList").node(),
				m_baseAddress, warnUnsupported);
		m_dmaMap = parseDMA(
				doc.select_node("/Bitfile/Project//DmaChannelAllocationList").node());
	} catch (pugi::xpath_exception &e) {
		throw errors::BFPParseBitfileError(bitfile, e.what());
	}
}

std::string BFP::getBitfilePath() const {
	return m_bitfilePath;
}

std::string BFP::getBitfileVersion() const {
	return m_bitfileVersion;
}

std::unordered_map<std::string, Register> BFP::getRegisters() const {
	return m_regMap;
}

Register BFP::getRegister(const std::string &registerName) const {
	try {
		return m_regMap.at(registerName);
	} catch (std::out_of_range&) {
		throw errors::ResourceNotFoundError(registerName + " not found");
	}
}

std::unordered_map<std::string, DMA> BFP::getDMAs() const {
	return m_dmaMap;
}

DMA BFP::getDMA(const std::string &dmaName) const {
	try {
		return m_dmaMap.at(dmaName);
	} catch (std::out_of_range&) {
		throw errors::ResourceNotFoundError(dmaName + " not found");
	}
}

std::string BFP::getSignature() const {
	return m_signature;
}

}  // namespace bfp
}  // namespace iriov2
