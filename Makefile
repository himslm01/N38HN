#
# (C) 2013 Mark Himsley
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#

V = 0
Q_0 := @
Q_1 := 
Q = $(Q_$(V))
QECHO_0 = @echo
QECHO_1 = @true
QECHO = $(QECHO_$(V))

DEBUG   = -O3

CC      = gcc
LD      = $(CC)

INCLUDE = -I/usr/local/include
CFLAGS  = $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS = -L/usr/local/lib
LDLIBS  = -lwiringPi

export

src :
	cd src/ && $(MAKE) $(MAKECMDGOALS)

all : src

install : all
	$(QECHO) "[Install]"
	$(Q)install -m 0755 -d /home/pi/bin
	$(Q)install -m 4555 -g root -o root -s src/n38hn /home/pi/bin/

clean : src

.PHONY : src all install clean
