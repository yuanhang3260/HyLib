#ifndef SECURED_SOCKET_H_
#define SECURED_SOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "openssl/ssl.h"
#include "openssl/bio.h"
#include "openssl/err.h"

#include "Socket.h"
#include "IO/FileDescriptorInterface.h"

namespace net {

class SecuredSocket : public IO::FileDescriptorInterface {
 public:
  SecuredSocket(const std::string hostname, const int port = 80);
  ~SecuredSocket() { Close(); }

  int Read(void* buffer, int nbytes) override;
  int Write(const void* buf, int nbytes) const override;
  int Close() override;

  bool ssl_connection_established() { return ssl_connection_established_; }

 private:
 	std::string hostname_;
  int port_ = 80;
  BIO* bio;
  SSL* ssl;
  SSL_CTX* ctx;
  bool ssl_connection_established_ = false;
};

}  // namespace net

#endif  // SECURED_SOCKET_H_
