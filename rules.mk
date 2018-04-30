ifeq ("$(patsubst %/,%,$(dir $(KEYBOARD)))", "helix")

SRC += thumbhelix/thumbhelix.c \
       lib/lufa/LUFA/Drivers/Peripheral/AVR8/TWI_AVR8.c \

EXTRAINCDIRS += $(USER_PATH)/thumbhelix

POINTING_DEVICE_ENABLE = yes

OPT_DEFS += -DUSE_THUMBHELIX

endif

