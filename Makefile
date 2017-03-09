#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)

all:
	source setup-env.sh && platformio run

upload:
	source setup-env.sh && platformio run --target upload

clean:
	platformio run --target clean

program:
	platformio run --target program

uploadfs:
	platformio run --target uploadfs

update:
	platformio update

secrets:
	[ ! -f setup-env.sh ] && cp setup-env.sh.tmpl setup-env.sh
