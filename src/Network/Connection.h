#ifndef NETWORK_CONNECTION_H_
#define NETWORK_CONNECTION_H_

#include <memory>
#include <string>

#include "Base/MacroUtils.h"
#include "Network/SocketBase.h"
#include "Utility/BufferedDataReader.h"

namespace net {

class Connection {
 public:
  // Constructor takes ownership of socket.
  Connection(net::SocketBase* socket);
  FORBID_COPY_AND_ASSIGN(Connection);
  virtual ~Connection();

  int fd() const;

 protected:
  std::unique_ptr<net::SocketBase> socket_;
  std::unique_ptr<Utility::BufferedDataReader> buf_reader_;
};

}  // namespace net

#endif  // NETWORK_CONNECTION_H_