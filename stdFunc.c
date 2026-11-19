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

// Globala variabler för UART, RTC, LED, knappar och tillstånd
nrfx_uarte_t instance0 = NRFX_UARTE_INSTANCE(0);
nrfx_uarte_t instance1 = NRFX_UARTE_INSTANCE(1);
const nrfx_rtc_t rtc_instance1 = NRFX_RTC_INSTANCE(0);
const nrfx_rtc_t rtc_instance0 = NRFX_RTC_INSTANCE(1);
int LEDArr[LED_ARR_LENGTH] = { LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN };
int ButtonArr[BUTTON_ARR_LENGTH] = { BUTTON1_PIN, BUTTON2_PIN, BUTTON3_PIN, BUTTON4_PIN };
int LEDState[LED_ARR_LENGTH] = { LED_OFF, LED_OFF, LED_OFF, LED_OFF };
volatile bool bufferFull = false;

char uarteBuffer;
char sprintfBuffer[STRING_LENGTH];
char string[STRING_LENGTH];
int rtcTime;

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

void rtc_handler(nrfx_rtc_int_type_t type)
{
    if (type == NRFX_RTC_INT_COMPARE0)
    {
        //uarte_write("\n\r[RTC] interrupt fired\n\r");  // Debugutskrift

        if (bufferFull)
        {
            //uarte_write("\n\r[RTC] Buffer full event\n\r");  // Debugutskrift
            readString(string, &rtcTime, 0);  // Läs sträng från UART0
            bufferFull = false;  // Återställ flaggan
        }

        // Ställ in nästa jämförelseintervall (32 ticks framåt)
        uint32_t now = nrfx_rtc_counter_get(&rtc_instance0);
        nrfx_rtc_cc_set(&rtc_instance0, 0, now + 32, true);
    }
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
            if(uarteBuffer != NULL)
            {
                bufferFull = true;  // Markera att bufferten är full
            }  
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
    nrfx_rtc_config_t configc0 = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_rtc_config_t configc1 = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_err_t errc0 = nrfx_rtc_init(&rtc_instance0, &configc0, rtc_handler);  // Initiera RTC med standardkonfiguration
    nrfx_err_t errc1 = nrfx_rtc_init(&rtc_instance1, &configc1, rtc_handler);  // Initiera RTC med standardkonfiguration
    nrfx_rtc_enable(&rtc_instance0);  // Aktivera RTC
    nrfx_rtc_enable(&rtc_instance1);  // Aktivera RTC
}

void initUART()
{
    const nrfx_uarte_config_t configu0 = NRFX_UARTE_DEFAULT_CONFIG(UART0_TX, UART0_RX);
    nrfx_err_t erru0 = nrfx_uarte_init(&instance0, &configu0, NULL);  // Initiera UART med standardkonfiguration
    if (erru0 != 0){
        //Här kan vi implementera felhantering, men för stunden håller vi tummarna att allt funkar
    }
    const nrfx_uarte_config_t configu1 = NRFX_UARTE_DEFAULT_CONFIG(UART1_TX, UART1_RX);
    nrfx_err_t erru1 = nrfx_uarte_init(&instance1, &configu1, NULL);  // Initiera UART med standardkonfiguration
    if (erru1 != 0){
        //Här kan vi implementera felhantering, men för stunden håller vi tummarna att allt funkar
    }
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
    confButtons();  // Konfigurera knappar
    initSystick();  // Initiera systick
    initUART();  // Initiera UART
    initRTC();  // Initiera RTC
    sprintf(sprintfBuffer, "\n\rPress Button 1 to start RTC\n\r");
    uarte_write(sprintfBuffer, 0);  // Uppmana användaren att trycka på knapp 1
    waitForButton(BUTTON1);  // Vänta på knapp 1

    rtcTime = nrfx_rtc_counter_get(&rtc_instance0);  // Hämta RTC-tid
    srand(rtcTime);  // Initiera slumpgenerator med RTC-tid

    confLEDs();  // Konfigurera LED
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < LED_ARR_LENGTH; j++)
        {
            toggleLED(j);  // Växla varje LED
            delay(50, 'm');  // Kort fördröjning
        }
    }

    sprintf(sprintfBuffer, "\n\rAll initiated\n\r");
    uarte_write(sprintfBuffer, 0);  // Bekräfta att allt är initierat
}