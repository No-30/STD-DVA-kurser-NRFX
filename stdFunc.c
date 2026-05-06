#include "stdHeader.h"
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <nrfx_rtc.h>
#include <nrfx_systick.h>
#include <nrf_gpio.h>

nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
const nrfx_rtc_t rtc_instance = NRFX_RTC_INSTANCE(0);
int LEDArr[LED_ARR_LENGTH] = { LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN };
int ButtonArr[BUTTON_ARR_LENGTH] = { BUTTON1_PIN, BUTTON2_PIN, BUTTON3_PIN, BUTTON4_PIN };
int LEDState[LED_ARR_LENGTH] = { LED_OFF, LED_OFF, LED_OFF, LED_OFF };

char uarteBuffer;
char sprintfBuffer[STRING_LENGTH];
char string[STRING_LENGTH];
int rtcTime;

int read_int()
{
    char charArr[STRING_LENGTH];
    int length = STRING_LENGTH;

    readString(charArr, &length);

    return atoi(charArr);
}

void uarte_write(char* data)
{
    nrfx_uarte_tx(&instance, data, strlen(data), 0);
}

void readString(char charArr[], int* length)
{
    for(int i = 0; i < *length; i++)
    {
        nrfx_uarte_rx (&instance, &uarteBuffer, sizeof(uarteBuffer));
        if (uarteBuffer != '\r')
        {
            charArr[i] = uarteBuffer;
        }
        else
        {
            charArr[i] = '\0';
            *length = i;
        }
    }
}

void toggleLEDFromButton()
{
    for(int i = 0; i < LED_ARR_LENGTH; i++){
        if(!nrf_gpio_pin_read(ButtonArr[i]))
        {
            LEDState[i] = (LEDState[i] + 1) % 2;
            nrf_gpio_pin_write(LEDArr[i], LEDState[i]);
            while(!nrf_gpio_pin_read(ButtonArr[i]))
            {
                nrfx_systick_delay_ms(50);
            }
        }
    }
}

void toggleLED(int led)
{
    LEDState[led] = (LEDState[led] + 1) % 2;
    nrf_gpio_pin_write(LEDArr[led], LEDState[led]);
}

void initSystick()
{
    nrfx_systick_init();
}

void initRTC()
{
    nrfx_rtc_config_t configc = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_err_t errc = nrfx_rtc_init(&rtc_instance, &configc, NULL);
    nrfx_rtc_enable(&rtc_instance);
    //rtcTime = nrfx_rtc_counter_get(&rtc_instance);
}

void initUART()
{
    const nrfx_uarte_config_t configu = NRFX_UARTE_DEFAULT_CONFIG(UART_TX, UART_RX);
    nrfx_err_t erru = nrfx_uarte_init(&instance, &configu, NULL);
    if (erru != 0){
        //Här kan vi implementera felhantering, men för stunden håller vi tummarna att allt funkar
    }
}

void confButtons()
{
    for(int i = 0; i < BUTTON_ARR_LENGTH; i++)
    {
        nrf_gpio_cfg_input(ButtonArr[i], NRF_GPIO_PIN_PULLUP);
    }
}

void confLEDs()
{
    for(int i = 0; i < LED_ARR_LENGTH; i++)
    {
        nrf_gpio_cfg_output(LEDArr[i]);
        nrf_gpio_pin_write(LEDArr[i], LEDState[i]);
    }
}

void delay(int time, char unit)//case för ms s
{
    switch (unit)
    {
    case 's': //second
        time *= 1000;
        break;
    
    case 't': //tenth second
        time *= 100;
        break;
    
    
    case 'm': //millisecond
        time *= 1;
        break;
    
    default:
        break;
    }
    nrfx_systick_delay_ms(time);
}

void waitForButton(int button)
{
    int whileloop = 1;
    while (whileloop)
    {
        if (!nrf_gpio_pin_read(ButtonArr[button]))
        {
            while (!nrf_gpio_pin_read(ButtonArr[button]))
            {
                delay(50, 'm');
                whileloop = 0;
            }
        }
        delay(50, 'm');
    }
}

int buttonPressed()
{
    int whileloop = 1;
    while (whileloop)
    {
        for (int i = 0; i < BUTTON_ARR_LENGTH; i++)
        {
            if (!nrf_gpio_pin_read(ButtonArr[i]))
            {
                while (!nrf_gpio_pin_read(ButtonArr[i]))
                {
                    nrfx_systick_delay_ms(50);
                    whileloop = 0;
                    return i;
                }
                nrfx_systick_delay_ms(50);
            }
        }
    }
}

void initAll()
{
    confButtons();
    initSystick();
    initUART();
    initRTC();
    sprintf(sprintfBuffer, "\n\rPress Button 1 to start RTC\n\r");
    uarte_write(sprintfBuffer);
    waitForButton(BUTTON1);

    rtcTime = nrfx_rtc_counter_get(&rtc_instance);
    srand(rtcTime);

    confLEDs();
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < LED_ARR_LENGTH; j++)
        {
            toggleLED(j);
            delay(50, 'm');
        }
    }

    sprintf(sprintfBuffer, "\n\rAll initiated\n\r");
    uarte_write(sprintfBuffer);
}