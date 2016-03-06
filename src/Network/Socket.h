#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../IO/FileDescriptorInterface.h"

namespace Network {

class Socket : public IO::FileDescriptorInterface {
 public:
  // construct server port
  Socket() = default;
  // construct client port
  Socket(std::string hostname, int port) :
    IO::FileDescriptorInterface(-1, true),
    hostname_(hostname),
    port_(port) {}
  // construct from fd
  Socket(const int fd, bool auto_close=true) :
      IO::FileDescriptorInterface(fd, auto_close) {}
  virtual ~Socket() {}

  static Socket* CreateClientSocket(std::string hostname, int port);
  int ClientConnect(bool block=true);

  static Socket* CreateServerSocket(int port, bool block=true);

  virtual int Read(void* buffer, int nbytes) const;
  virtual int Write(const void* buf, int nbytes) const;
  virtual int Send(void* buffer, int nbytes) const;
  virtual int Recv(const void* buffer, int nbytes) const;

 private:
  std::string hostname_;
  int port_;
};

}  // namespace Network

#endif  /* __SOCKET_H__ */
