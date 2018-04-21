#ifndef THUMBHELIX_CONFIG_H
#define THUMBHELIX_CONFIG_H

// I2C slave address
#define THUMBHELIX_ADDR 0x0A

// Cursor sensitivity
// [0.0f(sensitive) <= THUMBHELIX_SENSITIVITY <= 1.0f(insensitive)]
#define THUMBHELIX_SENSITIVITY 0.7f

// Cursor speed
// [1(slow) <= THUMBHELIX_CURSOR_SPEED <= 10(rapid)]
#define THUMBHELIX_CURSOR_SPEED 4

#define THUMBHELIX_ANALOG_MAX 1023

#define THUMBHELIX_CENTER_RANGE_MAX 600
#define THUMBHELIX_CENTER_RANGE_MIN 400

#endif
