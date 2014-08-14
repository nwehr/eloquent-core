DEPENDENCIES
============
Tools:
- CMake
- C++11-compatable compiler

Libraries:
- boost >= 1.55
- streamlog

INSTALL / RUN (source)
======================
1. Compile and install (core)
	$ git clone https://bitbucket.org/nwehr/eloquent.git
	$ cd eloquent				# <eloquent-srcroot>
	$ git submodule init		# initialize submodules
	$ git submodule update		# update submodules
	$ cd core
	$ mkdir build && cd build
	$ cmake ../
	$ make && sudo make install

2. Compile and install (extensions)
	$ cd <eloquent-srcroot>/ext-<Library>
	$ mkdir build && cd build
	$ cmake ../
	$ make && sudo make install

3.1. Run (Darwin)
	$ sudo launchctl start com.evrichart.eloquentd

3.2. Run (Linux)
	$ sudo start eloquentd


CONFIGURE
=========
1. You may need to configure one or more of the following files:
/usr/local/etc/eloquent/eloquent.conf
/usr/local/etc/eloquent/extensions.conf

2.1 Daemon Config (Darwin)
/Library/LaunchDaemons/com.evrichart.eloquentd.plist

2.2 Daemon Config (Linux)
/etc/init/eloquentd.conf
