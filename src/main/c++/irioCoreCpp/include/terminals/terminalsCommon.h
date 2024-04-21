#pragma once

#include <string>

#include "terminals/terminalsBase.h"

namespace irio {

class TerminalsCommon: public TerminalsBase {
 public:
  TerminalsCommon(ParserManager *parserManager, const NiFpga_Session &session);

  std::string getFPGAVIversion() const;

  std::uint32_t getFref() const;

  bool getInitDone() const;

  std::uint8_t getDevQualityStatus() const;

  std::int16_t getDevTemp() const;

  bool getDAQStartStop() const;

  bool getDebugMode() const;

  void setDAQStart() const;

  void setDAQStop() const;

  void setDAQStartStop(const bool &start) const;

  void setDebugMode(const bool &debug) const;

  double getMinSamplingRate() const;

  double getMaxSamplingRate() const;
};

}  // namespace irio
