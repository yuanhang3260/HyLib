#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <iostream>
#include <vector>

#include "Base/BaseTypes.h"
#include "Base/Log.h"
#include "Network/StunProber.h"
#include "Strings/Split.h"

namespace net {

namespace {

const int kBufferSize = 1024;

bool ParseStunServer(const std::string& stun_server,
                     struct sockaddr_in* servaddr) {
  auto re = Strings::Split(stun_server, ":");
  if (re.size() != 2) {
    LogERROR("Invalid stun server: %s, must be hostname:port or ip:port");
    return false;
  }

  int port = 0;
  try {
    port = std::stoi(re[1]);
  }
  catch (const std::invalid_argument& e) {
    LogERROR("Invalide port num: %s\n", re[1].c_str());
    return false;
  }

  // get server ip
  struct hostent *server = gethostbyname(re[0].c_str());
  if (server == NULL) {
    LogERROR("Hostname \"%s\" not found", re[0].c_str());
    return false;
  }

  bzero(servaddr, sizeof(*servaddr));
  servaddr->sin_family = AF_INET;
  servaddr->sin_addr = *(struct in_addr *)*server->h_addr_list;
  servaddr->sin_port = htons(port);

  return true;
}

}

std::string StunProber::ProbeNAT(const std::string& stun_server,
                                 int local_port) {
  struct sockaddr_in servaddr;
  if (!ParseStunServer(stun_server, &servaddr)) {
    return "";
  }

  // Create UDP socket.
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    LogERROR("Create UDP socket failed.");
    return "";
  } 

  // Client sockaddr_in.
  struct sockaddr_in localaddr;
  bzero(&localaddr, sizeof(localaddr));
  localaddr.sin_family = AF_INET;
  //inet_pton(AF_INET, "192.168.0.181", &localaddr.sin_addr);
  localaddr.sin_port = htons(local_port);

  int re = bind(sockfd, (struct sockaddr *)&localaddr, sizeof(localaddr));
  if (re < 0) {
    LogERROR("Bind local port to socket %d failed", local_port, sockfd);
    return "";
  }

  // Create stun bind request.
  byte bindingReq[20];
  uint16* uint16_p = (uint16*)bindingReq;
  *uint16_p = htons(0x0001);      // stun_method: bind
  *(uint16*)(bindingReq + 2) = htons(0x0000);      // msg_length
  *(uint32*)(bindingReq + 4) = htonl(0x2112A442);  // magic cookie

  *(uint32*)(bindingReq + 8) = htonl(0x63c7117e);  // transacation ID 
  *(uint32*)(bindingReq + 12)= htonl(0x0714278f);
  *(uint32*)(bindingReq + 16)= htonl(0x5ded3221);

  // Send UDP
  printf("Sending stun request...\n");
  re = sendto(sockfd, bindingReq, sizeof(bindingReq),0,
              (struct sockaddr *)&servaddr, sizeof(servaddr));
  if (re < 0) {
    LogERROR("Send Stun request failed");
    return "";
  }

  printf("Receiving response...\n");
  byte buf[kBufferSize];
  re = recvfrom(sockfd, buf, kBufferSize, 0, NULL, 0);
  if (re < 0) {
    LogERROR("Receive stun response failed");
    return "";
  }

  printf("Parsing response\n");

  int16 attr_type;
  int16 attr_length;
  int16 port;
  byte family;
  uint16_p = (uint16*)buf;
  if (*uint16_p == htons(0x0101)) {
    printf("STUN binding resp: success!\n");

    // Parse XOR
    // htons(*(uint16*)(buf + 2));
    uint32 offset = 20;
    while(offset < sizeof(buf)) {
      attr_type = htons(*(uint16*)(buf + offset));
      attr_length = htons(*(uint16*)(buf + offset + 2));
      offset += 4;
      if (attr_type == 0x0001) {
        // parse : port, IP.
        family = ntohs(*(byte*)(buf + offset + 1));
        printf("family = %x\n", family);
        // if (family != 0x01) {
        //   LogINFO("Skip IPv6 address");
        //   continue;
        // }
        port = ntohs(*(uint16*)(buf + offset + 2));
        //port ^= 0x2112;
        std::string result = std::to_string(buf[offset + 4]) + "." +
                             std::to_string(buf[offset + 5]) + "." +
                             std::to_string(buf[offset + 6]) + "." +
                             std::to_string(buf[offset + 7]) + ":" +
                             std::to_string(port);
        return result;
      }
      offset += attr_length;
    }
  }

  close(sockfd);
  return "";
}

}  // namespace net