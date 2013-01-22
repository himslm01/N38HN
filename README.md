# NAME
N38HN

# SYNOPSIS
**n38hn** [OPTIONS] [on|off]

# DESCRIPTION
A Raspberry Pi Remote Mains Switcher, to switch these

    http://www.maplin.co.uk/remote-controlled-mains-sockets-5-pack-348217

with one of these

    http://proto-pic.co.uk/434mhz-rf-link-transmitter/

using the documentation of the protocol detailed here

    http://elektronikforumet.com/wiki/index.php/RF_Protokoll_-_Nexa/Proove_%28%C3%A4ldre,_ej_sj%C3%A4lvl%C3%A4rande%29

Based on Raspberry Strogonanoff. See the original:

    https://github.com/dmcg/raspberry-strogonanoff

and my re-implementation:

    https://github.com/himslm01/raspberry-strogonanoff

Ported from Python to be faster, more accurate, and can be installed suid.

# OPTIONS

    -v, --verbose   enable verbose logging
    -g, --gpio      the GPIO pin of the Raspberry Pi to control (default is 0)
    -c, --channel   'channel' number to send to
                    The 'channel' number sets the 'code' group to send.
                    On the receiving socket device the 'code adjustment dial'
                    is labelled I, II, III & IV, the same as on the remote
                    control's 'code adjustment switch', but this parameter
                    requires 1, 2, 3 or 4 (default is 1).
    -b, --button    'button' number to 'press'
                    The remote control has 8 buttons arranged in 4 rows. This
                    parameter sets the row of buttons being 'pressed'. On the
                    receiving socket device the 'channel adjustment dial'
                    sets which button row corresponds to which receiving
                    socket device, values are 1, 2, 3 or 4 (default is 1)
    -r, --repeat    how many times to repeat the command
                    the receivers usually require 4 or more repeats before they react.
                    The minimum is 1, the maximum is 255 (default is 6)
    -d, --delay     shortest period of transmit on or off in micro-seconds
                    The 'T' time as described in the protocol document. You can vary
                    this to determine if your receivers are tuned correctly, they
                    accept a range of about 300 - 500 (default is 350)
    [on|off]        whether to send a 'turn on' command or a 'turn off'
                    command (default is on)

# INSTALLATION

Requires WiringPi

    $ git clone https://github.com/WiringPi/WiringPi.git
    $ pushd WiringPi
    $ sudo ./build
    $ popd

    $ git clone https://github.com/himslm01/N38HN.git
    $ pushd N38HN
    $ make
    (optional)
    $ sudo make install
    $ popd
 
Needs to be run as root - either sudo the version of n38hn built in N38HN/src
or continue to the install phase and execute /home/pi/bin/n38hn.

# EXAMPLE

    n38hn --channel 1 --button 3 --gpio 8 on 

where the GPIO pin numbers are the big ones on this diagram ![](http://pi4j.com/images/p1header-large.png)

# LICENCE

(C) 2013 Mark Himsley

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
