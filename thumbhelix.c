
#include "thumbhelix.h"
#include "LUFA/Drivers/Peripheral/TWI.h"

#include "timer.h"

#define u16(x) ((uint16_t)(x))
#define s16(x) ((int16_t)(x))
#define f(x)   ((float)(x))

static uint8_t buttons = 0;

void thumbhelix(void)
{
    static Thumbhelix prev   = {0, 0, 1};
    static Thumbhelix center = {0, 0, 1};
    Thumbhelix  th;
    Thumbhelix8 th8;

    th_read(&th);
    th_apply_lpf(&prev, &th);
    th_get_center(&prev, &th, &center);
    th_normalize(&center, &th, &th8);
    prev = th;

    report_mouse_t r = pointing_device_get_report();
    if(th8.z != TH_NO_PRESSED)  {
        th_correct_scroll(&th8);
        th_set_scroll(&th8, &r);
    }
    else {
        th_correct_cursor(&th8);
        th_set_cursor(&th8, &r);
    }
    th_set_buttons(&th8, &r);

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
        th->z = (byte5 == 0) ? THUMBHELIX_KEY : TH_NO_PRESSED;
    }
    else {
        // I2C read error
        th->x = 0;
        th->y = 0;
        th->z = TH_ERR;
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

__attribute__ ((weak))
void th_normalize(const Thumbhelix *center, const Thumbhelix *th, Thumbhelix8 *th8)
{
    normalize(THUMBHELIX_NORMALIZE_MAX, &(center->x), &(th->x), &(th8->x));
    normalize(THUMBHELIX_NORMALIZE_MAX, &(center->y), &(th->y), &(th8->y));
    th8->z = th->z;
}

__attribute__ ((weak))
void th_correct_cursor(Thumbhelix8 *th8)
{
    th8->x *= f(THUMBHELIX_CURSOR_SPEED) / 100.0;
    th8->y *= f(THUMBHELIX_CURSOR_SPEED) / 100.0;
}

__attribute__ ((weak))
void th_correct_scroll(Thumbhelix8 *th8)
{
    if(th8->x != 0) {
        th8->x = (th8->x > 0) ?  THUMBHELIX_H_SCROLL_SPEED : -THUMBHELIX_H_SCROLL_SPEED;
    }
    if(th8->y != 0) {
        th8->y = (th8->y > 0) ? -THUMBHELIX_V_SCROLL_SPEED :  THUMBHELIX_V_SCROLL_SPEED;
    }
}

void th_set_cursor(const Thumbhelix8 *th8, report_mouse_t *r)
{
    r->x = th8->x;
    r->y = th8->y;
    r->v = 0;
    r->h = 0;
}

void th_set_scroll(const Thumbhelix8 *th8, report_mouse_t *r)
{
    static uint16_t last_timer = 0;

    if(timer_elapsed(last_timer) > THUMBHELIX_SCROLL_INTERVAL) {
        r->v = th8->y;
        r->h = th8->x;
        last_timer = timer_read();
    }
    else {
        r->v = 0;
        r->h = 0;
    }
    r->x = 0;
    r->y = 0;
}

__attribute__ ((weak))
void th_set_buttons(const Thumbhelix8 *th8, report_mouse_t *r)
{
    // Is Thumbhelix key pressed?
    if(th8->z & THUMBHELIX_KEY) {
        th_pressed(THUMBHELIX_KEY);
    }
    else {
        th_released(THUMBHELIX_KEY);
    }

    // If TH_BTN2 is pressed, hold it.
    if(r->buttons & TH_BTN2) {
        th_pressed(TH_BTN2);
    }
    // But, if a key except for TH_BTN2 is pressed, release TH_BTN2.
    if(buttons & ~TH_BTN2) {
        th_released(TH_BTN2);
    }

    r->buttons = buttons;
}

void th_pressed(const TH_BUTTON b)
{
    buttons |= b;
}

void th_released(const TH_BUTTON b)
{
    buttons &= ~b;
}

#undef u16
#undef s16
#undef f
