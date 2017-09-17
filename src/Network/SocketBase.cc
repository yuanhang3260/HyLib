#include "SocketBase.h"

namespace net {

SocketBase::~SocketBase() {}

int SocketBase::Recv(void* buffer, int nbytes) const {
  return Read(buffer, nbytes);
}

int SocketBase::Send(const void* buffer, int nbytes) const {
  return Write(buffer, nbytes);
}

}  // namespace net
