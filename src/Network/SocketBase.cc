#include "SocketBase.h"

namespace Network {

SocketBase::~SocketBase() {}

int SocketBase::Send(void* buffer, int nbytes) const {
  return Read(buffer, nbytes);
}

int SocketBase::Recv(const void* buffer, int nbytes) const {
  return Write(buffer, nbytes);
}

}  // namespace Network
