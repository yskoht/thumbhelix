#ifndef THUMBHELIX_CONFIG_H
#define THUMBHELIX_CONFIG_H

// I2C slave address
#define THUMBHELIX_ADDR 0x0A

// Extension key behavior
#define THUMBHELIX_KEY TH_BTN3

// Cursor sensitivity
// [0.0f(sensitive) <= THUMBHELIX_SENSITIVITY <= 1.0f(insensitive)]
#define THUMBHELIX_SENSITIVITY 0.7f

// Cursor speed
// [1(slow) <= THUMBHELIX_CURSOR_SPEED <= 10(rapid)]
#define THUMBHELIX_CURSOR_SPEED 4

// Horizontal scroll speed
#define THUMBHELIX_H_SCROLL_SPEED 5

// Vertical scroll speed
#define THUMBHELIX_V_SCROLL_SPEED 2

#define THUMBHELIX_SCROLL_INTERVAL 50

#define THUMBHELIX_ANALOG_MAX    1023
#define THUMBHELIX_NORMALIZE_MAX  127

#define THUMBHELIX_CENTER_RANGE_MAX 600
#define THUMBHELIX_CENTER_RANGE_MIN 400

#endif
