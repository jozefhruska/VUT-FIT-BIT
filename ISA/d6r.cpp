#include "d6r.hpp"

App *main_app;

/**
 * @brief Assigns new Sniffer on interface
 *
 * @param DATA_IN DATA
 * @return void*
 */
void *assign_sniffer(void *DATA_IN) {
  struct ASSIGN_SNIFFER_DATA *DATA = (struct ASSIGN_SNIFFER_DATA *)DATA_IN;

  pcap_if_t *interface = main_app->get_resolved_interface(DATA->interface);
  main_app->sniffers[DATA->interface] = new Sniffer(main_app->FLAGS, main_app, interface);
  main_app->sniffers[DATA->interface]->start();

  /* Stop packet capture */
  main_app->sniffers[DATA->interface]->stop();
  pthread_exit(NULL);
}

void signal_watcher(int signum) {
  std::cout << "Stopped correctly, all resources have been cleaned." << std::endl;

  main_app->cleanup();
  exit(EXIT_SUCCESS);
}

void *start_signal_watcher(void *data) {
  signal(SIGINT, signal_watcher);

  while (true) {
    sleep(1);
  }
}

int main(int argc, char **argv) {
  /* Create an App wrapper */
  main_app = new App(argc, argv);

  /* Bind global socket */
  struct sockaddr_in6 relay_address;
  relay_address.sin6_family = AF_INET6;
  relay_address.sin6_port = htons(547);
  relay_address.sin6_addr = IN6ADDR_ANY_INIT;

  if ((main_app->global_socket_fd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) != -1) {
    if (bind(main_app->global_socket_fd, (struct sockaddr *)&relay_address, sizeof(relay_address)) == -1) {
      std::cout << "[ERROR] - Unable to bind a socket." << std::endl;
      std::cout << "[ERROR] Message: " << std::strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }
  } else {
    std::cout << "[ERROR] Unable to create socket." << std::endl;
    std::cout << "[ERROR] Message: " << std::strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }

  /* Initialize and set threads and thread attributes */
  pthread_t threads[main_app->resolved_interfaces_count];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  void *status;

  /* Start signal watcher */
  pthread_t signal_watcher_thread;
  if (pthread_create(&signal_watcher_thread, &attr, start_signal_watcher, NULL) != 0) {
    std::cout << "[ERROR] Unable to start a new thread." << std::endl;
  };

  /* Resolve available interfaces for packet capture */
  main_app->resolve_interfaces();

  /* Start packet capture for each of resolved interfaces */
  main_app->sniffers = (Sniffer **)calloc(main_app->resolved_interfaces_count, sizeof(Sniffer *));
  for (int i = 0; i < main_app->resolved_interfaces_count; i++) {
    ASSIGN_SNIFFER_DATA *DATA = new ASSIGN_SNIFFER_DATA();
    DATA->interface = i;

    if (pthread_create(&threads[i], &attr, assign_sniffer, (void *)DATA) != 0) {
      std::cout << "[ERROR] Unable to start a new thread." << std::endl;
    };
  }

  /* Wait for threads to finish */
  pthread_attr_destroy(&attr);
  for (int i = 0; i < main_app->resolved_interfaces_count; i++) {
    if (pthread_join(threads[i], &status) != 0) {
      std::cout << "[ERROR] Unable to join thread." << std::endl;
    };
  }

  main_app->cleanup();
  exit(EXIT_SUCCESS);
}