Yet Another ECU
===============

What is is?
-----------

*Yet Another ECU* is a software framework for engine control unit. It provides
clean programming interfaces for all the elements involved in a typical ECU. The
objective is to enable the re-usability of the source code across different
microprocessors and engine configurations.

One complete ECU firmware implementation using this framework is in the work: a
ATMEGA2561 based ECU running a VW type 4 engine with a 36-1 trigger wheel.


Installation
------------

The supported build platform is Linux, and the targets are Linux and AVR. The
Linux target is used for unit tests and simulation. The AVR target is the actual
hardware target. More targets may be added in the future.

Each target is compiled using its own make file named `Makefile.<target>`.

To compile the Linux simulator, type `make -f Makefile.linux -B`

To compile the AVR target, type `make -f Makefile.avr -B`

Note: the Make files do not calculate the header dependencies, so it is
important to use the `-B` argument to make sure everything gets compiled after
you modify a header file.

The compiler used for both targets is GCC. The default GCC version provided by
your distribution will do fine. The recommended AVR-GCC compiler is the one you
can download from Microchip.

I went away from the Arduino framework as I wanted full control over the
interrupt scheme, keeping the system as real-time as possible. The ECU board I
am using is also not supported by Arduino.


How to contribute
-----------------

Please report issues and submit pull requests using Github.


License
-------

    Yet Another ECU
    Copyright (C) 2019  Cedric Priscal

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see
    [https://www.gnu.org/licenses/](https://www.gnu.org/licenses/).


Authors
-------

- Cedric Priscal

