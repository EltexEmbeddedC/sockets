.PHONY: all clean

all:
	$(MAKE) -C Task1/AF_INET/TCP
	$(MAKE) -C Task1/AF_INET/UDP
	$(MAKE) -C Task1/AF_LOCAL/TCP
	$(MAKE) -C Task1/AF_LOCAL/UDP
	$(MAKE) -C Task2/Type1
	$(MAKE) -C Task2/Type2
	$(MAKE) -C Task2/Type3
	$(MAKE) -C Task2/Type4
	$(MAKE) -C Task3
	$(MAKE) -C Task4
	$(MAKE) -C Task5
	$(MAKE) -C Task6
	$(MAKE) -C Task7

clean:
	$(MAKE) -C Task1/AF_INET/TCP clean
	$(MAKE) -C Task1/AF_INET/UDP clean
	$(MAKE) -C Task1/AF_LOCAL/TCP clean
	$(MAKE) -C Task1/AF_LOCAL/UDP clean
	$(MAKE) -C Task2/Type1 clean
	$(MAKE) -C Task2/Type2 clean
	$(MAKE) -C Task2/Type3 clean
	$(MAKE) -C Task2/Type4 clean
	$(MAKE) -C Task3 clean
	$(MAKE) -C Task4 clean
	$(MAKE) -C Task5 clean
	$(MAKE) -C Task6 clean
	$(MAKE) -C Task7 clean
