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

Check kernel message log.
```sh
dmesg
```

Will output a dependency list suitable for the modprobe utility.

```sh
depmod -A
```

Load module
```sh
modprobe {your-module-name}
```

Unload module
```sh
modprobe -r {your-module-name}
```

Check module major numbers
```sh
cat /proc/devices
```

Make node to talk to a device
```sh
mknod /dev/{device-name} c {major-number} {minor-number}
```

Check iomem mapping
```sh
cat /proc/iomem
```
