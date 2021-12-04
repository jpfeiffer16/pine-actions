build:
	mkdir -p ./bin
	gcc -o ./bin/flashlight ./flashlight.c

install:
	cp ./bin/flashlight /usr/bin
	cp ./pine-actions-flashlight.service /usr/lib/systemd/system/

uninstall:
	rm /usr/bin/flashlight
	rm /usr/lib/systemd/system/pine-actions-flashlight.service
