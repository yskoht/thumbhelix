
#include "thumbhelix.h"
#include "LUFA/Drivers/Peripheral/TWI.h"

#define u16(x) ((uint16_t)(x))
#define s16(x) ((int16_t)(x))
#define f(x)   ((float)(x))

void thumbhelix(const bool scroll)
{
    static Thumbhelix prev   = {0, 0, 1};
    static Thumbhelix center = {0, 0, 1};
    Thumbhelix  th;
    Thumbhelix8 th8;

    th_read(&th);
    th_apply_lpf(&prev, &th);
    th_get_center(&prev, &th, &center);
    th_normalize(&center, &th, &th8);
    th_correct(&th8);

    prev = th;

    report_mouse_t r = pointing_device_get_report();
    if(scroll) {
        if(th8.y != 0) r.v = (th8.y > 0) ? -1 : 1;
        if(th8.x != 0) r.h = (th8.x > 0) ? 1 : -1;
    }
    else {
        r.x = th8.x;
        r.y = th8.y;
    }
    if(th8.z == 0) {
        r.buttons |= 0x01;
    }
    pointing_device_set_report(r);
    pointing_device_send();
}

void th_read(Thumbhelix *th)
{
    if(TWI_StartTransmission((THUMBHELIX_ADDR << 1) | TWI_ADDRESS_READ, 20) == TWI_ERROR_NoError)
    {
        uint8_t byte1, byte2, byte3, byte4, byte5;
        TWI_ReceiveByte(&byte1, false);
        TWI_ReceiveByte(&byte2, false);
        TWI_ReceiveByte(&byte3, false);
        TWI_ReceiveByte(&byte4, false);
        TWI_ReceiveByte(&byte5, true);
        TWI_StopTransmission();

        th->x = (u16(byte2) << 8) + byte1;
        th->y = (u16(byte4) << 8) + byte3;
        th->z = byte5;
    }
    else {
        th->x = 0;
        th->y = 0;
        th->z = 0x80;
    }
}

void th_apply_lpf(const Thumbhelix *prev, Thumbhelix *th)
{
    th->x = u16(THUMBHELIX_SENSITIVITY * f(prev->x)
                + (1.0 - THUMBHELIX_SENSITIVITY) * f(th->x));
    th->y = u16(THUMBHELIX_SENSITIVITY * f(prev->y)
                + (1.0 - THUMBHELIX_SENSITIVITY) * f(th->y));
}

void th_differentiate(const Thumbhelix *prev, const Thumbhelix *th, Thumbhelix *diff)
{
    diff->x = th->x - prev->x;
    diff->y = th->y - prev->y;
    diff->z = th->z - prev->z;
}

void th_get_center(const Thumbhelix *prev, const Thumbhelix *th, Thumbhelix *center)
{
    Thumbhelix diff;
    th_differentiate(prev, th, &diff);

    if(diff.x == 0) {
        if(THUMBHELIX_CENTER_RANGE_MIN < th->x && th->x < THUMBHELIX_CENTER_RANGE_MAX) {
            center->x = th->x;
        }
    }
    if(diff.y == 0) {
        if(THUMBHELIX_CENTER_RANGE_MIN < th->y && th->y < THUMBHELIX_CENTER_RANGE_MAX) {
            center->y = th->y;
        }
    }
}

/**
 *  Input : range (output is normalized to -range ~ range)
 *        : ref   (reference)
 *        : x     (target)
 *  Output: nx    (normalized x)
 */
static void normalize(const int8_t range, const uint16_t *ref, const uint16_t *x, int8_t *nx)
{
    const float d = f(s16(*x) - s16(*ref));
    if(d >= 0) {
        *nx = range * (d / f(THUMBHELIX_ANALOG_MAX - *ref));
    }
    else {
        *nx = range * (d / f(*ref));
    }
}

void th_normalize(const Thumbhelix *center, const Thumbhelix *th, Thumbhelix8 *th8)
{
    normalize(127, &(center->x), &(th->x), &(th8->x));
    normalize(127, &(center->y), &(th->y), &(th8->y));
    th8->z = th->z;
}

/**
 *  Input : x (target)
 *  Output: x (corrected x)
 */
static void correct(int8_t *x)
{
    *x *= f(THUMBHELIX_CURSOR_SPEED) / 100.0;
}

void th_correct(Thumbhelix8 *th8)
{
    correct(&(th8->x));
    correct(&(th8->y));
}

#undef u16
#undef s16
#undef f
