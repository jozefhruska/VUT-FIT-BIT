#ifndef TCP_SCANNER_HEADER
#define TCP_SCANNER_HEADER

#include "ipk-scan.h"

class TCP_Scanner : public Scanner {
	public:
		TCP_Scanner(struct sHOSTNAME *HOSTNAME, char *INTERFACE);
		Scanner::SCAN_RESULT startScan(int portNumber);
		unsigned short csum(unsigned short *buf,int nwords);
};

#endif