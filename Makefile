.PHONY: all clean

all:
	$(MAKE) -C Task1/AF_INET/TCP
	$(MAKE) -C Task1/AF_INET/UDP
	$(MAKE) -C Task1/AF_LOCAL/TCP
	$(MAKE) -C Task1/AF_LOCAL/UDP

clean:
	$(MAKE) -C Task1/AF_INET/TCP clean
	$(MAKE) -C Task1/AF_INET/UDP clean
	$(MAKE) -C Task1/AF_LOCAL/TCP clean
	$(MAKE) -C Task1/AF_LOCAL/UDP clean
