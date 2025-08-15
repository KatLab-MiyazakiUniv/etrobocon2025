/**
 * @file   ClockApiHandler.cpp
 * @brief  時刻関連APIを処理するハンドラクラスの実装
 * @author takuchi17
 */

#include "ClockApiHandler.h"
#include <iostream>

ClockApiHandler::ClockApiHandler(Socket* client)
  : ApiHandler(client),  // Call base class constructor
    clock()
{
}

// send() is now inherited from ApiHandler

void ClockApiHandler::handleSleep(const spike::ClockSleepRequest& request)
{
  spike::Response response;
  response.value = false;
  clock.sleep(request.microseconds);
}

spike::UInt64Response ClockApiHandler::handleNow()
{
  spike::UInt64Response response;
  response.value = 0;
  response.value = clock.now();
  return response;
}
