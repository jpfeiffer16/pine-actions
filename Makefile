build:
	mkdir -p ./bin
	gcc -o ./bin/twist-flashlight ./twist-flashlight.c

install:
	cp ./bin/twist-flashlight ${DESTDIR}/usr/bin
	cp ./twist-flashlight.service ${DESTDIR}/usr/lib/systemd/system/

uninstall:
	rm ${DESTDIR}/usr/bin/twist-flashlight
	rm ${DESTDIR}/usr/lib/systemd/system/twist-flashlight.service
