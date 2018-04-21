#ifndef __THUMBHELIX_H__
#define __THUMBHELIX_H__

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

/** Sample pointing function
 *  Input : scroll (If it's true, cursor dosen't move)
 */
void thumbhelix(const bool scroll);

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

/** Correct
 *  Input : th8
 *  Output: th8
 */
void th_correct(Thumbhelix8 *th8);

#endif
