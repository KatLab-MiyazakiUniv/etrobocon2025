/**
 * @file   ClockApiHandler.cpp
 * @brief  時刻関連APIを処理するハンドラクラスの実装
 * @author takuchi17
 */

#include "ClockApiHandler.h"
#include <iostream>
#include <algorithm>

ClockApiHandler::ClockApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    clock()
{
}

// Helper function for 64-bit integer conversion
uint64_t swap_uint64(uint64_t val)
{
  val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
  val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
  return (val << 32) | (val >> 32);
}

// send() is now inherited from ApiHandler

void ClockApiHandler::handleSleep(const spike::ClockSleepRequest& request)
{
  spike::Response response;
  response.value = false;
  uint64_t microseconds = swap_uint64(request.microseconds);
  clock.sleep(microseconds);
  send(reinterpret_cast<char*>(&response), sizeof(response));
}

void ClockApiHandler::handleNow()
{
  spike::UInt64Response response;
  response.value = 0;
  response.value = clock.now();
  response.value = swap_uint64(response.value);
  send(reinterpret_cast<char*>(&response), sizeof(response));
}
