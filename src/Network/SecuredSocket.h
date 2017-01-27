#ifndef __SECURED_SOCKET_H__
#define __SECURED_SOCKET_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "openssl/ssl.h"
#include "openssl/bio.h"
#include "openssl/err.h"

#include "Socket.h"
#include "IO/FileDescriptorInterface.h"

namespace Network {

class SecuredSocket : public IO::FileDescriptorInterface {
 public:
  SecuredSocket(const std::string hostname, const int port = 80);
  ~SecuredSocket() { Close(); }

  virtual int Read(void* buffer, const int nbytes) const;
  virtual int Write(const void* buf, const int nbytes) const;
  virtual int Send(void* buffer, const int nbytes) const;
  virtual int Recv(const void* buffer, const int nbytes) const;
  virtual int Close() {
    if (!closed_ && ssl_connection_established_) {
      BIO_free_all(bio);
	    SSL_CTX_free(ctx);
    }
    return -1;
  }

  bool ssl_connection_established() { return ssl_connection_established_; }

  virtual bool secured() { return true; }

 private:
 	std::string hostname_;
  int port_ = 80;
  BIO * bio;
  SSL * ssl;
  SSL_CTX * ctx;
  bool ssl_connection_established_ = false;
};

}  // namespace Network

#endif  /* __SECURED_SOCKET_H__ */
