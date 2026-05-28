/*#ifndef BUTTON_ADC_H
#define BUTTON_ADC_H

#include <stdint.h>
#include <stdbool.h>

// ===============================================================
// CONFIGURATION SECTION
// Change this number to switch between 4 and 9 buttons
// ===============================================================
#define BUTTON_COUNT 9   // <-- set to 4 or 9 depending on your hardware

// ADC input pin (AIN0 = P0.04 on nRF5340 DK)
#define BUTTON_ADC_INPUT NRF_SAADC_INPUT_AIN0

// Digital interrupt pin (any GPIO connected to button ladder)
#define BUTTON_IRQ_PIN  11   // <-- change to your chosen GPIO

// ===============================================================
// PUBLIC API
// ===============================================================

// Initialize SAADC + GPIO interrupt
void button_adc_init(void);

// Returns last pressed button (0..BUTTON_COUNT-1)
// Blocks until a button is detected
int button_adc_get(void);

// Non-blocking version (returns -1 if no button)
int button_adc_peek(void);

#endif*/
