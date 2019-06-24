all:
	$(MAKE) -f Makefile.linux -B all
	$(MAKE) -f Makefile.avr -B all

clean:
	$(MAKE) -f Makefile.linux -B clean
	$(MAKE) -f Makefile.avr -B clean
