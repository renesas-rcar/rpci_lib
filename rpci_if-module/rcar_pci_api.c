/*
 * Copyright (C) 2024 Renesas Electronics Corporation
 * Released under the MIT license
 * https://opensource.org/license/mit
 */

#include <stdio.h>

#include "rcar_pci_api.h"

int rcar_pci_transmit(int fd, void *buffer, size_t size)
{
	struct rcar_pci_xfer_buff buff;

	buff.buff = buffer;
	buff.size = size;

	if (!buff.size)
		return 0;

	return ioctl(fd, RCAR_PCI_XMIT, &buff);
}

int rcar_pci_has_rx_data(int fd, size_t *size)
{
	*size = 0;

	return ioctl(fd, RCAR_PCI_HAS_RX_DATA, size);
}

int rcar_pci_receive(int fd, void *buffer, size_t size)
{
	struct rcar_pci_xfer_buff buff;

	buff.buff = buffer;
	buff.size = size;

	if (!buff.size)
		return 0;

	return ioctl(fd, RCAR_PCI_RECEIVE, &buff);
}
