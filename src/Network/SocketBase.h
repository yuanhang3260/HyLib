#ifndef NETWORK_SOCKET_BASE_H_
#define NETWORK_SOCKET_BASE_H_

#include <string>
#include <memory>

#include "IO/FileDescriptorInterface.h"

namespace Network {

class SocketBase : public IO::FileDescriptorInterface {
 public:
  // Construct server socket.
  SocketBase() = default;
  virtual ~SocketBase();

  virtual int Send(void* buffer, int nbytes) const;
  virtual int Recv(const void* buffer, int nbytes) const;
};

}  // namespace Network

#endif  // NETWORK_SOCKET_BASE_H_