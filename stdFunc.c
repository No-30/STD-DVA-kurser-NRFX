#include "stdHeader.h"
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <nrfx_rtc.h>
#include <nrfx_systick.h>
#include <nrf_gpio.h>
#include <dwt.h>

// Globala variabler för UART, RTC, LED, knappar och tillstånd
nrfx_uarte_t instance0 = NRFX_UARTE_INSTANCE(0);
nrfx_uarte_t instance1 = NRFX_UARTE_INSTANCE(1);
const nrfx_uarte_t uarte_instance = NRFX_UARTE_INSTANCE(1);
const nrfx_rtc_t rtc_instance = NRFX_RTC_INSTANCE(0);
int LEDArr[LED_ARR_LENGTH] = { LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN };
int ButtonArr[BUTTON_ARR_LENGTH] = { BUTTON1_PIN, BUTTON2_PIN, BUTTON3_PIN, BUTTON4_PIN };
int LEDState[LED_ARR_LENGTH] = { LED_OFF, LED_OFF, LED_OFF, LED_OFF };
volatile bool bufferFull = false;

char uarteBuffer;
char sprintfBuffer[STRING_LENGTH];
char string[STRING_LENGTH];
int rtcTime;
volatile char uarteKey;
volatile bool uarteKeyReady = false;
volatile char uarte0Byte;
volatile bool uarte0ByteReady = false;
volatile char uarte1Byte;
volatile bool uarte1ByteReady = false;
static uint8_t rxByte0;
static uint8_t rxByte1;

int read_int()
{
    char charArr[STRING_LENGTH];
    int length = STRING_LENGTH;

    readString(charArr, &length, 0);  // Läs en sträng från UART0

    return atoi(charArr);  // Konvertera strängen till heltal
}

void uarte_write(char* data, int uart) //instance måste vara 1 eller 0 annars Rico-moment
{
    nrfx_uarte_t const * instance = NULL;  // Standardinstans för UART
    switch (uart)
    {
        case 0:
            instance = &instance0;  // Använd UART0
            break;
        case 1:
            instance = &instance1;  // Använd UART1
            break;
        default:
            return;
    }
    nrfx_uarte_tx(instance, (uint8_t const *)data, strlen(data), 0);  // Skicka data via UART
}

void uart_bb_tx(uint32_t pin, uint8_t data, float baudrate)
{

    uint32_t cycles_per_bit = (uint32_t)((SystemCoreClock / baudrate) + 0.5f);
    if (cycles_per_bit == 0)
    {
        return;
    }

    nrf_gpio_cfg_output(pin);
    nrf_gpio_pin_write(pin, 1); // Idle level

    uint32_t target = dwt_get_cycle() + cycles_per_bit;

    // Startbit
    nrf_gpio_pin_write(pin, 0);
    dwt_delay_until_cycle(target);

    // Data (LSB first)
    for (uint32_t i = 0; i < 8; i++)
    {
        if ((data & (uint8_t)(1U << i)) != 0)
        {
            nrf_gpio_pin_write(pin, 1);
        }
        else
        {
            nrf_gpio_pin_write(pin, 0);
        }

        target += cycles_per_bit;
        dwt_delay_until_cycle(target);
    }

    // Stopbit
    nrf_gpio_pin_write(pin, 1);
    target += cycles_per_bit;
    dwt_delay_until_cycle(target);
}

void uarte_pi_read(char* data, int length)
{
    nrfx_uarte_t const * instance = &instance1;  // Använd UART1 för läsning
    nrfx_uarte_rx(instance, (uint8_t *)data, length);  // Läs data från UART

}

void uarte0_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    (void)p_context;

    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
        uarte0Byte = (char)rxByte0;
        uarte0ByteReady = true;
        nrfx_uarte_rx(&instance0, &rxByte0, 1);
    }
}

void uarte1_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    (void)p_context;

    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
        uarte1Byte = (char)rxByte1;
        uarte1ByteReady = true;
        nrfx_uarte_rx(&instance1, &rxByte1, 1);
    }
}

void interrupts(void)
{
    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));

    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(1)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(1)));
}

void readString(char charArr[], int* length, int uart)//instance måste vara 1 eller 0 annars Rico-moment
{
    nrfx_uarte_t const * instance = NULL;  // Standardinstans för UART
    switch (uart)
    {
        case 0:
            instance = &instance0;  // Använd UART0
            break;
        case 1:
            instance = &instance1;  // Använd UART1
            break;
        default:
            *length = 0;
            return;
    }
    for(int i = 0; i < *length; i++)
    {
        uint8_t received = 0;
        nrfx_uarte_rx(instance, &received, sizeof(received));  // Läs ett tecken från UART
        uarteBuffer = (char)received;
        if (uarteBuffer != '\r')
        {
            charArr[i] = uarteBuffer; // Lägg till tecknet i arrayen
            bufferFull = true;
        }
        else
        {
            charArr[i] = '\0';  // Avsluta strängen med null-terminator
            *length = i;  // Uppdatera längden
            break;
        }
    }
}

void toggleLEDFromButton()
{
    for(int i = 0; i < LED_ARR_LENGTH; i++){
        if(!nrf_gpio_pin_read(ButtonArr[i]))  // Om knappen är nedtryckt
        {
            LEDState[i] = (LEDState[i] + 1) % 2;  // Växla LED-tillstånd
            nrf_gpio_pin_write(LEDArr[i], LEDState[i]);  // Uppdatera LED
            while(!nrf_gpio_pin_read(ButtonArr[i]))  // Vänta tills knappen släpps
            {
                nrfx_systick_delay_ms(50);
            }
        }
    }
}

void toggleLED(int led)
{
    LEDState[led] = (LEDState[led] + 1) % 2;  // Växla tillstånd för specifik LED
    nrf_gpio_pin_write(LEDArr[led], LEDState[led]);  // Uppdatera LED
}

void initSystick()
{
    nrfx_systick_init();  // Initiera systick för fördröjningar
}

void initRTC()
{
    nrfx_rtc_config_t config = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_rtc_init(&rtc_instance, &config, NULL);
    nrfx_rtc_enable(&rtc_instance);
}

void initUART()
{
    const nrfx_uarte_config_t configu0 = NRFX_UARTE_DEFAULT_CONFIG(UART0_TX, UART0_RX);
    nrfx_err_t erru0 = nrfx_uarte_init(&instance0, &configu0, uarte0_handler);
    if (erru0 == NRFX_SUCCESS)
    {
        nrfx_uarte_rx(&instance0, &rxByte0, 1);
    }

    const nrfx_uarte_config_t configu1 = NRFX_UARTE_DEFAULT_CONFIG(UART1_TX, UART1_RX);
    nrfx_err_t erru1 = nrfx_uarte_init(&instance1, &configu1, uarte1_handler);
    if (erru1 == NRFX_SUCCESS)
    {
        nrfx_uarte_rx(&instance1, &rxByte1, 1);
    }

    interrupts();
}

void confButtons()
{
    for(int i = 0; i < BUTTON_ARR_LENGTH; i++)
    {
        nrf_gpio_cfg_input(ButtonArr[i], NRF_GPIO_PIN_PULLUP);  // Konfigurera knappar som ingångar med pullup
    }
}

void confLEDs()
{
    for(int i = 0; i < LED_ARR_LENGTH; i++)
    {
        nrf_gpio_cfg_output(LEDArr[i]);  // Konfigurera LED som utgångar
        nrf_gpio_pin_write(LEDArr[i], LEDState[i]);  // Sätt initialt tillstånd
    }
}

void delay(int time, char unit)//case för ms s
{
    switch (unit)
    {
    case 's': //second
        time *= 1000;  // Konvertera sekunder till millisekunder
        break;
    
    case 't': //tenth second
        time *= 100;  // Konvertera tiondels sekunder till millisekunder
        break;
    
    
    case 'm': //millisecond
        time *= 1;  // Millisekunder som är
        break;
    
    default:
        break;
    }
    nrfx_systick_delay_ms(time);  // Fördröj med angiven tid
}

void waitForButton(int button)
{
    int whileloop = 1;
    while (whileloop)
    {
        if (!nrf_gpio_pin_read(ButtonArr[button]))  // Om specifik knapp är nedtryckt
        {
            while (!nrf_gpio_pin_read(ButtonArr[button]))  // Vänta tills knappen släpps
            {
                delay(50, 'm');
                whileloop = 0;  // Avsluta yttre loop
            }
        }
        delay(50, 'm');  // Kort fördröjning för att undvika busy-wait
    }
}

int buttonPressed()
{
    int whileloop = 1;
    while (whileloop)
    {
        for (int i = 0; i < BUTTON_ARR_LENGTH; i++)
        {
            if (!nrf_gpio_pin_read(ButtonArr[i]))  // Om någon knapp är nedtryckt
            {
                while (!nrf_gpio_pin_read(ButtonArr[i]))  // Vänta tills knappen släpps
                {
                    nrfx_systick_delay_ms(50);
                    whileloop = 0;
                    return i;  // Returnera knappens index
                }
                nrfx_systick_delay_ms(50);
            }
        }
    }
    return -1;
}


void initAll()
{
    confButtons();
    initSystick();
    initUART();
    initRTC();
    dwt_init();  // Initiera DWT för cykelräkning
    nrf_gpio_cfg_output(BBUART_TX);
    nrf_gpio_pin_write(BBUART_TX, 1);  // Sätt BBUART TX hög

    sprintf(sprintfBuffer, "\n\rPress Button 1 to start RTC\n\r");
    uarte_write(sprintfBuffer, 0);  // Uppmana användaren att trycka på knapp 1
    waitForButton(BUTTON1);  // Vänta på knapp 1

    rtcTime = nrfx_rtc_counter_get(&rtc_instance);
    srand(rtcTime);

    confLEDs();
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < LED_ARR_LENGTH; j++)
        {
            toggleLED(j);  // Växla varje LED
            delay(50, 'm');  // Kort fördröjning
        }
    }

    sprintf(sprintfBuffer, "\n\rAll initiated\r\n");
    uarte_write(sprintfBuffer, 0);
    while (nrfx_uarte_tx_in_progress(&instance0)) {}
}