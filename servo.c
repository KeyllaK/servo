#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22
#define PWM_FREQUENCY 50
#define CLOCK_DIVIDER 64.0f
#define PWM_WRAP (125000000 / (CLOCK_DIVIDER * PWM_FREQUENCY))

void set_servo_position(uint slice_num, uint channel, uint32_t pulse_width_us) {
    pwm_set_chan_level(slice_num, channel, (pulse_width_us * PWM_WRAP) / 20000);
}

int main() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    uint channel = pwm_gpio_to_channel(SERVO_PIN);

    pwm_set_clkdiv(slice_num, CLOCK_DIVIDER);
    pwm_set_wrap(slice_num, PWM_WRAP);
    pwm_set_enabled(slice_num, true);

    // Posições do servo
    set_servo_position(slice_num, channel, 2400); // Braço do servo em 180 graus
    sleep_ms(5000);
    set_servo_position(slice_num, channel, 1470); // Braço do servo em 90 graus
    sleep_ms(5000);
    set_servo_position(slice_num, channel, 500);  // Braço do servo em 0 graus
    sleep_ms(5000);

    // Movimentação periódica
    while (1) {
        // Movimento de 0° a 180°
        for (uint32_t pos = 500; pos <= 2400; pos += 5) {
            set_servo_position(slice_num, channel, pos);
            sleep_ms(10);
        }
        // Movimento 180° a 0°
        for (uint32_t pos = 2400; pos >= 500; pos -= 5) {
            set_servo_position(slice_num, channel, pos);
            sleep_ms(10);
        }
    }
}
