// SPDX-License-Identifier: GPL-2.0-only

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <getopt.h>
#include <poll.h>

#include "rcar_pci_api.h"

void usage(const char *app_name) {
	printf("Usage: %s -tx | -rx [-s <size>] [-c <count>]\n", app_name);
	printf("Options:\n");
	printf("  -t, --tx            Enable transmit mode (mandatory if -rx is not provided).\n");
	printf("  -r, --rx            Enable receive mode (mandatory if -tx is not provided).\n");
	printf("  -s, --size <value>  Set the transmission size (default: 11).\n");
	printf("  -c, --count <value> Set the number of transmissions (default: 1).\n");
	exit(1);
}

static struct option long_options[] = {
	{"tx", no_argument, 0, 't'},
	{"rx", no_argument, 0, 'r'},
	{"size", required_argument, 0, 's'},
	{"count", required_argument, 0, 'c'},
	{0, 0, 0, 0}
};

void transmit(int fd, int size, int count)
{
	void *user_buffer;
	int i, j, ret;

	for (j = 0; j < count; j++) {
		user_buffer = malloc(size);

		for (i = 0; i < size; i++)
			*((char *)(user_buffer) + i) = (char) i + j;

		ret = rcar_pci_transmit(fd, user_buffer, size);
		if (ret)
			printf("TX%d: Failed\n", j);

		free(user_buffer);
	}
}

void receive(int fd) {
	struct pollfd fds;

	fds.fd = fd;
	fds.events = POLLIN;

	while (1) {
		void *user_buffer;
		size_t size;
		int i, ret;

		poll(&fds, 1, -1);

		if (fds.revents & POLLIN) {
			rcar_pci_has_rx_data(fd, &size);

			if (size <= 0) {
				continue;
			}

			user_buffer = malloc(size);

			ret = rcar_pci_receive(fd, user_buffer, size);
			if (ret < 0) {
				printf("RX: Failed\n");
			} else {
				printf("-------- RX SIZE = %ld ---------\n", size);
				for (i = 0; i < size; i++) {
					printf("%2.2x ", *((char *)(user_buffer) + i));
					if (!((i + 1) % 32))
						printf("\n");
				}
				printf("\n");
			}

			free(user_buffer);
		}
	}
}

int main(int argc, char **argv)
{
	int tx_mode = 0;
	int rx_mode = 0;
	int size = 11;
	int count = 1;
	int opt;
	int option_index = 0;
	int fd;

	while ((opt = getopt_long(argc, argv, "trs:c:", long_options, &option_index)) != -1) {
		switch (opt) {
		case 't':
			tx_mode = 1;
			break;
		case 'r':
			rx_mode = 1;
			break;
		case 's':
			size = atoi(optarg);
			break;
		case 'c':
			count = atoi(optarg);
			break;
		default:
			usage(argv[0]);
		}
	}

	fd = open("/dev/rcar-pci-ep0", O_RDWR);
	if (fd < 0)
		return fd;

	if (!tx_mode && !rx_mode) {
		printf("Error: Either -tx or -rx must be provided.\n");
		usage(argv[0]);
	}

	if (tx_mode && rx_mode) {
		printf("Error: Cannot use both -tx and -rx at the same time.\n");
		usage(argv[0]);
	}

	if (tx_mode) {
		if (size < 0) {
			printf("Error: Size must be not negative.\n");
			exit(-EINVAL);
		}

		transmit(fd, size, count);
	} else if (rx_mode) {
		receive(fd);
	}

	close(fd);

	return 0;
}
