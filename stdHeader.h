#ifndef STD_HEADER_H
#define STD_HEADER_H

#include <stdio.h>

#define LED1_PIN 28
#define LED2_PIN 29
#define LED3_PIN 30
#define LED4_PIN 31
#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3
#define BUTTON1_PIN 23
#define BUTTON2_PIN 24
#define BUTTON3_PIN 8
#define BUTTON4_PIN 9
#define BUTTON1 0
#define BUTTON2 1
#define BUTTON3 2
#define BUTTON4 3
#define UART0_TX 20
#define UART0_RX 22
#define UART1_TX 36
#define UART1_RX 37
#define LED_ARR_LENGTH 4
#define BUTTON_ARR_LENGTH 4
#define LED_ON 0
#define LED_OFF 1
#define STRING_LENGTH 100

extern int LEDArr[LED_ARR_LENGTH];
extern int ButtonArr[BUTTON_ARR_LENGTH];
extern int LEDState[LED_ARR_LENGTH];
extern char uarteBuffer;
extern char sprintfBuffer[STRING_LENGTH];
extern char string[STRING_LENGTH];
extern int rtcTime;

int read_int();
void uarte_write(char* data, int uart);
void rtc_handler(nrfx_rtc_int_type_t type);
void readString(char charArr[], int* length, int uart);
void toggleLEDFromButton();
void toggleLED(int led);
void initSystick();
void initRTC();
void initUART();
void confButtons();
void confLEDs();
void delay(int time, char unit);
void waitForButton(int button);
int buttonPressed();
void initAll();

#endif //↑ ↑ ↓ ↓ ← → ← → B A (Start)