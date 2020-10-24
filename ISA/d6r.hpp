#ifndef MAIN_HEADER
#define MAIN_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <iostream>
#include <pthread.h>
#include <csignal>

#include "classes/App.hpp"
#include "classes/Sniffer.hpp"
#include "classes/Relay.hpp"

struct ASSIGN_SNIFFER_DATA {
  int interface;
};

#endif