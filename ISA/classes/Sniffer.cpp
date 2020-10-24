#include "Sniffer.hpp"

/**
 * @brief Construct a new Sniffer:: Sniffer object
 *
 * @param FLAGS_IN Proram's flags
 * @param main_app_in Main App object
 * @param interface_in Local interface
 */
Sniffer::Sniffer(struct FLAGS_ST *FLAGS_IN, App *main_app_in, pcap_if_t *interface_in) {
  FLAGS = FLAGS_IN;
  main_app = main_app_in;
  interface = interface_in;
};

/**
 * @brief Starts packet capture on network interface.
 */
void Sniffer::start() {
  /* Create new socket for communication, bind to this socket */
  socket_fd = main_app->create_socket(interface->name);
  Relay *relay = new Relay(FLAGS, main_app, socket_fd, interface);

  if ((capture_handle = pcap_open_live(interface->name, BUFSIZ, 1, 1000, errbuf)) != NULL) {
    const u_char *packet;
    struct pcap_pkthdr header;
    struct bpf_program fp;
    int n = 1;

    /* Compile PCAP filter */
    if (pcap_compile(capture_handle, &fp, "dst port 547 and not src port 547", 0, PCAP_NETMASK_UNKNOWN) == -1) {
      std::cout << "[ERROR] Unable to compile PCAP filter." << std::endl;
      std::cout << "[ERROR] Message: " << errbuf << std::endl;
      exit(EXIT_FAILURE);
    }

    /* Set PCAP filter */
    if (pcap_setfilter(capture_handle, &fp) == -1) {
      std::cout << "[ERROR] Unable to set PCAP filter." << std::endl;
      std::cout << "[ERROR] Message: " << errbuf << std::endl;
      exit(EXIT_FAILURE);
    }

    /* Leave resolving of captured packet on relay module */
    while ((packet = pcap_next(capture_handle, &header)) != NULL) {
      relay->resolve_packet(packet);
    }

    if (packet == NULL) {
      std::cout << "[ERROR] Unable to start capture loop on network interface." << std::endl;
      std::cout << "[ERROR] Message: " << errbuf << std::endl;
      exit(EXIT_FAILURE);
    }
  } else {
    std::cout << "[ERROR] Unable to start capture on network interface." << std::endl;
    std::cout << "[ERROR] Message: " << errbuf << std::endl;
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Stops packet capture on network interface.
 */
void Sniffer::stop() { pcap_close(capture_handle); }