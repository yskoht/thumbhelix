#ifndef __THUMBHELIX_H__
#define __THUMBHELIX_H__

#include <stdint.h>
#include <stdbool.h>

#include "pointing_device.h"

typedef struct {
    uint16_t x;
    uint16_t y;
    int8_t   z;
} Thumbhelix;

typedef struct {
    int8_t x;
    int8_t y;
    int8_t z;
} Thumbhelix8;

typedef enum {
    TH_NO_PRESSED = 0x00,
    TH_BTN1       = 0x01,
    TH_BTN2       = 0x02,
    TH_BTN3       = 0x04,
    TH_BTN4       = 0x08,
    TH_BTN5       = 0x10,
    TH_ERR        = 0x80,
} TH_BUTTON;


/** Sample pointing function
 */
void thumbhelix(void);

/** Read I2C slave value
 *  Output: th (th.z = 0x80 means I2C read error)
 */
void th_read(Thumbhelix *th);

/** Apply LPF. Coeff is THUMBHELIX_SENSITIVITY
 *  Input : prev (previous th)
 *          th   (current th)
 *  Output: th   (th is updated as LPF output)
 */
void th_apply_lpf(const Thumbhelix *prev, Thumbhelix *th);

/** Calcurate differential
 *  Input : prev (previous th)
 *          th   (current th)
 *  Output: diff
 */
void th_differentiate(const Thumbhelix *prev, const Thumbhelix *th, Thumbhelix *diff);

/** Get thumbstick center position
 *  Input : prev (previous th)
 *          th   (current th)
 *  Output: center
 */
void th_get_center(const Thumbhelix *prev, const Thumbhelix *th, Thumbhelix *center);

/** Normalize to 8 bit signed
 *  Input : center
 *          th  (current th)
 *  Output: th8 (8 bit th. -THUMBHELIX_CURSOR_SPEED <= (th8.x, th8.y) <= THUMBHELIX_CURSOR_SPEED)
 */
void th_normalize(const Thumbhelix *center, const Thumbhelix *th, Thumbhelix8 *th8);

/** Correct th8.x and th8.y for cursor
 *  Input : th8
 *  Output: th8
 */
void th_correct_cursor(Thumbhelix8 *th8);

/** Correct th8.x and th8.y for scroll
 *  Input : th8
 *  Output: th8
 */
void th_correct_scroll(Thumbhelix8 *th8);

/** Set th8.x and th8.y to cursor
 *  Input : th8
 *  Output: th8
 */
void th_set_cursor(const Thumbhelix8 *th8, report_mouse_t *r);

/** Set th8.x and th8.y to scroll
 *  Input : th8
 *  Output: th8
 */
void th_set_scroll(const Thumbhelix8 *th8, report_mouse_t *r);

/** Set th8.z to scroll
 *  Input : th8
 *  Output: th8
 */
void th_set_buttons(const Thumbhelix8 *th8, report_mouse_t *r);

/** Set button
 *  Input : TH_BUTTON
 */
void th_pressed(const TH_BUTTON b);

/** Unset button
 *  Input : TH_BUTTON
 */
void th_released(const TH_BUTTON b);

#endif
