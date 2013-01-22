/*
 * (C) 2013 Mark Himsley
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @file   n38hn.c
 * @author Mark Himsley
 * @date   21 Jan 2013
 */

#define DEFAULT_CHANNEL 1
#define DEFAULT_BUTTON 1
#define DEFAULT_GPIO 0
#define DEFAULT_REPEAT 6
#define DEFAULT_US_DELAY 350

#define HELP "\
-v, --verbose  be verbose\n\
-c, --channel  the channel to send, 1-4 (default is 1)\n\
-b, --button   the button to send, 1-4 (default is 1)\n\
-g, --gpio     which wiringPi GPIO to use, 0-16 (default is 0)\n\
-r, --repeat   how many times to repeat the command (default is 6)\n\
-d, --delay    shortest period of transmit on or off in micro-seconds (default is 350)\n\
[on|off]       send an or or off command (default is on)\n"

#define ABORT_WIRINGPI "WiringPi setup failed"
#define ABORT_NO_FREE_TX "No free memory to allocate TX buffer"
#define ABORT_GETTIMEOFDAY "gettimeofday failed"

uint8_t *add_data(uint8_t *destination, const uint8_t *source, int length);
void busy_wait_until(uint64_t end);
uint16_t encode_message_frame(const int channel, const int button, const int on);
void encode_bitstream(uint8_t *buffer, const int channel, const int button, const int on, const int repeat);
void send_bitstream(const uint8_t *buffer, const int buffer_size, const int gpio, const int us_delay);
void error(const char *message);
void help();
