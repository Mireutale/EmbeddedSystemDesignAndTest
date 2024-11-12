/*
 * week11_advanced.c
 *
 *  Created on: 2023. 9. 4.
 *      Author: CYH
 *
 * Example of PWM motor operating in reverse according to button input
 */


#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "lcd.h"
#include "touch.h"
#include "math.h"


void Init(void);
void RccInit(void);
void GpioInit(void);
void TIM_Configure(void);
void NvicInit(void);
void ledToggle(int num);

const int color[12] = { WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY };

// timer counter
int t1;
int t2;
// led on/off
char ledOn = 0;
// motor set
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
// motor angle
int motorAngle = 0;
int motorDir = 0;
uint16_t x, y;

int main() {
  Init();

  LCD_Clear(WHITE);

  // TODO: Display team name and button on LCD
  LCD_ShowString(20, 20, "TUE_Team03", BLUE, WHITE);
  LCD_ShowString(90, 200, "BUT", BLACK, WHITE);
  LCD_DrawRectangle(60, 180, 150, 220);

// TODO: Motor setup
  while (1) {
  //LED OFF 상태
    if (ledOn == 0) {
      motorDir = 1;
    }
    //LED ON 상태
    else {
      motorDir = 0;
    }

  // Get touch position
    Touch_GetXY(&x, &y, 1);
    Convert_Pos(x, y, &x, &y);

  // Detect button touch
    if (60 <= x && x <= 180 && 150 <= y && y <= 220) {
      if (ledOn == 0) {
          LCD_ShowString(70, 80, " ", BLACK, WHITE);
          ledOn = 1;
          LCD_ShowString(70, 80, "ON", RED, WHITE);
        } else {
          LCD_ShowString(70, 80, " ", BLACK, WHITE);
          ledOn = 0;
          LCD_ShowString(70, 80, "OFF", RED, WHITE);
      }
    }
  }
}


void Init(void) {
  SystemInit();
  RccInit();
  GpioInit();
  TIM_Configure();
  NvicInit();

  LCD_Init();
  Touch_Configuration();
  Touch_Adjust();
}


void RccInit(void) {
  RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}

// TODO
void GpioInit(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  uint16_t prescale = 0;
  // LED setup

  // PWM motor setup

  // Timer setup

  // LED setup on GPIOD Pin 2 and Pin 3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // PWM motor setup on GPIOB Pin 0
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// TODO
void TIM_Configure(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  // Configure TIM2 for periodic interrupts
  TIM_TimeBaseStructure.TIM_Period = 10000 - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);

  // Configure TIM3 for PWM output on motor control
  TIM_TimeBaseStructure.TIM_Period = 20000 - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = motorAngle;
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

  TIM_Cmd(TIM3, ENABLE);
}

void NvicInit(void) {
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//ledtoggle(n) -> n번 LED 점등
void ledToggle(int num) {
  uint16_t pin;

  switch (num) {
    case 1:
      pin = GPIO_Pin_2;
      break;
    case 2:
      pin = GPIO_Pin_3;
      break;
    default:
      return;
  }

  if (GPIO_ReadOutputDataBit(GPIOD, pin) == Bit_RESET) {
    GPIO_SetBits(GPIOD, pin);
  }
  else {
    GPIO_ResetBits(GPIOD, pin);
  }
}

//motorDir == 0(LED On) -> 정방향 회전
//motorDir == 1(LED Off) -> 역방향 회전
void moveMotor() {
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = motorAngle + 700;
  if (motorDir == 0) {
    motorAngle = motorAngle + 100;
  if (motorAngle == 1500) 
    motorAngle = 0;
  }
  else {
    motorAngle = motorAngle - 100;
  if (motorAngle == 0) 
    motorAngle = 1500;
  }

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
}

/*
* ISR
*/

void TIM2_IRQHandler(void) {
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {

  t1++;
  t2++;
  moveMotor();

  if (ledOn == 1) {
    // Toggle LED 1
    ledToggle(1);
  if (t1 % 5 == 0) {
    // Toggle LED 2
    ledToggle(2);
    }
  }

  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}
