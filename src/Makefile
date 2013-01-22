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

SRC     = n38hn.c
OBJ     = $(SRC:.c=.o)
BINS    = n38hn

n38hn : $(OBJ)
	$(QECHO) "[LD] $< > $@"
	$(Q)$(LD) -o $@ $< $(LDFLAGS) $(LDLIBS)

.c.o :
	$(QECHO) "[CC] $< > $@"
	$(Q)$(CC) -o $@ $< -c $(CFLAGS)

all : $(BINS)

install : all

clean : 
	$(QECHO) "[Clean]"
	$(Q)rm -f *~ $(OBJ) $(BINS)

.PHONY : all install clean
