# Uncomment lines below if you have problems with $PATH
#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)

all:
	source setup-env.sh && platformio run

upload:
	platformio --target upload

clean:
	platformio --target clean

program:
	platformio --target program

uploadfs:
	platformio --target uploadfs

update:
	platformio update
