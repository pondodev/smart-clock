# smart clock
a smart clock with an e-ink display, all powered by a raspberry pi pico.

## project status
i'm still very much prototyping things still while i figure out how to write code for the raspberry pi pico and how to
make circuits that don't burst into flames (i've already destroyed one dht22 in the making of this project). the
structure of the project is going to be a bit of a mess for a while, especially the kicad project, but i promise to
clean it up eventually so it's not so hard to navigate.

## schematics
super work in progress. i barely have things together right now, but i plan to at the very least have the schematics
together so that it's easy enough to recreate this project if you want to. i won't be creating a pcb for this however,
cause frankly that's a bit of a waste of money and resources, so i'll likely just stick this all onto a prototype pcb as
my final product.

## firmware
i'm writing this project in c using the pico sdk. hopefully the project structure is easy enough to follow, and i hope
i've done my job properly so that the code can be relatively easily transported from platform to platform if you decide
that you want to port it to a different mcu. i'll drop some more info on that when i'm closer to finalising the firmware
though.

