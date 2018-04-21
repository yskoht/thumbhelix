
# Thumbhelix

Thumbhelix is the thumb extension for [Helix keyboard](https://github.com/qmk/qmk_firmware/tree/master/keyboards/helix).

About Thumbhelix device, please see the following.

- [yskoht/thumbhelix\_device](https://github.com/yskoht/thumbhelix_device)


## Install

1. Clone [qmk\_firmware](https://github.com/qmk/qmk_firmware).

```bash
$ git clone https://github.com/qmk/qmk_firmware.git
```

2. Clone thumbhelix as submodule. Please replace [KEYMAP] with your keymap name.

```bash
$ git clone https://github.com/yskoht/thumbhelix.git qmk_firmware/users/[KEYMAP]/thumbhelix
```

3.  Add the following lines in your `config.h` and `rules.mk` to build thumbhelix.

**qmk_firmware/users/[KEYMAP]/config.h**

```c
#define USE_THUMBHELIX
#ifdef USE_THUMBHELIX
    #include "thumbhelix/config.h"
#endif
```

**qmk_firmware/users/[KEYMAP]/rules.mk**

```makefile
include $(USER_PATH)/thumbhelix/rules.mk
```

4.  Modify keymap.c .

**qmk_firmware/keyboards/helix/rev2/keymaps/[KEYMAP]/keymap.c**

```c
#ifdef USE_THUMBHELIX
  #include "thumbhelix.h"
  static bool th_scroll = false;
#endif

#ifdef USE_THUMBHELIX
void pointing_device_init(void)
{
  #ifndef SSD1306OLED
    TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 800000));
  #endif
}

void pointing_device_task(void)
{
  thumbhelix(th_scroll);
}
#endif
```

5.  If you put Thumbhelix on the right side, you have to set the right side to master, and vice versa.

**qmk_firmware/keyboards/helix/rev2/keymaps/[KEYMAP]/config.c**

```c
// #define MASTER_LEFT
#define MASTER_RIGHT
```

6. Build and upload.

Build

```bash
$ make helix:[KEYMAP]
```
If build script shows warning for checking file size, you have to delete some code.

Upload

```bash
$ sudo make helix:[KEYMAP]:avrdude
```


## Configurable parameters

[thumbhelix/config\.h](https://github.com/yskoht/thumbhelix/blob/master/config.h) has some configurable parameters. Please check the file.


## Example

- [qmk\_firmware/users/yskoht at yskoht](https://github.com/yskoht/qmk_firmware/tree/yskoht/users/yskoht)
- [qmk\_firmware/keyboards/helix/rev2/keymaps/yskoht at yskoht](https://github.com/yskoht/qmk_firmware/tree/yskoht/keyboards/helix/rev2/keymaps/yskoht)


## License

This software is released under the MIT License, see LICENSE.