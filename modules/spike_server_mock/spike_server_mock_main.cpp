
/**
 * @file   spike_server_mock_main.cpp
 * @brief  SPIKEモックサーバーのメインエントリポイント
 * @author takuchi17
 */

#include "SpikeServerMock.h"
#include <iostream>

int main()
{
  std::cout << "Starting SpikeServerMock..." << std::endl;
  SpikeServerMock::start();
  return 0;
}
