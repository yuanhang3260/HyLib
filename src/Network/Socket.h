#ifndef NETWORK_SOCKET_H_
#define NETWORK_SOCKET_H_

#include <string>
#include <memory>

#include "Base/MacroUtils.h"
#include "Network/SocketBase.h"

namespace net {

class Socket : public SocketBase {
 public:
  // Construct from fd.
  Socket() = default;
  Socket(int fd);
  virtual ~Socket();

  int fd() const { return fd_; }
  int Read(void* buffer, int nbytes) const override;
  int Write(const void* buf, int nbytes) const override;
  int Close() override;

 protected:
  int fd_;
};

class ServerSocket : public Socket {
 public:
  ServerSocket(int port, bool block=true);

  DEFINE_ACCESSOR(port, int);
 
 private:
  int port_;  // Server listen port.
};

class ClientSocket : public Socket {
 public:
  ClientSocket(const std::string& hostname, int port);

  int Connect(bool block=true);

  DEFINE_ACCESSOR(hostname, std::string);
  DEFINE_ACCESSOR(port, int);

 private:
  std::string hostname_;
  int port_;  // Server port
};

}  // namespace net

#endif  // NETWORK_SOCKET_H_
