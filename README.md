TSC Settingseditor
==================

This is a specialised editor for editing the `.settings` files of the
[TSC game](http://www.secretchronicles.de).

Dependencies
------------

The editor depends on wxWidgets, at least version 3.0.0. So you have
to have that installed, including the GDI graphical context library as
that is used for drawing the main editing widget.

Building
--------

Clone the repository and initialise the Git submodules:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
$ git clone git://github.com/Secretchronicles/TSC-Settingseditor.git
$ git submodule init
$ git submodule update
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The rest is a regular CMake build:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
$ mkdir build
$ cd build
$ cmake ..
$ make
# make install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you don’t want to install to `/usr/local`, the default, set the
`CMAKE_INSTALL_PREFIX` variable accordingly before compilation. For
example:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
$ cmake -DCMAKE_INSTALL_PREFIX=/opt/tsc-settingseditor ..
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

License
-------

A specialised TSC settings files editor.
Copyright © 2015 Marvin Gülker

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
