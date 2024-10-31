all:
	${CC} rcar_pci_host.c -o rcar_pci_host -I$(INCSHARED) -L$(LIBSHARED) -lrpci -O
	${CC} rcar_pci_ep.c -o rcar_pci_ep -I$(INCSHARED) -L$(LIBSHARED) -lrpci -O

host:
	${CC} rcar_pci_host.c -o rcar_pci_host -I$(INCSHARED) -L$(LIBSHARED) -lrpci -O

ep:
	${CC} rcar_pci_ep.c -o rcar_pci_ep -I$(INCSHARED) -L$(LIBSHARED) -lrpci -O

clean:
	rm -f rcar_pci_host
	rm -f rcar_pci_ep
