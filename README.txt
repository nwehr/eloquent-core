DEPENDENCIES
============
Tools:
- CMake
- C++ compiler

Libraries:
- boost
- streamlog

INSTALL / RUN (source)
======================
1. Compile and install
	$ git clone https://bitbucket.org/nwehr/eloquent.git eloquent
	$ cd eloquent
	$ cmake ./
	$ make && sudo make install
	
2.1. Run (Darwin)
	$ sudo launchctl start com.evrichart.eloquentd

2.2. Run (Linux)
	$ sudo start eloquentd


INSTALL / RUN (MacPorts)
========================
$ sudo port install eloquent
$ sudo port load eloquent
