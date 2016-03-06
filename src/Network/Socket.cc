#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "Socket.h"

namespace Network {

Socket* Socket::CreateClientSocket(std::string hostname, int port) {
  return new Socket(hostname, port);
}

int Socket::ClientConnect(bool block) {
  // Create socket
  fd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (fd_ < 0) {
    fprintf(stderr, "ERROR: opening socket\n");
    return -1;
  }

  struct sockaddr_in server_addr;
  struct hostent *server;

  // get server ip
  server = gethostbyname(hostname_.c_str());
  if (server == NULL) {
    fprintf(stderr,"ERROR: hostname not found\n");
    return -1;
  }

  // set server
  bzero((char *)&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET; // IPv4
  server_addr.sin_addr = *(struct in_addr *)*server->h_addr_list;
  server_addr.sin_port = htons(port_);

  /* Now connect to the server */
  if (connect(fd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    fprintf(stderr, "ERROR connecting\n");
    return -1;
  }

  if (!block) {
    int x = fcntl(fd_, F_GETFL, 0);
    fcntl(fd_, F_SETFL, x | O_NONBLOCK);
  }

  setFd(fd_);

  return 0;
}

Socket* Socket::CreateServerSocket(int port, bool block) {
  struct sockaddr_in serv_addr;

  // Create socket
  int fd = socket(AF_INET, SOCK_STREAM, 0);
 
  if (fd < 0) {
    fprintf(stderr, "ERROR: opening socket\n");
    return NULL;
  }

  if (!block) {
    int x = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, x | O_NONBLOCK);
  }

  // Set server addr
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);

  /* Now bind the host address using bind() call.*/
  if (bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr,"ERROR: hostname binding server sockfd\n");
    return NULL;
  }

  // Start listening
  listen(fd, 5);
  Socket *new_socket = new Socket();
  new_socket->setFd(fd);
  return new_socket;
}

int Socket::Read(void* buffer, int nbytes) const {
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

int Socket::Send(void* buffer, int nbytes) const {
  return Read(buffer, nbytes);
}

int Socket::Recv(const void* buffer, int nbytes) const {
  return Write(buffer, nbytes);
}


}  // namespace Network