#ifndef STD_HEADER_H
#define STD_HEADER_H
#define LED1 28
#define LED2 29
#define LED3 30
#define LED4 31
#define BUTTON1 23
#define BUTTON2 24
#define BUTTON3 8
#define BUTTON4 9
#define UART_TX 20
#define UART_RX 22
#define LED_ARR_LENGTH 4
#define BUTTON_ARR_LENGTH 4
#define LED_ON 0
#define LED_OFF 1
#define STRING_LENGTH 100

/*extern int LEDArr[LED_ARR_LENGTH] = {LED1, LED2, LED3, LED4};
extern int ButtonArr[BUTTON_ARR_LENGTH] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4};
extern int LEDState[LED_ARR_LENGTH] = {LED_OFF, LED_OFF, LED_OFF, LED_OFF};
extern char uarteBuffer;
extern char sprintfBuffer[STRING_LENGTH];
extern char string[STRING_LENGTH];*/

int read_int();
void uarte_write();
void readString(char charArr[], int* length);
void toggleLED();
void initSystick();
void initRTC();
void initUART();
void confButtons();
void confLEDs();
void delay(int time, char unit);

#endif //↑ ↑ ↓ ↓ ← → ← → B A (Start)