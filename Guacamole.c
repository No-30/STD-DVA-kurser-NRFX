#include "Guacamole.h"
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <nrfx_rtc.h>
#include <nrfx_systick.h>
#include <nrf_gpio.h>
#include <stdio.h>
#include <nrfx_gpiote.h>
#include <nrf_gpiote.h>
#include <math.h>


nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
const nrfx_rtc_t rtc_instance = NRFX_RTC_INSTANCE(0);
int LEDArr[ARR_LENGTH] = { LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN };
int ButtonArr[ARR_LENGTH] = { BUTTON1_PIN, BUTTON2_PIN, BUTTON3_PIN, BUTTON4_PIN };
int LEDState[ARR_LENGTH] = { LED_OFF, LED_OFF, LED_OFF, LED_OFF };
int lives = 5;
int hits = 0;
int score = 0;

char uarteBuffer;
char sprintfBuffer[STRING_LENGTH];
char string[STRING_LENGTH];
int rtcTime;

volatile int lastButtonPressed = -1;
volatile bool buttonEventFlag = false;
volatile bool hitsEvent = false;
volatile bool livesEvent = false;


void uarte_write(char* data)
{
    nrfx_uarte_tx(&instance, data, strlen(data), 0);  // Skicka data via UART
}

void toggleLEDFromButton()
{
    for(int i = 0; i < ARR_LENGTH; i++){
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

void game(int alive) 
{

    float booster;
    int lit, rngNum, rngTime;
    int counter = 0;
    int waittime;

    while (alive)
    {
        booster = boost(hits, &lives);
        lit = timeLit(hits);

        if(counter < 60)
            waittime = cos((float)counter/40.0)*1000;
        else
            waittime = 60;
        rngNum = rand() % 4;
        rngTime = rand() % waittime*1/4 + waittime*3/4;
        
        /*sprintf(sprintfBuffer, "\n\rCounter %d, waittime %d", counter, waittime);
        uarte_write(sprintfBuffer);*/

        nrf_gpio_pin_write(LEDArr[rngNum], LED_ON);
        counter++;

        int buttonId = timedButtonPress(lit);

        // CASE 1: Correct button
        if (buttonId == rngNum)
        {
            hits++;
            hitsEvent = true; 
            score += (int)(10 * booster);

        }
        // CASE 2: Wrong button
        else if (buttonId != -1 && buttonId != rngNum)
        {
            lives--;
            livesEvent = true;
        }
        // CASE 3: No button pressed in time
        else if (buttonId == -1)
        {
            lives--;
            livesEvent = true;
        }

        nrf_gpio_pin_write(LEDArr[rngNum], LED_OFF);

        if (lives <= 0)
            alive = 0;

        delay(rngTime, 'm');
    }
}

void gpio_button_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    for (int i = 0; i < ARR_LENGTH; i++)
    {
        if (pin == ButtonArr[i])
        {
            lastButtonPressed = i;
            buttonEventFlag = true;
            break;
        }
    }
}

void rtc_handler(nrfx_rtc_int_type_t type)
{
    if (type == NRFX_RTC_INT_COMPARE0)
    {
        uarte_write("\n\r[RTC] interrupt fired\n\r");  // debug

        if (hitsEvent || livesEvent)
        {
            status(score, hits, lives);
            hitsEvent = false;
            livesEvent = false;
        }

        uint32_t now = nrfx_rtc_counter_get(&rtc_instance);
        nrfx_rtc_cc_set(&rtc_instance, 0, now + 32, true);
    }
}


int timeLit(int hits)
{
    int lit = 1000 - (hits * 15);

    if(lit <= 100)
    {
        lit = 100;
        return lit;
    }
    return lit;
}

float boost(int hits, int *lives)
{
    float booster = 1 + (hits * 0.25);

    if(booster > 5)
    {
        booster = 5;
        if(lives < 5)
            (*lives)++;
    }
    return booster;  
}

void status(int score, int hits, int lives)
{
    sprintf(sprintfBuffer, "\n\r"COLOR_GREEN"Score: %d"COLOR_RESET, score);
    uarte_write(sprintfBuffer);
    sprintf(sprintfBuffer, "\n\r"COLOR_YELLOW"Hits: %d"COLOR_RESET, hits);
    uarte_write(sprintfBuffer); 
    sprintf(sprintfBuffer, "\n\r"COLOR_RED"Lives: %d\n\r"COLOR_RESET, lives);
    uarte_write(sprintfBuffer);
}

void initSystick()
{
    nrfx_systick_init();  // Initiera systick för fördröjningar
}

void initRTC()
{
    nrfx_rtc_config_t config = NRFX_RTC_DEFAULT_CONFIG;

    nrfx_err_t err = nrfx_rtc_init(&rtc_instance, &config, rtc_handler);

    NVIC_EnableIRQ(RTC0_IRQn);

    uint32_t now = nrfx_rtc_counter_get(&rtc_instance);
    nrfx_rtc_cc_set(&rtc_instance, 0, now + 32, true);

    nrfx_rtc_enable(&rtc_instance);
}



void initUART()
{
    const nrfx_uarte_config_t configu = NRFX_UARTE_DEFAULT_CONFIG(UART_TX, UART_RX);
    nrfx_err_t erru = nrfx_uarte_init(&instance, &configu, NULL);  // Initiera UART med standardkonfiguration
    if (erru != 0){
        //Här kan vi implementera felhantering, men för stunden håller vi tummarna att allt funkar
    }
}

void confButtons()
{
    for(int i = 0; i < ARR_LENGTH; i++)
    {
        nrf_gpio_cfg_input(ButtonArr[i], NRF_GPIO_PIN_PULLUP);  // Konfigurera knappar som ingångar med pullup
    }
}

void confLEDs()
{
    for(int i = 0; i < ARR_LENGTH; i++)
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

int timedButtonPress(int timeout_ms)
{
    int elapsed = 0;

    while (elapsed < timeout_ms)
    {
        for (int i = 0; i < ARR_LENGTH; i++)
        {
            if (!nrf_gpio_pin_read(ButtonArr[i]))
            {
                while (!nrf_gpio_pin_read(ButtonArr[i]))
                    nrfx_systick_delay_ms(20);

                return i;  // button pressed
            }
        }

        nrfx_systick_delay_ms(10);
        elapsed += 10;
    }

    return -1;  // no button pressed
}

void initAll_Mod()
{
    confButtons();
    initSystick();
    initUART();
    initRTC();

    sprintf(string,
        "\n\r"COLOR_PURPLE"==============================================================\n\r"
        " Press the buttons glowing purple to add to your total score\n\r"
        " Avoid pressing non glowing buttons to avoid losing lives\n\r"
        "=============================================================="COLOR_RESET);
    uarte_write(string);

    rtcTime = nrfx_rtc_counter_get(&rtc_instance);
    srand(rtcTime);

    confLEDs();

    // Startup LED animation
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < ARR_LENGTH; j++)
        {
            toggleLED(j);
            delay(50, 'm');
        }
    }

    sprintf(sprintfBuffer, "\n\rGame Started\n\r");
    uarte_write(sprintfBuffer);
}