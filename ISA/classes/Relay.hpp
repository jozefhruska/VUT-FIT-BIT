#ifndef RELAY_HEADER
#define RELAY_HEADER

#define MSG_TYPE_REPLY 7
#define MSG_TYPE_RELAY_FORWARD 12

#define OPTION_RELAY_MSG 9
#define OPTION_INTERFACE_ID 18
#define MESSAGE_BUFFER_LENGTH 1024

#define OPTION_IA_NA 3
#define OPTION_IA_NA_OFFSET 12

#define OPTION_IA_TA 4
#define OPTION_IA_TA_OFFSET 4

#define OPTION_IA_PD 25
#define OPTION_IA_PD_OFFSET 12
#define OPTION_IA_PD_LIFETIMES_OFFSET 8

#define OPTION_CLIENT_IDENTIFIER 1
#define OPTION_CLIENT_LINKLAYER_ADDR 79
#define OPTION_CLIENT_LINKLAYER_ADDR_SIZE 8

#define MAC_ADDRESS_LENGTH 6

#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netinet/if_ether.h>
#include <netinet/ip6.h>
#include <netinet/udp.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <cerrno>
#include <clocale>
#include <cstring>
#include <iostream>

#include "App.hpp"

struct dhcpv6_relay_message_header {
  uint8_t msg_type;
  uint8_t hop_count;
  struct in6_addr link_address;
  struct in6_addr peer_address;
} __attribute__((packed));

struct dhcpv6_relay_message_option_header {
  uint16_t option_code;
  uint16_t option_len;
} __attribute__((packed));

class Relay {
 private:
  struct FLAGS_ST *FLAGS;
  App *main_app;
  int socket_fd;
  pcap_if_t *interface;

  /**
   * @brief Get the link address value for Relay-Message option
   *
   * @param interface Interface of which link address to get
   * @return in6_addr Link address of requested interface
   */
  in6_addr get_link_address_value(pcap_if_t *interface);

  /**
   * @brief Get IPv6 addres from IA_NA option
   *
   * @param reply_message DHCPv6 message from which to get address
   * @param reply_message_length Lenght of DHCPv6 message
   * @return char* IPv6 address
   */
  char *get_ia_na_address(const u_char *reply_message, size_t reply_message_length);

  /**
   * @brief Get IPv6 addres from IA_TA option
   *
   * @param reply_message DHCPv6 message from which to get address
   * @param reply_message_length Lenght of DHCPv6 message
   * @return char* IPv6 address
   */
  char *get_ia_ta_address(const u_char *reply_message, size_t reply_message_length);

  /**
   * @brief Get IPv6 addres from IA_PD option
   *
   * @param reply_message DHCPv6 message from which to get address
   * @param reply_message_length Lenght of DHCPv6 message
   * @return char* IPv6 address
   */
  char *get_ia_pd_address(const u_char *reply_message, size_t reply_message_length);

  /**
   * @brief Get client's mac address
   *
   * @param reply_message DHCPv6 message from which to get address
   * @param reply_message_length Lenght of DHCPv6 message
   * @param peer_address Client's IPv6 address
   * @return char*
   */
  char *get_mac_address(const u_char *reply_message, size_t reply_message_length, in6_addr peer_address);

  /**
   * @brief Prints out debug message
   *
   * @param reply_message DHCPv6 message
   * @param reply_message_length DHCPv6 message length
   * @param peer_address Client's IPv6 address
   */
  void print_debug_message(const u_char *reply_message, size_t reply_message_length, in6_addr peer_address);

 public:
  /**
   * @brief Construct a new Relay:: Relay object
   *
   * @param FLAGS_IN Program flags
   * @param main_app_in Main App object
   * @param socket_fd_in Local socket file descriptor
   * @param interface_in Interface on which is Relay running
   */
  Relay(struct FLAGS_ST *FLAGS_IN, App *main_app_in, int socket_fd_in, pcap_if_t *interface_in);

  /**
   * @brief Separates DHCPv6 message from network headers
   *
   * @param packet Received packet with DHCPv6 message
   */
  void resolve_packet(const u_char *packet);

  /**
   * @brief Resolve unwrapper DHCPv6 message
   *
   * @param received_message Received DHCPv6 message
   * @param received_message_length Length of DHCPv6 message
   * @param ip6_header IPv6 header of received message
   * @param eth_header Ethernet header of received message
   */
  void resolve_dhcp_message(const u_char *received_message, size_t received_message_length, struct ip6_hdr ip6_header,
                            struct ether_header *eth_header);
};

#endif