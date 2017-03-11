#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "Base/Log.h"
#include "Network/Socket.h"

namespace net {

Socket::Socket(int fd) {
  if (fd > 0) {
    fd_ = fd;
    closed_ = false;
  }
}

Socket::~Socket() {
  Close();
}

int Socket::Close() {
  if (!closed_ && fd_ > 0) {
    int re = close(fd_);
    if (re != 0) {
      return re; 
    }
    closed_ = true;
    return 0;
  }
  return -1;
}

int Socket::Read(void* buffer, int nbytes) {
  if (!closed_ && fd_ > 0) {
    return read(fd_, buffer, nbytes);
  }
  return -1;
}

int Socket::Write(const void* buf, int nbytes) const {
  if (!closed_ && fd_ > 0) {
    int nwrite = write(fd_, buf, nbytes);
    return nwrite;
  }
  return -1;
}

void Socket::SetNonBlock() {
  int x = fcntl(fd_, F_GETFL, 0);
  fcntl(fd_, F_SETFL, x | O_NONBLOCK);
}

// Server socket.
ServerSocket::ServerSocket(int port, bool block) {
  struct sockaddr_in serv_addr;

  // Create socket
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
 
  if (fd_ < 0) {
    LogERROR("Opening server socket");
    return;
  }

  if (!block) {
    int x = fcntl(fd_, F_GETFL, 0);
    fcntl(fd_, F_SETFL, x | O_NONBLOCK);
  }

  // Set server addr
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);

  /* Now bind the host address using bind() call.*/
  if (bind(fd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    LogERROR("Failed to hostname binding server sockfd");
    return;
  }

  // Start listening
  if (listen(fd_, 5) != 0) {
    LogERROR("Failed to listen on server socket");
  }
  port_ = port;
  closed_ = false;
}

// Client socket.
ClientSocket::ClientSocket(const std::string& hostname, int port) :
    hostname_(hostname),
    port_(port) {
}

ClientSocket::ClientSocket(int local_port,
                           const std::string& hostname, int port) :
    local_port_(local_port),
    hostname_(hostname),
    port_(port) {
}

int ClientSocket::Connect(bool block) {
  // Create socket
  fd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (fd_ < 0) {
    LogERROR("Faild to create client socket");
    return -1;
  }

  if (local_port_ > 0) {
    struct sockaddr_in localaddr;
    bzero(&localaddr, sizeof(localaddr));
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(local_port_);

    int re = bind(fd_, (struct sockaddr*)&localaddr, sizeof(localaddr));
    if (re < 0) {
      LogERROR("Bind local port to socket %d failed", local_port_, fd_);
      return -1;
    }
  }

  struct sockaddr_in server_addr;
  struct hostent *server;

  // get server ip
  server = gethostbyname(hostname_.c_str());
  if (server == NULL) {
    LogERROR("Hostname \"%s\" not found", hostname_.c_str());
    return -1;
  }

  // set server
  bzero((char *)&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET; // IPv4
  server_addr.sin_addr = *(struct in_addr *)*server->h_addr_list;
  server_addr.sin_port = htons(port_);

  /* Now connect to the server */
  if (connect(fd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    LogERROR("Can't connect to \"%s\"", hostname_.c_str());
    return -1;
  }

  if (!block) {
    int x = fcntl(fd_, F_GETFL, 0);
    fcntl(fd_, F_SETFL, x | O_NONBLOCK);
  }

  closed_ = false;
  return 0;
}

}  // namespace net
