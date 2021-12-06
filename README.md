# Pine-Actions
PineActions is a basic re-implementation of some [Moto Actions](https://play.google.com/store/apps/details?id=com.motorola.actions&hl=en_US&gl=US) for the Pine Phone.

## Currently implemented actions:

 - [ ] Twist camera
 - [x] Twist flashlight

## TODO:

- AUR package

## Installing

From source:

```shell
git clone git@github.com:jpfeiffer16/Pine-Actions.git
cd Pine-Actions
make
make install
sudo systemctl enable twist-flashlight.service
sudo systemctl start twist-flashlight.service
```

AUR(Any Arch-based distro):

An aur package is being worked on now. For now you can just build from source.

## Thanks
- postmarketOS and their [docs](https://wiki.postmarketos.org/wiki/Phosh) for documentation of the gyro and led descriptors.
- [clickclack](https://git.sr.ht/~proycon/clickclack) for its insight into how to control the haptic motor.
