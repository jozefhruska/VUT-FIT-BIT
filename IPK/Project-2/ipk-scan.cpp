#include "ipk-scan.h"
#define SOCK_PATH "/dev/socket/echo_socket"

/* Hostname regular expressions
============================================================================= */

/*
	Regex licenced under Creative Commons.
	Author: Sllouyssgort
	Source: https://stackoverflow.com/questions/5284147/validating-ipv4-addresses-with-regexp
*/
regex regex_ipv4(
    "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?["
    "0-9][0-9]?)$");

/*
	Regex licenced under Creative Commons. (Modified)
	Author: David M. Syzdek, Qix
	Source: https://stackoverflow.com/questions/53497/regular-expression-that-matches-valid-ipv6-addresses
*/
regex regex_ipv6(
    "^(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-"
    "9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-"
    "F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-"
    "F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-"
    "F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{"
    "1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,"
    "4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|"
    "("
    "2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|"
    "1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])"
    ")$");

regex regex_domain(
    "^(?!:\\/\\/"
    ")([a-zA-Z0-9-_]+\\.)*[a-zA-Z0-9][a-zA-Z0-9-_]+\\.[a-zA-Z]{2,11}?$");

/* Ports regular expressions
============================================================================= */
regex regex_ports_single("^[0-9][0-9]*$");

/*
	Regex licenced under Creative Commons. (Modified)
	Author: Asaph, kenorb
	Source: https://stackoverflow.com/questions/1396084/regex-for-comma-delimited-list
*/
regex regex_ports_multiple("(\\d+)(,\\s*\\d+)*");

regex regex_ports_range("^[0-9]([0-9]*)-[0-9]([0-9]*)$");

HOST_TYPE getHostnameType(char *hostname) {
  if (regex_match(hostname, regex_ipv4)) {
    return IPV4;
  } else if (regex_match(hostname, regex_ipv6)) {
    return IPV6;
  } else if (regex_match(hostname, regex_domain)) {
    return DOMAIN;
  } else if (strcmp(hostname, "localhost") == 0) {
    return LOCAL;
  } else
    return UNKNOWN;
}

void parseArguments(int argc, char **argv, struct sFLAGS *FLAGS, sHOSTNAME *HOSTNAME) {
  int argument;
  bool p_active = false;
  opterr = 0;

  while ((argument = getopt(argc, argv, "i:pt:u:")) != -1) {
    switch (argument) {
      case 'i':
        FLAGS->I_FLAG->active = true;
        FLAGS->I_FLAG->value = optarg;
        break;
      case 'p':
        p_active = true;
        continue;
      case 't':
        if (p_active) {
          FLAGS->PT_FLAG->active = true;
          FLAGS->PT_FLAG->value = optarg;

          p_active = false;
        }
        break;
      case 'u':
        if (p_active) {
          FLAGS->PU_FLAG->active = true;
          FLAGS->PU_FLAG->value = optarg;

          p_active = false;
        }
        break;
      case '?':
        if (optopt == 't')
          cerr << "Error: Option \'-pt\' requires an argument.\n";
        else if (optopt == 'u')
          cerr << "Error: Option \'-pu\' requires an argument.\n";
        else if (optopt == 'i')
          cerr << "Error: Option \'-i\' requires an argument.\n";
        else if (isprint(optopt))
          cerr << "Error: Unknown option `-%c'.\n";
        else
          cerr << "Error: Unknown option character `\\x%x'.\n";
        exit(1);
      default:
        abort();
    }
  }

  /* Get the hostname value */
  if (optind == argc - 1)
    HOSTNAME->value = argv[optind];
  else {
    cerr << "Error: Invalid use of arguments.\n";
    exit(1);
  }

  /* Get the correct hostname type */
  HOSTNAME->type = getHostnameType(HOSTNAME->value);
}

vector<string> splitWithDelimiter(string rawText, string delimiter) {
	vector<string> result;
	size_t position = 0;
	string value;

	while((position = rawText.find(delimiter)) != string::npos) {
		value = rawText.substr(0, position);
    rawText.erase(0, position + delimiter.length());

		result.push_back(value);
	}

	/* Push the last value */
	result.push_back(rawText);

	return result;
}

vector<int> convertRange(char *rawValue) {
	vector<int> result;

  if (regex_match(rawValue, regex_ports_single)) {
    result.push_back(stoi(rawValue));
  } else if (regex_match(rawValue, regex_ports_multiple)) {
		vector<string> values = splitWithDelimiter(rawValue, ",");

		for(size_t i = 0; i < values.size(); i++)
    	result.push_back(stoi(values[i]));
	} else if (regex_match(rawValue, regex_ports_range)) {
		vector<string> values = splitWithDelimiter(rawValue, "-");

		for(size_t i = 0; i < values.size(); i++) {
    	result.push_back(stoi(values[i]));
		}

		/* Check if the first range number is lower than the second */
		if (result[0] <= result[1]) {
			int rangeStart = result[0];
			int rangeEnd = result[1];
			result.clear();

			for(int i = rangeStart; i <= rangeEnd; i++) {
				result.push_back(i);
			}
		} else {
			cerr << "Error: Invalid port range." << endl;
    	exit(1);
		}
	} else {
		cerr << "Error: Invalid port specification." << endl;
    exit(1);
	}

	return result;
}

int main(int argc, char **argv) {
	sFLAG *I_FLAG = new sFLAG();
	sFLAG *PT_FLAG = new sFLAG();
	sFLAG *PU_FLAG = new sFLAG();

	sFLAGS *FLAGS = new sFLAGS();
	FLAGS->I_FLAG = I_FLAG;
	FLAGS->PT_FLAG = PT_FLAG;
	FLAGS->PU_FLAG = PU_FLAG;

  sHOSTNAME *HOSTNAME = new sHOSTNAME();
	char error_buffer[PCAP_ERRBUF_SIZE];
	pcap_if_t *INTERFACES, *INTERFACE;
	char *INTERFACE_ADDR = NULL;

  /* Parse program arguments */
  parseArguments(argc, argv, FLAGS, HOSTNAME);
  if (HOSTNAME->type == UNKNOWN) {
    cerr << "Error: Unknown hostname value.\n";
    exit(1);
  }

	/* Choose the correct interface */
	if (!FLAGS->I_FLAG->active) {
		FLAGS->I_FLAG->value = pcap_lookupdev(error_buffer);

		if (FLAGS->I_FLAG->value == NULL) {
			fprintf(stderr, "Error: Unable to find a default interface: %s\n", error_buffer);
			exit(1);
		}
	} else {
		/* Get all network interfaces */
		if (pcap_findalldevs(&INTERFACES, error_buffer) == -1) {
			fprintf(stderr, "Error: pcap_findalldevs(): %s\n", error_buffer);
			exit(1);
		}

		/* Check if given interface is available */
		bool interface_ok = false;
		for(INTERFACE = INTERFACES; INTERFACE; INTERFACE = INTERFACE->next) {
			if (strcmp(INTERFACE->name, FLAGS->I_FLAG->value) == 0) {
				for(pcap_addr_t *a = INTERFACE->addresses; a != NULL; a = a->next) {
					if(a->addr->sa_family == AF_INET) {
						interface_ok = true;
						INTERFACE_ADDR = inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr);
						break;
					}
        }

				if (interface_ok) break;
			}
		}

		if (!interface_ok) {
			fprintf(stderr, "Error: Unable to find interface: %s\n", FLAGS->I_FLAG->value);
			exit(1);
		}
	}

  if (FLAGS->PT_FLAG->active) {
		/* Convert raw ranges to vectors */
		FLAGS->PT_FLAG->range = convertRange(FLAGS->PT_FLAG->value);

		TCP_Scanner TCP_SCANNER(HOSTNAME, INTERFACE_ADDR);

		for(size_t i = 0; i < FLAGS->PT_FLAG->range.size(); i++)
    	TCP_SCANNER.startScan(FLAGS->PT_FLAG->range[i]);
	}

  if (FLAGS->PU_FLAG->active) FLAGS->PU_FLAG->range = convertRange(FLAGS->PU_FLAG->value);

  return 0;
}