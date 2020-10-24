#ifndef SNIFFER_HEADER
#define SNIFFER_HEADER

#include <pcap/pcap.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

class App;

#include "App.hpp"
#include "Relay.hpp"

class Sniffer {
 private:
  struct FLAGS_ST *FLAGS;
  App *main_app;
  pcap_if_t *interface;

  pcap_t *capture_handle;
  char errbuf[PCAP_ERRBUF_SIZE];

 public:
  /**
   * @brief Construct a new Sniffer:: Sniffer object
   *
   * @param FLAGS_IN Proram's flags
   * @param main_app_in Main App object
   * @param interface_in Local interface
   */
  Sniffer(struct FLAGS_ST *FLAGS_IN, App *main_app_in, pcap_if_t *interface_in);

  int socket_fd;

  /**
   * @brief Starts packet capture on network interface.
   */
  void start();

  /**
   * @brief Stops packet capture on network interface.
   */
  void stop();
};

#endif