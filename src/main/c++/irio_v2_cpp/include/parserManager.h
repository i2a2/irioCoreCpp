#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "bfp.h"

namespace iriov2 {
struct MismatchInfo {
	std::string resourceMissing;
	std::string resourceNotMissing;

	MismatchInfo(const std::string &resMissing,
				const std::string &resNotMissing);
	bool operator==(const MismatchInfo &other) const;
};
}


template<>
struct std::hash<iriov2::MismatchInfo> {
	size_t operator()(const iriov2::MismatchInfo& info) const {
		size_t hash1 = std::hash<std::string>{}(info.resourceMissing);
		size_t hash2 = std::hash<std::string>{}(info.resourceNotMissing);
		return hash1 ^ (hash2 << 1);  // Combining hashes
	}
};

namespace iriov2 {

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


struct GroupInfo {
	std::unordered_set<std::string> found;
	std::unordered_set<std::string> notFound;
	std::unordered_set<MismatchInfo> mismatch;

	GroupInfo() = default;
};

class ParserManager {
 public:
	explicit ParserManager(const bfp::BFP &bfp);

	bool findRegister(const std::string &resourceName,
			const GroupResource &group, bfp::Register *reg,
			const bool optional = false);

	bool findDMA(const std::string &resourceName,
			const GroupResource &group, bfp::DMA *dma,
			const bool optional = false);

	bool findRegisterAddress(const std::string &resourceName,
			const GroupResource &group, std::uint32_t *address,
			const bool optional = false);

	bool findDMANum(const std::string &resourceName,
			const GroupResource &group, std::uint32_t *address,
			const bool optional = false);

	bool findRegisterEnumAddress(const std::string &resourceName,
			std::uint32_t nResource, const GroupResource &group,
			std::unordered_map<std::uint32_t, const std::uint32_t> *mapInsert,
			const bool optional = false);

	bool findDMAEnumNum(const std::string &resourceName,
			std::uint32_t nResource, const GroupResource &group,
			std::unordered_map<std::uint32_t, const std::uint32_t> *mapInsert,
			const bool optional = false);

	void logResourceMismatch(const std::string &resourceName,
			const std::string &relatedResource, const GroupResource &group);

	bool hasErrorOccurred() const;

	void printInfo() const;

	void printInfoError() const;

 private:
	void logResourceFound(const std::string &resourceName,
			const GroupResource &group);

	void logResourceNotFound(const std::string &resourceName,
			const GroupResource &group);

	bfp::BFP m_bfp;
	std::unordered_map<GroupResource, GroupInfo> m_groupInfo;
	bool m_error;

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

}  // namespace iriov2



