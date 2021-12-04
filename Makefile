build:
	mkdir -p ./bin
	gcc -o ./bin/flashlight ./flashlight.c

install:
	cp ./bin/flashlight ${DESTDIR}/usr/bin
	cp ./pine-actions-flashlight.service ${DESTDIR}/usr/lib/systemd/system/

uninstall:
	rm ${DESTDIR}/usr/bin/flashlight
	rm ${DESTDIR}/usr/lib/systemd/system/pine-actions-flashlight.service
