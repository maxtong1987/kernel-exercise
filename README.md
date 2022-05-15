# README

## Prerequisites

- install linux header in Raspberry Pi
```
apt install raspberrypi-kernel-headers
```

## simple-led

### build modules

```sh
make modules
```

### to install modules

```sh
make modules_install
```

## Common commands

Will output a dependency list suitable for the modprobe utility.

```sh
demod -A
```

Load module
```sh
modprobe {your-module-name}
```

Check module major numbers
```sh
cat /proc/devices
```

Make node to talk to a device
```sh
mknode c /dev/{device-name} {major-number} {minor-number}
```
