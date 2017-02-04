#ifndef NETWORK_SOCKET_BASE_H_
#define NETWORK_SOCKET_BASE_H_

#include <string>
#include <memory>

#include "IO/FileDescriptorInterface.h"

namespace net {

class SocketBase : public IO::FileDescriptorInterface {
 public:
  // Construct server socket.
  SocketBase() = default;
  virtual ~SocketBase();

  virtual int fd() const = 0;
  virtual int Recv(void* buffer, int nbytes);
  virtual int Send(const void* buffer, int nbytes) const;
};

}  // namespace net

#endif  // NETWORK_SOCKET_BASE_H_
