#include "utils.h"
#include <stdexcept>

void throwIfNotSuccessNiFpga(NiFpga_Status& status, const std::string& errMsg){
    if(status != NiFpga_Status_Success){
        const std::string err = errMsg 
            + std::string("(Code: ")
            + std::to_string(static_cast<std::int32_t>(status))
            + std::string(")");
        throw std::runtime_error(err);
    }
}