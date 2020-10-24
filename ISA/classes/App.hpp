#ifndef APP_HEADER
#define APP_HEADER

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <pcap/pcap.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

class Sniffer;

#include "Sniffer.hpp"

struct FLAGS_ST {
  char *FLAG_S;
  char *FLAG_I;
  bool FLAG_L;
  bool FLAG_D;
};

struct CLIENT_RECORD {
  struct in6_addr ipv6;
  uint8_t mac[6];
  struct CLIENT_RECORD *next;
};

class App {
 private:
  /**
   * @brief Resolves arguments and stores their values into FLAGS struct
   *
   * @param argc Argument count
   * @param argv Argument list
   */
  void resolve_arguments(int argc, char **argv);

  /**
   * @brief Checks if interface is capable of packet capture
   *
   * @param interface Interface to check
   * @return true Interface is capable of packet capture
   * @return false Interface is not capable of packet capture
   */
  bool check_interface_validity(pcap_if_t *interface);

 public:
  /**
   * @brief Construct a new App:: App object
   * This object is primarily for data storing purposes.
   * Other objects can acces it at any time
   *
   * @param argc Argument count
   * @param argv Argument list
   */
  App(int argc, char **argv);

  struct FLAGS_ST *FLAGS;
  struct CLIENT_RECORD *CLIENT_RECORDS;
  sem_t CLIENT_RECORDS_MUTEX;

  struct sockaddr_in6 relay_address;
  int global_socket_fd;

  pcap_if_t *resolved_interfaces, *available_interfaces;
  int resolved_interfaces_count;
  Sniffer **sniffers;

  /**
   * @brief Creates a list of interfaces with packet capturing
   *
   */
  void resolve_interfaces();

  /**
   * @brief Gets a interface from resolved interfaces list
   *
   * @param position Position of interface in resolved interfaces list
   * @return pcap_if_t* Resolved interface
   */
  pcap_if_t *get_resolved_interface(int position);

  /**
   * @brief Creates a socket for given interface name
   *
   * @param interface_name Name of interface for which to create new socket
   * @return int Socket file descriptor
   */
  int create_socket(char *interface_name);

  /**
   * @brief Cleans up memory and running threads
   *
   */
  void cleanup();
};

#endif