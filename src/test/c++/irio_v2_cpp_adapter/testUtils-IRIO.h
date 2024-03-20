#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <gtest/gtest.h>
#include <irioDataTypes.h>
#include <functional>
#include <map>

using std::string;

namespace TestUtilsIRIO {

enum class IRIOFamily { FlexRIO, cRIO, NONE };
enum class IRIOProfile { NoModule, OnlyResources, IO, CPUDAQ, CPUIMAQ, Mod5761DAQ, Mod6581DIO, Mod1483IMAQ };
const string FWHT = "\x1b[37m";  // Foreground color, White
const string BBLA = "\x1b[40m";  // Background color, Black
const string RST  = "\x1B[0m";   // Foreground color, Reset
const string FCYN = "\x1b[36m";  // Foreground color, Cyan
const string FRED = "\x1b[31m";  // Foreground color, Red

void displayTitle(const string& msg, const string& forecolor = FWHT, const string& backcolor = BBLA);
void logErrors(const int ret_status, const TStatus& out_status);
string getEnvVar(const string& shellVarName);
double sineCorrelation(const std::vector<double>& vec, int f, int fs);

[[gnu::warn_unused_result]] int initDriver(IRIOProfile profile, irioDrv_t* drv);
[[gnu::warn_unused_result]] int closeDriver(irioDrv_t* drv);
//int  loadHeaderFile(irioDrv_t* drv, string file_path, TStatus* status);
//void freeHeaderFile(irioDrv_t* drv);
void startFPGA(irioDrv_t* drv);
void setDebugMode(irioDrv_t* drv, int debug_mode);
TIRIOCouplingMode setAICoupling(irioDrv_t* drv);
IRIOFamily getIRIOFamily(string device);

typedef struct {
    int AI, AO, auxAI, auxAO, DI, DO, auxDI, auxDO, SG, DMA, CLConfig, CLUART, samplingRate;
} irioResources_t;

void getResources(irioDrv_t* drv, irioResources_t* res);

namespace DMAHost {
    void cleanDMA(irioDrv_t* drv);
    void setupDMA(irioDrv_t* drv);
    int  setSamplingRate(irioDrv_t* drv, int32_t sampling_rate);
    void setEnable(irioDrv_t* drv, int channel, int enable);
    void setDAQStartStop(irioDrv_t* drv, int startstop);
    std::vector<uint64_t> readDMAData(irioDrv_t* drv, int dmaN, int blocksToRead, int wordsPerBlock, int sampling_freq);
    std::vector<uint64_t> readDMADataTimeout(irioDrv_t* drv, int dmaN, int blocksToRead, int wordsPerBlock, int sampling_freq);
}  // namespace DMAHost

namespace SG {
    void     setUpdateRate(irioDrv_t* drv, int channel, int32_t update_rate, uint32_t fref);
    void     setSignalType(irioDrv_t* drv, int channel, int signal_type);
    void     setFsig(irioDrv_t* drv, int channel, uint32_t update_rate, uint32_t freq);
    void     setSigAmp(irioDrv_t* drv, int channel, int32_t amp);
    uint32_t getFref(irioDrv_t* drv, int channel);
    double   getCVDAC(irioDrv_t* drv);
}  // namespace SG 
}  // namespace TestUtilsIRIO

#endif /* TESTUTILS_H_ */
