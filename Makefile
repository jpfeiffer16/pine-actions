build:
	mkdir -p ./bin
	gcc -o ./bin/twist-flashlight ./twist-flashlight.c

build-utils:
	mkdir -p ./bin
	gcc -o ./bin/gyro_monitor ./gyro_monitor.c

install:
	cp ./bin/twist-flashlight ${DESTDIR}/usr/bin
	cp ./twist-flashlight.service ${DESTDIR}/etc/superd/services/

install-systemd:
	cp ./bin/twist-flashlight ${DESTDIR}/usr/bin
	cp ./twist-flashlight.service ${DESTDIR}/usr/lib/systemd/system/

uninstall:
	rm ${DESTDIR}/usr/bin/twist-flashlight
	rm ${DESTDIR}/usr/lib/systemd/system/twist-flashlight.service
	rm ${DESTDIR}/etc/superd/services/twist-flashlight.service
