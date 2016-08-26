#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

class Telemetry {
  public:
    Telemetry(const char *hostname, const char *port);
    void Send(double value);
 private:
    int sock;
    sockaddr_storage dstAddr;
};

Telemetry::Telemetry(const char *hostname, const char *port) {
  int result = 0;
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock == -1) {
    throw "failed to init sock";
  }

  sockaddr_in addrListen = {};
  addrListen.sin_family = AF_INET;
  result = bind(sock, (sockaddr*)&addrListen, sizeof(addrListen));
  if (result == -1) {
    throw "failed to bind";
  }

  addrinfo* result_list = NULL;
  addrinfo hints = {};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  result = getaddrinfo(hostname, port, &hints, &result_list);
  if (result != 0) {
    throw "failed to getaddrinfo";
  }
  memcpy(&dstAddr, result_list->ai_addr, result_list->ai_addrlen);
  freeaddrinfo(result_list);
}

void Telemetry::Send(double value) {
  char msg [1024];
  int n;
  n = sprintf(msg, "%f", value);

  int result;
  result = sendto(sock, msg, n, 0, (sockaddr*)&dstAddr, sizeof(dstAddr));
  if (result == -1) {
    throw "failed to send";
  }

  std::cout << result << " bytes sent" << std::endl;
}

int main(int argc, char* argv[]) {
  try {
    if (argc != 3) {
      throw "bad arguments";
    }
    Telemetry t(argv[1], argv[2]);
    t.Send(2.23);
  } catch (const char *e) {
    std::cout << "exception: " << e << std::endl;
  }
}
