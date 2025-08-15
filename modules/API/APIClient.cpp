/**
 * @file   APIClient.cpp
 * @brief  サーバーと通信を行うクライアントクラスの抽象クラス
 * @author takuchi17
 */

#include "APIClient.h"

APIClient::APIClient(SpikeClient& client) : spikeClient(client) {}

APIClient::~APIClient() {}