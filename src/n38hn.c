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
 * @brief  A port to 'C' of raspberry-strogonanoff.
 *         See https://github.com/himslm01/raspberry-strogonanoff
 *         and the original https://github.com/dmcg/raspberry-strogonanoff
 */

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <wiringPi.h>

#include <sys/time.h>

#include "n38hn.h"

static const uint8_t TRINARY_0[] =
{
        1,0,0,0,1,0,0,0
};
static const uint8_t TRINARY_1[] =
{
        1,1,1,0,1,1,1,0
};
static const uint8_t TRINARY_X[] =
{
        1,0,0,0,1,1,1,0
};
static const uint8_t SYNC[] =
{
        1,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0
};

static int verbose_flag;    ///< flag set by verbose command line parameter

int main(int argc, char *argv[])
{
    int c;

    int channel  = DEFAULT_CHANNEL;
    int button   = DEFAULT_BUTTON;
    int gpio     = DEFAULT_GPIO;
    int repeat   = DEFAULT_REPEAT;
    int us_delay = DEFAULT_US_DELAY;
    int on = 0;

    int buffer_size;
    uint8_t *buffer;

    while (1)
    {
        static struct option long_options[] =
            {
                    /* These options set a flag. */
                    {"verbose", no_argument, &verbose_flag, 1},
                    {"button",  required_argument, 0, 'b'},
                    {"channel", required_argument, 0, 'c'},
                    {"gpio",    required_argument, 0, 'g'},
                    {"repeat",  required_argument, 0, 'r'},
                    {"delay",   required_argument, 0, 'd'},
                    {0, 0, 0, 0}
            };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "vb:c:d:g:r:",
                        long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
        case 0:
            /* If this option set a flag, do nothing else now. */
            if (long_options[option_index].flag != 0)
                break;
            break;
        case 'v':
            verbose_flag = 1;
            break;
        case 'b':
            button = (int)strtol(optarg, (char **)NULL, 10);
            if (button < 1 || button > 4)
                help();
            break;
        case 'c':
            channel = (int)strtol(optarg, (char **)NULL, 10);
            if (channel < 1 || channel > 4)
                help();
            break;
        case 'd':
            us_delay = (int)strtol(optarg, (char **)NULL, 10);
            if (us_delay < 1 || us_delay > INT_MAX)
                help();
            break;
        case 'g':
            gpio = (int)strtol(optarg, (char **)NULL, 10);
            if (gpio < 0 || gpio > 16)
                help();
            break;
        case 'r':
            repeat = (int)strtol(optarg, (char **)NULL, 10);
            if (repeat < 1 || repeat > 255)
                help();
            break;
        default:
            help();
            break;
        }
    }
    if (optind == argc)
    {
        on = 1;
    }
    else if (optind+1 == argc)
    {
        if (!(strcmp("off",argv[optind])))
            on = 0;
        else if (!(strcmp("on",argv[optind])))
            on = 1;
        else
            help();
    }
    else
    {
        help();
    }

    if (verbose_flag)
    {
        printf("gpio: %d channel: %d button: %d state: %s\n",
                gpio, channel, button, (on ? "on" : "off"));
    }

    if (wiringPiSetup() == -1)
    {
        error(ABORT_WIRINGPI);
    }
    pinMode(gpio, OUTPUT);

    buffer_size = repeat * (12 * sizeof(TRINARY_0) + sizeof(SYNC));
    if (verbose_flag)
    {
        printf("bits to send: %d\n", buffer_size);
    }

    if (!(buffer = calloc(buffer_size,1)))
        error(ABORT_NO_FREE_TX);

    encode_bitstream(buffer, channel, button, on, repeat);

    if (verbose_flag)
    {
        printf("data: ");
        for(c = 0; c < buffer_size; c++)
        {
            printf("%d",buffer[c]);
        }
        printf("\n");
    }

    send_bitstream(buffer, buffer_size, gpio, us_delay);

    return 0;
}

uint8_t *add_data(uint8_t *destination, const uint8_t *source, int length)
{
    memcpy(destination, source, length);
    destination += length;
    return destination;
}

void busy_wait_until(uint64_t end)
{
    struct timeval tv;
    uint64_t now;

    do
    {
        if (gettimeofday(&tv, NULL) < 0)
            error(ABORT_GETTIMEOFDAY);
        now = tv.tv_sec * 1000000ll + tv.tv_usec;
    }
    while (now <= end);
}

uint16_t encode_message_frame(const int channel, const int button, const int on)
{
    uint8_t channel_code = ~(1 << (channel-1)) & 0x0f;
    uint8_t button_code  = ~(1 << (button-1))  & 0x0f;
    uint8_t pad_code     = 0b110;
    uint8_t state_code   = on ? 0b1 : 0b0;

    return state_code << 11 | pad_code << 8 | button_code << 4 | channel_code;
}

void encode_bitstream(uint8_t *buffer, const int channel, const int button, const int on, const int repeat)
{
    uint16_t message_frame = encode_message_frame(channel, button, on);
    uint8_t *current_buffer_pointer = buffer;
    int r;

    for (r = 0; r < repeat; r++)
    {
        int b;
        uint16_t message_temp = message_frame;

        for (b = 0; b < 12; b++)
        {
            if (message_temp & 0b01)
            {
                current_buffer_pointer = add_data(current_buffer_pointer,
                            TRINARY_X, sizeof(TRINARY_X));
            }
            else
            {
                current_buffer_pointer = add_data(current_buffer_pointer,
                            TRINARY_0, sizeof(TRINARY_0));

            }
            message_temp = message_temp >> 1;
        }
        current_buffer_pointer = add_data(current_buffer_pointer,
                SYNC, sizeof(SYNC));
    }
}

void send_bitstream(const uint8_t *buffer, const int buffer_size, const int gpio, const int us_delay)
{
    struct timeval tv;
    uint64_t end_time;
    int c;

    if (gettimeofday(&tv, NULL) < 0)
        error(ABORT_GETTIMEOFDAY);

    end_time = tv.tv_sec * 1000000ll + tv.tv_usec;

    for(c = 0; c < buffer_size; c++)
    {
        digitalWrite(gpio, buffer[c]);
        end_time = end_time + us_delay;
        busy_wait_until(end_time);
    }
}

void error(const char *message)
{
    printf("ABORT!\n%s\n", message);
    exit(1);
}

void help()
{
    printf(HELP);
    exit(1);
}
