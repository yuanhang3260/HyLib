#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>

#include "SecuredSocket.h"

namespace net {

SecuredSocket::SecuredSocket(const std::string hostname, const int port) :
    hostname_(hostname),
    port_(port)
{
  /* Set up the library */
  ERR_load_BIO_strings();
  SSL_load_error_strings();
  SSL_library_init();
  OpenSSL_add_all_algorithms();

  /* Set up the SSL context */
  ctx = SSL_CTX_new(SSLv23_client_method());
  if (ctx == NULL) {
    fprintf(stderr, "ERROR: ctx is NULL\n");
    return;
  }
  /* Load the trust store */
  if(!SSL_CTX_load_verify_locations(ctx, "TrustStore.pem", NULL)) {
    fprintf(stderr, "Error loading trust store\n");
    ERR_print_errors_fp(stderr);
    return;
  }

  /* Setup the connection */
  bio = BIO_new_ssl_connect(ctx);

  /* Set the SSL_MODE_AUTO_RETRY flag */
  BIO_get_ssl(bio, & ssl);
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

  /* Create and setup the connection */
  BIO_set_conn_hostname(bio, (hostname_+":https").c_str());
  if(BIO_do_connect(bio) <= 0) {
    fprintf(stderr, "Error attempting to connect\n");
    ERR_print_errors_fp(stderr);
    return;
  }

  /* Check the certificate */
  if(SSL_get_verify_result(ssl) != X509_V_OK) {
    fprintf(stderr, "Certificate verification error: %ld\n",
            SSL_get_verify_result(ssl));
    BIO_free_all(bio);
    SSL_CTX_free(ctx);
    return;
  }

  ssl_connection_established_ = true;
  closed_ = false;
}

int SecuredSocket::fd() const {
  // TODO
  return 0;
}

int SecuredSocket::Close() {
  if (!closed_ && ssl_connection_established_) {
    BIO_free_all(bio);
    SSL_CTX_free(ctx);
  }
  return -1;
}

int SecuredSocket::Read(void* buffer, int nbytes) const {
  if (ssl_connection_established_) {
    int nread = BIO_read(bio, buffer, nbytes);
    return nread;
  }
  return -1;
}

int SecuredSocket::Write(const void* buffer, int nbytes) const {
  // Send the request.
  if (ssl_connection_established_) {
    int nwrite = BIO_write(bio, buffer, nbytes);
    return nwrite;
  }
  return -1;
}

}  // namespace net