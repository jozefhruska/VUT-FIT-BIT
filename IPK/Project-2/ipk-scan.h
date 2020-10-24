#ifndef MAIN_HEADER
#define MAIN_HEADER

#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <errno.h>
#include <netinet/ip_icmp.h>

#include <iostream>
#include <regex>

using namespace std;

class Scanner {
	public:
		typedef enum eSCAN_RESULT { OPEN, FILTERED, CLOSED } SCAN_RESULT;
		SCAN_RESULT startScan(int portNumber);

	protected:
		struct sHOSTNAME *_HOSTNAME;
		char *_INTERFACE;
};

#include "TCP_Scanner.h"

enum HOST_TYPE { IPV4, IPV6, DOMAIN, LOCAL, UNKNOWN };

/**
 * @brief  Structure containing data of a program argument.
 */
struct sFLAG {
  bool active;
  char *value;
  vector<int> range;
};

struct sFLAGS {
	struct sFLAG *I_FLAG;
	struct sFLAG *PT_FLAG;
	struct sFLAG *PU_FLAG;
};

struct sHOSTNAME {
  HOST_TYPE type;
  char *value;
};

#endif