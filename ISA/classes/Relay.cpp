#include "Relay.hpp"

/**
 * @brief Construct a new Relay:: Relay object
 *
 * @param FLAGS_IN Program flags
 * @param main_app_in Main App object
 * @param socket_fd_in Local socket file descriptor
 * @param interface_in Interface on which is Relay running
 */
Relay::Relay(struct FLAGS_ST *FLAGS_IN, App *main_app_in, int socket_fd_in, pcap_if_t *interface_in) {
  FLAGS = FLAGS_IN;
  main_app = main_app_in;
  socket_fd = socket_fd_in;
  interface = interface_in;
};

/**
 * @brief Separates DHCPv6 message from network headers
 *
 * @param packet Received packet with DHCPv6 message
 */
void Relay::resolve_packet(const u_char *packet) {
  struct ether_header *eth_header;
  struct ip6_hdr ip6_header;

  eth_header = (struct ether_header *)malloc(sizeof(struct ether_header));
  memcpy(eth_header, packet, sizeof(struct ether_header));

  switch (ntohs(eth_header->ether_type)) {
    case ETHERTYPE_IPV6: {
      memcpy(&ip6_header, packet + sizeof(struct ether_header), sizeof(struct ip6_hdr));

      if (ip6_header.ip6_nxt == IPPROTO_UDP) {
        struct udphdr udp_header;
        memcpy(&udp_header, packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr), sizeof(struct udphdr));

        /* Length of received DHCPv6 message */
        size_t received_message_length = ntohs(udp_header.len) - sizeof(struct udphdr);

        /* Store client IPv6 and MAC addresses */
        struct CLIENT_RECORD *client_record = (struct CLIENT_RECORD *)malloc(sizeof(struct CLIENT_RECORD));
        memcpy(&client_record->ipv6, &ip6_header.ip6_src, sizeof(struct in6_addr));
        memcpy(client_record->mac, &eth_header->ether_shost, MAC_ADDRESS_LENGTH);

        sem_wait(&main_app->CLIENT_RECORDS_MUTEX);
        if (main_app->CLIENT_RECORDS == NULL) {
          main_app->CLIENT_RECORDS = client_record;
        } else {
          client_record->next = main_app->CLIENT_RECORDS;
          main_app->CLIENT_RECORDS = client_record;
        }
        sem_post(&main_app->CLIENT_RECORDS_MUTEX);

        /* Resolve specific DHCPv6 message */
        resolve_dhcp_message(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr) + sizeof(struct udphdr),
                             received_message_length, ip6_header, eth_header);
      }
    }
  }
}

/**
 * @brief Get the link address value for Relay-Message option
 *
 * @param interface Interface of which link address to get
 * @return in6_addr Link address of requested interface
 */
in6_addr Relay::get_link_address_value(pcap_if_t *interface) {
  pcap_addr_t *interface_address;
  for (interface_address = interface->addresses; interface_address; interface_address = interface_address->next) {
    if (interface_address->addr->sa_family == AF_INET6) {
      struct sockaddr_in6 *sock_address = (struct sockaddr_in6 *)interface_address->addr;
      return sock_address->sin6_addr;
    }
  }
}

/**
 * @brief Get IPv6 addres from IA_NA option
 *
 * @param reply_message DHCPv6 message from which to get address
 * @param reply_message_length Lenght of DHCPv6 message
 * @return char* IPv6 address
 */
char *Relay::get_ia_na_address(const u_char *reply_message, size_t reply_message_length) {
  char *result = (char *)malloc(INET6_ADDRSTRLEN);

  struct in6_addr n_address;

  /* Skip DHCPv6 header */
  size_t options_offset = 4;

  struct dhcpv6_relay_message_option_header option_header;
  memcpy(&option_header, reply_message + options_offset, sizeof(struct dhcpv6_relay_message_option_header));

  while (ntohs(option_header.option_code) != OPTION_IA_NA) {
    options_offset += sizeof(struct dhcpv6_relay_message_option_header) + ntohs(option_header.option_len);

    if (options_offset >= reply_message_length) {
      return NULL;
    }

    /* Try next option header */
    memcpy(&option_header, reply_message + options_offset, sizeof(struct dhcpv6_relay_message_option_header));
  }

  /* Skip to IA_NA-options */
  options_offset += sizeof(struct dhcpv6_relay_message_option_header) + OPTION_IA_NA_OFFSET;
  memcpy(&option_header, reply_message + options_offset, sizeof(struct dhcpv6_relay_message_option_header));
  memcpy(&n_address, reply_message + options_offset + sizeof(struct dhcpv6_relay_message_option_header),
         sizeof(struct in6_addr));

  inet_ntop(AF_INET6, &n_address, result, INET6_ADDRSTRLEN);

  return result;
}

/**
 * @brief Get IPv6 addres from IA_TA option
 *
 * @param reply_message DHCPv6 message from which to get address
 * @param reply_message_length Lenght of DHCPv6 message
 * @return char* IPv6 address
 */
char *Relay::get_ia_ta_address(const u_char *reply_message, size_t reply_message_length) {
  char *result = (char *)malloc(INET6_ADDRSTRLEN);

  struct in6_addr n_address;

  /* Skip DHCPv6 header */
  size_t options_offset = 4;

  struct dhcpv6_relay_message_option_header option_header;
  memcpy(&option_header, reply_message + options_offset, sizeof(struct dhcpv6_relay_message_option_header));

  while (ntohs(option_header.option_code) != OPTION_IA_TA) {
    options_offset += sizeof(struct dhcpv6_relay_message_option_header) + ntohs(option_header.option_len);

    if (options_offset >= reply_message_length) {
      return NULL;
    }

    /* Try next option header */
    memcpy(&option_header, reply_message + options_offset, sizeof(struct dhcpv6_relay_message_option_header));
  }

  /* Skip to IA_TA-options */
  options_offset += sizeof(struct dhcpv6_relay_message_option_header) + OPTION_IA_TA_OFFSET;
  memcpy(&option_header, reply_message + options_offset, sizeof(struct dhcpv6_relay_message_option_header));
  memcpy(&n_address, reply_message + options_offset + sizeof(struct dhcpv6_relay_message_option_header),
         sizeof(struct in6_addr));

  inet_ntop(AF_INET6, &n_address, result, INET6_ADDRSTRLEN);

  return result;
}

/**
 * @brief Get IPv6 addres from IA_PD option
 *
 * @param reply_message DHCPv6 message from which to get address
 * @param reply_message_length Lenght of DHCPv6 message
 * @return char* IPv6 address
 */
char *Relay::get_ia_pd_address(const u_char *reply_message, size_t reply_message_length) {
  char *result = (char *)malloc(INET6_ADDRSTRLEN + 4);

  struct in6_addr n_address;

  /* Skip DHCPv6 header */
  size_t options_offset = 4;

  struct dhcpv6_relay_message_option_header option_header;
  memcpy(&option_header, reply_message + options_offset, sizeof(struct dhcpv6_relay_message_option_header));

  while (ntohs(option_header.option_code) != OPTION_IA_PD) {
    options_offset += sizeof(struct dhcpv6_relay_message_option_header) + ntohs(option_header.option_len);

    if (options_offset >= reply_message_length) {
      return NULL;
    }

    /* Try next option header */
    memcpy(&option_header, reply_message + options_offset, sizeof(struct dhcpv6_relay_message_option_header));
  }

  /* Skip to IA_PD-options */
  options_offset += sizeof(struct dhcpv6_relay_message_option_header) + OPTION_IA_PD_OFFSET;
  memcpy(&option_header, reply_message + options_offset, sizeof(struct dhcpv6_relay_message_option_header));

  u_char prefix;
  char prefix_string[10];
  memcpy(&prefix,
         reply_message + options_offset + sizeof(struct dhcpv6_relay_message_option_header) +
             OPTION_IA_PD_LIFETIMES_OFFSET,
         sizeof(u_char));
  sprintf(prefix_string, "/%d", prefix);

  memcpy(&n_address,
         reply_message + options_offset + sizeof(struct dhcpv6_relay_message_option_header) +
             OPTION_IA_PD_LIFETIMES_OFFSET + 1,
         sizeof(struct in6_addr));
  inet_ntop(AF_INET6, &n_address, result, INET6_ADDRSTRLEN);

  result = strcat(result, prefix_string);

  return result;
}

/**
 * @brief Get client's mac address
 *
 * @param reply_message DHCPv6 message from which to get address
 * @param reply_message_length Lenght of DHCPv6 message
 * @param peer_address Client's IPv6 address
 * @return char*
 */
char *Relay::get_mac_address(const u_char *reply_message, size_t reply_message_length, in6_addr peer_address) {
  char peer_address_string[INET6_ADDRSTRLEN];
  inet_ntop(AF_INET6, &peer_address, peer_address_string, INET6_ADDRSTRLEN);

  CLIENT_RECORD *client_record = main_app->CLIENT_RECORDS;

  while (client_record != NULL) {
    char ipv6_string[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &client_record->ipv6, ipv6_string, INET6_ADDRSTRLEN);

    if (strcmp(peer_address_string, ipv6_string) == 0) {
      char *mac = (char *)malloc(20);
      mac = (ether_ntoa((const struct ether_addr *)&client_record->mac));
      return mac;
    }

    client_record = client_record->next;
  }

  return NULL;
}

/**
 * @brief Prints out debug message
 *
 * @param reply_message DHCPv6 message
 * @param reply_message_length DHCPv6 message length
 * @param peer_address Client's IPv6 address
 */
void Relay::print_debug_message(const u_char *reply_message, size_t reply_message_length, in6_addr peer_address) {
  char *p_address = get_ia_na_address(reply_message, reply_message_length);

  /* If IA_NA option was not found, try to look for IA_TA option */
  if (p_address == NULL) {
    p_address = get_ia_ta_address(reply_message, reply_message_length);
  }

  /* If IA_TA option was not found, try to look for IA_PD option */
  if (p_address == NULL) {
    p_address = get_ia_pd_address(reply_message, reply_message_length);
  }

  if (p_address != NULL) {
    char *mac_address = get_mac_address(reply_message, reply_message_length, peer_address);

    if (mac_address != NULL) {
      if (FLAGS->FLAG_D) {
        std::cout << p_address << ", " << mac_address << std::endl;
      }

      if (FLAGS->FLAG_L) {
        openlog("d6r", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
        syslog(LOG_INFO, "%s, %s", p_address, mac_address);
        closelog();
      }
    } else {
      if (FLAGS->FLAG_D) {
        std::cout << p_address << ", unable to obtain mac addres" << std::endl;
      }

      if (FLAGS->FLAG_L) {
        openlog("d6r", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
        syslog(LOG_INFO, "%s, unable to obtain mac address", p_address);
        closelog();
      }
    }

    free(p_address);
  }
}

/**
 * @brief Resolve unwrapper DHCPv6 message
 *
 * @param received_message Received DHCPv6 message
 * @param received_message_length Length of DHCPv6 message
 * @param ip6_header IPv6 header of received message
 * @param eth_header Ethernet header of received message
 */
void Relay::resolve_dhcp_message(const u_char *received_message, size_t received_message_length,
                                 struct ip6_hdr ip6_header, struct ether_header *eth_header) {
  /* Extract received DHCPv6 message type */
  u_char received_message_type = received_message[0];

  switch (received_message_type) {
    case 12: {
      // Not implemented
    }

    case 13: {
      // Not implemented
    }

    default: {
      /* Create server address struct */
      struct sockaddr_in6 *server_address = new sockaddr_in6();
      server_address->sin6_family = AF_INET6;
      server_address->sin6_port = htons(547);
      inet_pton(AF_INET6, FLAGS->FLAG_S, &(server_address->sin6_addr));

      /* Declare variables for creating new packets */
      u_char *relay_message = NULL;
      size_t relay_message_length;

      struct in6_addr link_address_value = get_link_address_value(interface);

      /* Allocate memory for 'Relay-forward' message */
      relay_message_length = sizeof(struct dhcpv6_relay_message_header) +
                             sizeof(struct dhcpv6_relay_message_option_header) +
                             sizeof(struct dhcpv6_relay_message_option_header) + OPTION_CLIENT_LINKLAYER_ADDR_SIZE +
                             received_message_length;
      if (IN6_IS_ADDR_LINKLOCAL(&link_address_value) || IN6_IS_ADDR_MULTICAST(&link_address_value)) {
        relay_message_length += sizeof(struct dhcpv6_relay_message_option_header) + sizeof(interface->name);
      }

      relay_message = (u_char *)malloc(relay_message_length);
      memset(relay_message, 0, relay_message_length);

      /* Create a 'Relay-forward' message header */
      struct dhcpv6_relay_message_header relay_message_header;
      relay_message_header.msg_type = MSG_TYPE_RELAY_FORWARD;
      relay_message_header.hop_count = 0;
      relay_message_header.link_address = link_address_value;
      memcpy(&relay_message_header.peer_address, &ip6_header.ip6_src, sizeof(struct in6_addr));
      memcpy(relay_message, &relay_message_header, sizeof(struct dhcpv6_relay_message_header));

      /* Create a 'Relay-forward' message option header */
      struct dhcpv6_relay_message_option_header relay_message_option_header;
      relay_message_option_header.option_code = htons(OPTION_RELAY_MSG);
      relay_message_option_header.option_len = htons(received_message_length);
      memcpy(relay_message + sizeof(struct dhcpv6_relay_message_header), &relay_message_option_header,
             sizeof(struct dhcpv6_relay_message_option_header));

      /* Copy received DHCPv6 message to 'Relay-forward' message */
      memcpy(relay_message + sizeof(struct dhcpv6_relay_message_header) +
                 sizeof(struct dhcpv6_relay_message_option_header),
             received_message, received_message_length);

      struct dhcpv6_relay_message_option_header relay_message_option_client_address_header;
      relay_message_option_client_address_header.option_code = htons(OPTION_CLIENT_LINKLAYER_ADDR);
      relay_message_option_client_address_header.option_len = htons(OPTION_CLIENT_LINKLAYER_ADDR_SIZE);
      memcpy(relay_message + sizeof(struct dhcpv6_relay_message_header) +
                 sizeof(struct dhcpv6_relay_message_option_header) + received_message_length,
             &relay_message_option_client_address_header, sizeof(struct dhcpv6_relay_message_option_header));

      uint16_t client_address_prefix = htons(1);
      memcpy(relay_message + sizeof(struct dhcpv6_relay_message_header) +
                 sizeof(struct dhcpv6_relay_message_option_header) + received_message_length +
                 sizeof(struct dhcpv6_relay_message_option_header),
             &client_address_prefix, sizeof(uint16_t));
      memcpy(relay_message + sizeof(struct dhcpv6_relay_message_header) +
                 sizeof(struct dhcpv6_relay_message_option_header) + received_message_length +
                 sizeof(struct dhcpv6_relay_message_option_header) + sizeof(uint16_t),
             &eth_header->ether_shost, sizeof(MAC_ADDRESS_LENGTH));

      /* Create Interface-ID option if peer address is not globally scoped */
      if (IN6_IS_ADDR_LINKLOCAL(&link_address_value) || IN6_IS_ADDR_MULTICAST(&link_address_value)) {
        struct dhcpv6_relay_message_option_header relay_message_option_interface_id_header;
        relay_message_option_interface_id_header.option_code = htons(OPTION_INTERFACE_ID);
        relay_message_option_interface_id_header.option_len = htons(sizeof(interface->name));
        memcpy(relay_message + sizeof(struct dhcpv6_relay_message_header) +
                   sizeof(struct dhcpv6_relay_message_option_header) + received_message_length +
                   sizeof(struct dhcpv6_relay_message_option_header) + OPTION_CLIENT_LINKLAYER_ADDR_SIZE,
               &relay_message_option_interface_id_header, sizeof(struct dhcpv6_relay_message_option_header));

        memcpy(relay_message + sizeof(struct dhcpv6_relay_message_header) +
                   sizeof(struct dhcpv6_relay_message_option_header) + received_message_length +
                   sizeof(struct dhcpv6_relay_message_option_header) + OPTION_CLIENT_LINKLAYER_ADDR_SIZE +
                   sizeof(struct dhcpv6_relay_message_option_header),
               interface->name, sizeof(interface->name));
      }

      /* Send 'Relay-forward' message to DHCPv6 server */
      if (sendto(main_app->global_socket_fd, relay_message, relay_message_length + 1, MSG_CONFIRM,
                 (const struct sockaddr *)server_address, sizeof(struct sockaddr_in6)) != -1) {
        /* Wait for server's reply */
        u_char *reply_buffer = (u_char *)malloc(MESSAGE_BUFFER_LENGTH);
        int reply_bytes_received = 0;

        if ((reply_bytes_received =
                 recvfrom(main_app->global_socket_fd, reply_buffer, MESSAGE_BUFFER_LENGTH, 0,
                          (struct sockaddr *)server_address, (socklen_t *)sizeof(struct sockaddr_in6)))) {
          struct dhcpv6_relay_message_header relay_reply_header;
          memcpy(&relay_reply_header, reply_buffer, sizeof(struct dhcpv6_relay_message_header));

          struct dhcpv6_relay_message_option_header relay_reply_option_header;
          memcpy(&relay_reply_option_header, reply_buffer + sizeof(struct dhcpv6_relay_message_header),
                 sizeof(struct dhcpv6_relay_message_option_header));

          size_t options_offset = 0;
          while (ntohs(relay_reply_option_header.option_code) != OPTION_RELAY_MSG) {
            options_offset +=
                sizeof(struct dhcpv6_relay_message_option_header) + ntohs(relay_reply_option_header.option_len);
            memcpy(&relay_reply_option_header,
                   reply_buffer + sizeof(struct dhcpv6_relay_message_header) + options_offset,
                   sizeof(struct dhcpv6_relay_message_option_header));
          }

          /* Free unused memory */
          free(relay_message);

          /* Allocate memory for Relay message */
          relay_message_length = ntohs(relay_reply_option_header.option_len);
          relay_message = (u_char *)malloc(relay_message_length);
          memset(relay_message, 0, relay_message_length);
          memcpy(relay_message,
                 reply_buffer + sizeof(struct dhcpv6_relay_message_header) + options_offset +
                     sizeof(struct dhcpv6_relay_message_option_header),
                 relay_message_length);

          /* Print debug message */
          if (relay_message[0] == MSG_TYPE_REPLY && (FLAGS->FLAG_D || FLAGS->FLAG_L)) {
            print_debug_message(relay_message, relay_message_length, relay_reply_header.peer_address);
          }

          /* Send a DHCP message to client */
          struct sockaddr_in6 *client_address = new sockaddr_in6();
          client_address->sin6_family = AF_INET6;
          client_address->sin6_port = htons(546);
          memcpy(&client_address->sin6_addr, &relay_reply_header.peer_address, sizeof(struct in6_addr));

          if (sendto(socket_fd, relay_message, relay_message_length + 1, 0, (const struct sockaddr *)client_address,
                     sizeof(struct sockaddr_in6)) != -1) {
            /* Free unused memory */
            free(relay_message);
            free(reply_buffer);
            delete (client_address);
          } else {
            std::cout << "[ERROR] Unable to forward 'Relay-reply to client." << std::endl;
            std::cout << "[ERROR] Message: " << std::strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
          }
        } else {
          std::cout << "[ERROR] Unable to receive a 'Relay-reply' from DHCPv6 server (" << FLAGS->FLAG_S << ")."
                    << std::endl;
          std::cout << "[ERROR] Message: " << std::strerror(errno) << std::endl;
          exit(EXIT_FAILURE);
        }
      } else {
        std::cout << "[ERROR] Unable to send 'Relay-forward' to DHCPv6 server (" << FLAGS->FLAG_S << ")." << std::endl;
        std::cout << "[ERROR] Message: " << std::strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }
}