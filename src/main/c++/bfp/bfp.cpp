#include "bfp.h"

#include <pugixml.hpp>
#include <iostream>

std::unordered_map<std::string, bfp::Register> parseRegisters(const pugi::xml_node& node, const std::uint32_t& baseAddress);
std::unordered_map<std::string, bfp::DMA> parseDMA(const pugi::xml_node& node);

namespace bfp{

BFP::BFP(const std::string& bitfile):
	m_bitfilePath(bitfile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result resParse = doc.load_file(bitfile.c_str());

	if(resParse.status != 0){
		const std::string msg = "Unable to parse '" + bitfile + "' ("
				+ std::string(resParse.description()) + ")";
		throw std::runtime_error(msg);
	}

	try{
		m_signature = doc.select_node("/Bitfile/SignatureRegister").node().text().as_string();
		m_baseAddress = doc.select_node("//NiFpga/BaseAddressOnDevice").node().text().as_uint();

		m_regMap = parseRegisters(doc.select_node("/Bitfile/VI/RegisterList").node(), m_baseAddress);
		m_dmaMap = parseDMA(doc.select_node("/Bitfile/Project//DmaChannelAllocationList").node());
	}catch(pugi::xpath_exception&){
		const std::string msg = "Problem while parsing bitfile '" + bitfile + "'";
		throw std::runtime_error(msg);
	}
}

std::string BFP::getBitfilePath() const{
	return m_bitfilePath;
}

std::unordered_map<std::string, Register> BFP::getRegisters() const{
	return m_regMap;
}

Register BFP::getRegister(const std::string& registerName) const{
	return m_regMap.at(registerName);
}

std::unordered_map<std::string, DMA> BFP::getDMAs() const{
	return m_dmaMap;
}

DMA BFP::getDMA(const std::string& dmaName) const{
	return m_dmaMap.at(dmaName);
}

std::string BFP::getSignature() const{
	return m_signature;
}

std::uint32_t BFP::getBaseAddress() const{
	return m_baseAddress;
}

}


std::unordered_map<std::string, bfp::Register> parseRegisters(const pugi::xml_node& node, const std::uint32_t& baseAddress){
	std::unordered_map<std::string, bfp::Register> mapRet;

	for(const auto& regNode: node.children("Register")){
		//Skip if internal register
		if(regNode.child("Internal").text().as_bool())
			continue;

		bfp::Register aux = bfp::processRegister(regNode, baseAddress);
		//Skip unsupported types
		if(aux.elemType != bfp::ElemTypes::Unsupported){
			mapRet.insert({aux.name, aux});
		}else{
			std::cerr << "WARNING: Skipping register " << aux.name << ". Unsupported type." << std::endl;
		}
	}

	return mapRet;
}


std::unordered_map<std::string, bfp::DMA> parseDMA(const pugi::xml_node& node){
	std::unordered_map<std::string, bfp::DMA> mapRet;

	for(const auto& dmaNode: node.children("Channel")){
		bfp::DMA aux = bfp::processDMA(dmaNode);
		mapRet.insert({aux.name, aux});
	}

	return mapRet;
}
