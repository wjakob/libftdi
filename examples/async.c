/* LIBFTDI EEPROM access example

   This program is distributed under the GPL, version 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <ftdi.h>

int main(int argc, char **argv)
{
    struct ftdi_context *ftdi;
    int do_read = 0;
    int do_write = 0;
    int i, f, retval;

    if ((ftdi = ftdi_new()) == 0)
    {
        fprintf(stderr, "Failed to allocate ftdi structure :%s \n",
                ftdi_get_error_string(ftdi));
        return EXIT_FAILURE;
    }

    while ((i = getopt(argc, argv, "brw")) != -1)
    {
        switch (i)
        {
            case 'b':
                do_read = 1;
                do_write = 1;
                break;
            case 'r':
                do_read = 1;
                break;
            case 'w':
                do_write  = 1;
                break;
            default:
                fprintf(stderr, "usage: %s [options]\n", *argv);
                fprintf(stderr, "\t-b do synchronous read and write\n");
                fprintf(stderr, "\t-r do synchronous read\n");
                fprintf(stderr, "\t-w do synchronous write\n");
                retval = -1;
                goto done;
        }
    }

    // Select first interface
    ftdi_set_interface(ftdi, INTERFACE_ANY);

    struct ftdi_device_list *devlist;
    int res;
    if ((res = ftdi_usb_find_all(ftdi, &devlist, 0, 0)) < 0)
    {
        fprintf(stderr, "No FTDI with default VID/PID found\n");
        retval =  EXIT_FAILURE;
        goto do_deinit;
    }
    if (res > 0)
    {
        int i = 1;
        f = ftdi_usb_open_dev(ftdi, devlist[0].dev);
        if (f < 0)
        {
            fprintf(stderr, "Unable to open device %d: (%s)",
                    i, ftdi_get_error_string(ftdi));
            retval = -1;
            goto do_deinit;
        }
    }
    else
    {
        fprintf(stderr, "No devices found\n");
        retval = -1;
        goto do_deinit;
    }
    ftdi_list_free(&devlist);
    int err = ftdi_usb_purge_buffers(ftdi);
    if (err != 0) {
        fprintf(stderr, "ftdi_usb_purge_buffer: %d: %s\n",
                err, ftdi_get_error_string(ftdi));
        retval = -1;
        goto do_deinit;
    }
    /* Reset MPSSE controller. */
    err = ftdi_set_bitmode(ftdi, 0,  BITMODE_RESET);
    if (err != 0) {
        fprintf(stderr, "ftdi_set_bitmode: %d: %s\n",
                err, ftdi_get_error_string(ftdi));
        retval = -1;
        goto do_deinit;
   }
    /* Enable MPSSE controller. Pin directions are set later.*/
    err = ftdi_set_bitmode(ftdi, 0, BITMODE_MPSSE);
    if (err != 0) {
        fprintf(stderr, "ftdi_set_bitmode: %d: %s\n",
                err, ftdi_get_error_string(ftdi));
        return -1;
    }
    uint8_t ftdi_init[15] = {TCK_DIVISOR, 0x00, 0x00,
                             SET_BITS_HIGH, 0, 0xff,
                             GET_BITS_HIGH,
                             SET_BITS_HIGH, 0x55, 0xff,
                             GET_BITS_HIGH,
                             SET_BITS_HIGH, 0xaa, 0xff,
                             GET_BITS_HIGH};
    struct ftdi_transfer_control *tc_read;
    struct ftdi_transfer_control *tc_write;
    uint8_t data[3];
    if (do_read) {
        tc_read = ftdi_read_data_submit(ftdi, data, 3);
    }
    if (do_write) {
        tc_write = ftdi_write_data_submit(ftdi, ftdi_init, 15);
        int transfer = ftdi_transfer_data_done(tc_write);
        printf("Async write %d\n", transfer);
    } else {
        int written = ftdi_write_data(ftdi, ftdi_init, 15);
        if (written != 15) {
            printf("Sync write failed\n");
        }
    }
    if (do_read) {
        int transfer = ftdi_transfer_data_done(tc_read);
        printf("Async Read %d\n", transfer);
    } else {
        int index = 0;
        while (index < 3) {
            int res = ftdi_read_data(ftdi, data + index, 3 - index);
            if (res < 0) {
                printf("Async read failure at %d\n", index);
            } else {
                index += res;
            }
        }
    }
    printf("Read %02x %02x %02x\n", data[0], data[1], data[2]);
done:
    ftdi_usb_close(ftdi);
do_deinit:
    ftdi_free(ftdi);
    return retval;
}
