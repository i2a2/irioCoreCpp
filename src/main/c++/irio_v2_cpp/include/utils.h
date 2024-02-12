#pragma once

#include <string>
#include <bfp.h>
#include <NiFpga.h>
#include <unordered_map>

void throwIfNotSuccessNiFpga(NiFpga_Status& status, const std::string& errMsg = "");

template<typename T>
void findAndInsertEnumResources(const std::unordered_map<std::string, T> &mapSearch,
                                const std::string &resourceName,
                                const size_t maxElem,
                                std::unordered_map<std::uint8_t, const std::uint32_t> &mapInsert);

void findAndInsertEnumRegisters(const bfp::BFP &parsedBitfile,
                                const std::string &terminalName,
                                const size_t maxElem,
                                std::unordered_map<std::uint8_t, const std::uint32_t> &mapInsert);

void findAndInsertEnumDMAs(const bfp::BFP &parsedBitfile,
                            const std::string &dmaName,
                            const size_t maxElem,
                            std::unordered_map<std::uint8_t, const std::uint32_t> &mapInsert);