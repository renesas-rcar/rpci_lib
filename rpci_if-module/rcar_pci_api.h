/*
 * Copyright (C) 2024 Renesas Electronics Corporation
 * Released under the MIT license
 * https://opensource.org/license/mit
 */

#ifndef __RCAR_PCI_API_H
#define __RCAR_PCI_API_H

#include <errno.h>
#include <sys/ioctl.h>

#define RCAR_PCI_HAS_RX_DATA            _IOW('V', 0, size_t)
#define RCAR_PCI_RECEIVE                _IOW('V', 1, struct rcar_pci_xfer_buff)
#define RCAR_PCI_XMIT                   _IOW('V', 2, struct rcar_pci_xfer_buff)

struct rcar_pci_xfer_buff {
	void *buff;
	size_t size;
};

int rcar_pci_transmit(int fd, void *buffer, size_t size);
int rcar_pci_has_rx_data(int fd, size_t *size);
int rcar_pci_receive(int fd, void *buffer, size_t size);

#endif /* __RCAR_PCI_API_H */
