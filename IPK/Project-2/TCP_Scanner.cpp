#include "TCP_Scanner.h"

TCP_Scanner::TCP_Scanner(sHOSTNAME *HOSTNAME, char *INTERFACE) {
  _HOSTNAME = HOSTNAME;
  _INTERFACE = INTERFACE;
}

/*
        Checksum (Modified)
        Source: https://www.tenouk.com/Module43a.html
*/
unsigned short TCP_Scanner::csum(unsigned short *buf, int len) {
  unsigned long sum;

  for (sum = 0; len > 0; len--) sum += *buf++;

  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);

  return (unsigned short)(~sum);
}

Scanner::SCAN_RESULT TCP_Scanner::startScan(int portNumber) {
  struct sockaddr_in sin;
  unsigned char buffer[4096];

  memset(buffer, 0, 4096);
  memset(&sin, 0, sizeof(sin));

  sin.sin_family = AF_INET;
  sin.sin_port = htons(portNumber);
  sin.sin_addr.s_addr = inet_addr(_HOSTNAME->value);

  /* Create an IP header */
  struct ip *IP_HEADER = (struct ip *)buffer;

  IP_HEADER->ip_hl = 5;
  IP_HEADER->ip_v = 4;
  IP_HEADER->ip_tos = IPTOS_PREC_ROUTINE;
  IP_HEADER->ip_len = htons(sizeof(struct ip));
  IP_HEADER->ip_id = htonl(random());
  IP_HEADER->ip_off = 0x0;
  IP_HEADER->ip_ttl = MAXTTL;
  IP_HEADER->ip_p = 6;
  IP_HEADER->ip_sum = 0x0;
  IP_HEADER->ip_src.s_addr = inet_addr(_INTERFACE);
  IP_HEADER->ip_dst.s_addr = inet_addr(_HOSTNAME->value);

  /* Create a TCP header */
  struct tcphdr *TCP_HEADER = (struct tcphdr *)(buffer + sizeof(struct ip));

  TCP_HEADER->th_sport = htons(9000);
  TCP_HEADER->th_dport = htons(portNumber);
  TCP_HEADER->th_seq = random();
  TCP_HEADER->th_ack = 0x0;
  TCP_HEADER->th_off = sizeof(struct tcphdr) + 1;
  TCP_HEADER->th_flags = TH_SYN;
  TCP_HEADER->th_win = htonl(TCP_MAXWIN);
  TCP_HEADER->th_sum = 0x0;
  TCP_HEADER->th_urp = 0x0;

  IP_HEADER->ip_sum = csum((unsigned short *)buffer,
                           (sizeof(struct ip) + sizeof(struct tcphdr)) >> 1);

  /* Create a socket */
  int SOCKET_FD = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
  if (SOCKET_FD < 0) {
    cerr << "Error: Unable to create a socket.\n";
    exit(1);
  }

  int one = 1;
  const int *val = &one;
  setsockopt(SOCKET_FD, IPPROTO_IP, IP_HDRINCL, val, sizeof(one));
  sendto(SOCKET_FD, buffer, IP_HEADER->ip_len, 0, (struct sockaddr *)&sin,
         sizeof(sin));
}