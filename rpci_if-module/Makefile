all:
	${CC} -fPIC -c rcar_pci_api.c -I$(INCSHARED) -Wall -O
	${CC} -shared -Wl,-soname=librpci.so.1 -o librpci.so.1.0.0 rcar_pci_api.o
	ln -s ./librpci.so.1.0.0 librpci.so.1
	ln -s ./librpci.so.1 librpci.so

clean:
	rm -f rcar_pci_api.o librpci.so librpci.so.1 librpci.so.1.0.0

install:
	${CP} ./rcar_pci_api.h $(INCSHARED)/
