#include "button_adc.h"
#include <nrfx_saadc.h>
#include <nrfx_gpiote.h>
#include <nrf_gpio.h>

static volatile int last_button = -1;
static int16_t adc_buffer;

// ===============================================================
// BUTTON VOLTAGE THRESHOLDS
// You MUST measure your resistor ladder values and update these.
// For 4 buttons → use 4 thresholds
// For 9 buttons → use 9 thresholds
// ===============================================================

#if BUTTON_COUNT == 4
static const int button_thresholds[4] = {
    300,   // Button 0
    900,   // Button 1
    1600,  // Button 2
    3000   // Button 3
};
#elif BUTTON_COUNT == 9
static const int button_thresholds[9] = {
    200,   // Button 0
    500,   // Button 1
    900,   // Button 2
    1300,  // Button 3
    1700,  // Button 4
    2100,  // Button 5
    2500,  // Button 6
    2900,  // Button 7
    3300   // Button 8
};
#endif

// ===============================================================
// Decode ADC value into button index
// ===============================================================
static int decode_button(int16_t adc)
{
    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        if (adc < button_thresholds[i])
            return i;
    }
    return -1;
}

// ===============================================================
// SAADC callback
// ===============================================================
static void saadc_handler(nrfx_saadc_evt_t const * evt)
{
    if (evt->type == NRFX_SAADC_EVT_DONE)
    {
        int button = decode_button(adc_buffer);
        last_button = button;

        nrfx_saadc_buffer_convert(&adc_buffer, 1);
    }
}

// ===============================================================
// Start ADC sampling
// ===============================================================
static void start_adc(void)
{
    nrfx_saadc_sample();
}

// ===============================================================
// GPIO interrupt handler
// ===============================================================
static void button_irq_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    start_adc();
}

// ===============================================================
// PUBLIC API IMPLEMENTATION
// ===============================================================
void button_adc_init(void)
{
    // --- SAADC ---
    nrfx_saadc_init(NRFX_SAADC_DEFAULT_CONFIG_IRQ_PRIORITY);

    nrf_saadc_channel_config_t config =
        NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(BUTTON_ADC_INPUT);

    nrfx_saadc_channel_init(0, &config);
    nrfx_saadc_buffer_convert(&adc_buffer, 1);

    // --- GPIO Interrupt ---
    nrfx_gpiote_init();

    nrfx_gpiote_in_config_t irq_cfg =
        NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    irq_cfg.pull = NRF_GPIO_PIN_PULLUP;

    nrfx_gpiote_in_init(BUTTON_IRQ_PIN, &irq_cfg, button_irq_handler);
    nrfx_gpiote_in_event_enable(BUTTON_IRQ_PIN, true);
}

int button_adc_get(void)
{
    while (last_button == -1)
        __WFE();   // Sleep until interrupt

    int b = last_button;
    last_button = -1;
    return b;
}

int button_adc_peek(void)
{
    int b = last_button;
    last_button = -1;
    return b;
}
