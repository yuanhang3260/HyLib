#include "Network/Connection.h"

namespace net {

Connection::Connection(net::SocketBase* socket) :
    socket_(socket),
    buf_reader_(new Utility::BufferedDataReader(socket_.get())) {
}

int Connection::fd() const {
  return socket_->fd();
}

Connection::~Connection() {}

}  // namespace net
