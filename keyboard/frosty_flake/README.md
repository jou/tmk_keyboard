Kitten Paw controller firmware
======================
Custom controller for the Cooler Master QuickFire Rapid keyboard designed by bpiphany.

*Note that this is not the official firmware*

Build
-----
Move to this directory then just run `make` like:

    $ make -f Makefile.lufa

At the moment only the LUFA stack is supported.

By default, it builds the firmware for hardware revision 20140521. To build for hardware revision 20130602, specify it in the `make` command:

    $ make -f Makefile.lufa HW_REVISION=20130602

Bootloader
---------
To enter bootloader by hardware use a magnet above the controller before connecting the usb cable.

It is still possible to use Boot Magic and Command (LSFT+RSFT+PAUS) to access the bootloader though.
