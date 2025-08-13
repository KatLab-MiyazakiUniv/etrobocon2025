/**
 * @file   SpikeClient.cpp
 * @brief  SPIKEサーバーと通信するためのクライアントクラスの実装
 * @author takuchi17
 */

#include "SpikeClient.h"

SpikeClient::SpikeClient() {}

bool SpikeClient::connect(const std::string& host, int port)
{
  return socket.connect(host, port);
}

void SpikeClient::disconnect()
{
  socket.close();
}