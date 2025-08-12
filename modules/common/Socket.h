#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <cstddef> // For size_t

class Socket {
public:
    Socket();
    virtual ~Socket();

    // Server
    bool create();
    bool bind(int port);
    bool listen() const;
    Socket* accept() const;

    // Client
    bool connect(const std::string& host, int port);

    // Data Transmission
    bool send(const std::string& message) const;
    int receive(std::string& message) const;

    // New methods for raw data
    bool sendData(const void* data, size_t size) const;
    bool receiveData(void* buffer, size_t size) const;


    bool isValid() const;
    void close();

private:
    explicit Socket(int sock);
    int sock;
};

#endif