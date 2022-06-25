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
## Try to use PWM instead of GPIO to control LED
- PWM API: https://docs.kernel.org/driver-api/pwm.html
- Suggest experimenting with PWM through sysfs first: https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-class-pwm
- Remember to enable PWM function in config.txt # https://github.com/raspberrypi/firmware/blob/master/boot/overlays/README#L2894
- PWM expander if need more than two channels: https://www.adafruit.com/product/815