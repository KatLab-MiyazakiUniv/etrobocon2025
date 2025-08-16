#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include "actions/MiniFigActionHandler.h"
#include "actions/BackgroundPlaActionHandler.h"
#include <vector>

class SocketServer {
public:
    SocketServer(MiniFigActionHandler& minifigHandler, BackgroundPlaActionHandler& bgPlaHandler);
    bool init();
    void run();
    void shutdown();

private:
    int listenSocket;
    bool isRunning;
    MiniFigActionHandler& minifigHandler;
    BackgroundPlaActionHandler& bgPlaHandler;

    void handle_connection(int clientSocket);
};

#endif // SOCKET_SERVER_H
