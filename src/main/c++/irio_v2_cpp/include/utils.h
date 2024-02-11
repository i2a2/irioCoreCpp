#pragma once

#include <string>
#include <NiFpga.h>

void throwIfNotSuccessNiFpga(NiFpga_Status& status, const std::string& errMsg = "");