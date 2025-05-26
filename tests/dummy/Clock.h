/**
 * @file Clock.h
 * @brief クロッククラス（ダミー）
 * @author takahashitom
 */
#ifndef CLOCK_H
#define CLOCK_H

#include <cstdint>

namespace spikeapi {
  class Clock {
   public:
    Clock() { microTime = 0; }
    void sleep(int duration) { microTime += static_cast<uint64_t>(duration); }
    uint64_t now(void) const { return microTime; }

   private:
    uint64_t microTime;
  };
}  // namespace spikeapi

#endif