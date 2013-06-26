IBM5150
=======

An emulator of the IBM PC 5150, and its descendants and clones.

IRC Channel
===========

Join x86emulator channel at freenode to talk about the emulator.

Compiling and running
=====================

So far, this emulator works only on Unix systems. It was tested on Ubuntu and Arch Linux.

To compile IBM5150 you will need to install g++, yasm and SDL. After you've done that run:

cd IBM5150; make; ./ibm5150 biosFile mdaFile mda.cfg > log.txt

You should see a black window on the screen and log.txt should contain everything the emulator has been doing.

You can get character ROM from http://www.minuszerodegrees.net/rom/IBM_5788005_AM9264_1981_CGA_MDA_CARD.BIN and BIOS from http://www.minuszerodegrees.net/bios/bios.htm.

Todo
====

ISA DMA
-------

I don't quite understand how transfers are started, but then again, I haven't even started on this.

MDA
---

Only updates every frame. Might want to change that eventually.

CGA
---

Right now, it's just in there as a slightly modified MDA clone. Lots to be done.

8088
----

Only accurate on an opcode-level. No regards to actual execution times. This will have to wait until the 8086/8088 microcode is dumped.

80286
-----

VERY rudimentary. Not really needed right now anyway.

PIT
---

Not complete, but the basics are there.

PPI
---

Much to be done here.

PIC
---

It's sorta there, but still incomplete.
