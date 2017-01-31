#include "Network/Connection.h"

namespace net {

Connection::Connection(net::SocketBase* socket) :
    socket_(socket),
    buf_reader_(new Utility::BufferedDataReader(socket_.get())) {
}

Connection::~Connection() {}

}  // namespace net
