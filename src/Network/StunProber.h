#ifndef NET_STUN_PROBER_
#define NET_STUN_PROBER_

#include <string>

#define MAXLINE 200
 
// STUN(RFC5389) client.

namespace net {

class StunProber {
 public:
  // Probe NAT public address for a local port.
  // Parameter stun_server should be hostname:port or ip:port.
  // Returns NAT public ip:port.
  static std::string ProbeNAT(const std::string& stun_server, int local_port);
};

}  // namespace net

#endif  // NET_STUN_PROBER_