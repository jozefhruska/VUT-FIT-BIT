#include "App.hpp"

char HELP_CONTENT[] =
    "Usage: d6r -s server [-l] [-d] [-i interface]\n\n"
    "Options:\n"
    "-s: IPv6 address of DHCPv6 server\n"
    "-l: Enables logging with syslog messages\n"
    "-i: Interface on which should Relay watch for DHCPv6 requests (all "
    "interfaces if not set)\n"
    "-d: Enables debugging log on STDOUT";
/**
 * @brief Construct a new App:: App object
 * This object is primarily for data storing purposes.
 * Other objects can acces it at any time
 *
 * @param argc Argument count
 * @param argv Argument list
 */
App::App(int argc, char **argv) {
  FLAGS = new FLAGS_ST();
  FLAGS->FLAG_S = NULL;
  FLAGS->FLAG_I = NULL;
  FLAGS->FLAG_L = false;
  FLAGS->FLAG_D = false;

  resolved_interfaces = NULL;
  resolved_interfaces_count = 0;

  CLIENT_RECORDS = NULL;
  sem_init(&CLIENT_RECORDS_MUTEX, 0, 1);

  resolve_arguments(argc, argv);
};

/**
 * @brief Resolves arguments and stores their values into FLAGS struct
 *
 * @param argc Argument count
 * @param argv Argument list
 */
void App::resolve_arguments(int argc, char **argv) {
  char argument;

  if (argc == 1) {
    std::cout << HELP_CONTENT << std::endl;
    exit(EXIT_FAILURE);
  }

  while ((argument = getopt(argc, argv, "s:li:d")) != -1) {
    switch (argument) {
      case 's':
        FLAGS->FLAG_S = optarg;
        break;
      case 'l':
        FLAGS->FLAG_L = true;
        break;
      case 'i':
        FLAGS->FLAG_I = optarg;
        break;
      case 'd':
        FLAGS->FLAG_D = true;
        break;
      default:
        std::cout << HELP_CONTENT << std::endl;
        exit(EXIT_FAILURE);
    }
  }
}

/**
 * @brief Checks if interface is capable of packet capture
 *
 * @param interface Interface to check
 * @return true Interface is capable of packet capture
 * @return false Interface is not capable of packet capture
 */
bool App::check_interface_validity(pcap_if_t *interface) {
  pcap_addr_t *interface_address;
  struct sockaddr_in6 *interface_ipv6_address;
  bool result = false;

  for (interface_address = interface->addresses; interface_address; interface_address = interface_address->next) {
    if (interface_address->addr->sa_family == AF_INET6) {
      interface_ipv6_address = (sockaddr_in6 *)interface_address->addr;

      if (IN6_IS_ADDR_LINKLOCAL(&interface_ipv6_address->sin6_addr)) {
        result = true;
      }
    }
  }

  return result;
}

/**
 * @brief Creates a list of interfaces with packet capturing
 *
 */
void App::resolve_interfaces() {
  pcap_if_t *interface_temp;
  char errbuf[PCAP_ERRBUF_SIZE];

  if (pcap_findalldevs(&available_interfaces, errbuf) != -1) {
    if (FLAGS->FLAG_I != NULL) {
      for (interface_temp = available_interfaces; interface_temp; interface_temp = interface_temp->next) {
        /* Skip interface if it's invalid */
        if (!check_interface_validity(interface_temp)) {
          continue;
        }

        /* Add interface to the list of resolved interfaces */
        if (strcmp(FLAGS->FLAG_I, interface_temp->name) == 0) {
          resolved_interfaces = new pcap_if_t();
          memcpy(resolved_interfaces, interface_temp, sizeof(pcap_if_t));
          resolved_interfaces->addresses = interface_temp->addresses;
          resolved_interfaces->next = NULL;

          /* Increase the conter of resolved interfaces */
          resolved_interfaces_count++;

          break;
        }
      }

      if (resolved_interfaces == NULL) {
        std::cout << "[ERROR] Interface '" << FLAGS->FLAG_I << "' not found." << std::endl;
      }
    } else {
      for (interface_temp = available_interfaces; interface_temp; interface_temp = interface_temp->next) {
        /* Skip interface if it's invalid */
        if (!check_interface_validity(interface_temp)) {
          continue;
        }

        /* Add interface to the list of resolved interfaces */
        if (resolved_interfaces == NULL) {
          resolved_interfaces = new pcap_if_t();
          memcpy(resolved_interfaces, interface_temp, sizeof(pcap_if_t));
          resolved_interfaces->addresses = interface_temp->addresses;
          resolved_interfaces->next = NULL;
        } else {
          pcap_if_t *interface_loop_current = resolved_interfaces;

          while (interface_loop_current->next != NULL) {
            interface_loop_current = interface_loop_current->next;
          }

          interface_loop_current->next = new pcap_if_t();
          memcpy(interface_loop_current->next, interface_temp, sizeof(pcap_if_t));
          interface_loop_current->next->addresses = interface_temp->addresses;
          interface_loop_current->next->next = NULL;
        }

        /* Increase the conter of resolved interfaces */
        resolved_interfaces_count++;
      }
    }
  } else {
    std::cout << "[ERROR] Unable to get network interfaces." << std::endl;
    std::cout << "[ERROR] Message: " << std::strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Gets a interface from resolved interfaces list
 *
 * @param position Position of interface in resolved interfaces list
 * @return pcap_if_t* Resolved interface
 */
pcap_if_t *App::get_resolved_interface(int position) {
  pcap_if_t *interface = resolved_interfaces;

  for (int i = 0; i < position; i++) {
    if (interface->next != NULL) {
      interface = interface->next;
    } else {
      return NULL;
    }
  }

  return interface;
}

/**
 * @brief Creates a socket for given interface name
 *
 * @param interface_name Name of interface for which to create new socket
 * @return int Socket file descriptor
 */
int App::create_socket(char *interface_name) {
  int socket_fd;

  if ((socket_fd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) != -1) {
    if (setsockopt(socket_fd, SOL_SOCKET, SO_BINDTODEVICE, interface_name, strlen(interface_name)) < 0) {
      std::cout << "[ERROR] - Unable to bind a socket." << std::endl;
      std::cout << "[ERROR] Message: " << std::strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }

    return socket_fd;
  } else {
    std::cout << "[ERROR] Unable to create socket." << std::endl;
    std::cout << "[ERROR] Message: " << std::strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Cleans up memory and running threads
 *
 */
void App::cleanup() {
  /* Stop created sniffers and close their sockets */
  for (int i = 0; i < resolved_interfaces_count; i++) {
    if (sniffers[i] != NULL) {
      sniffers[i]->stop();
      close(sniffers[i]->socket_fd);
    }
  }

  /* Free the list of available interfaces */
  pcap_freealldevs(available_interfaces);
}