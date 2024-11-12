#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "misc.h"
#include "lcd.h"
#include "touch.h"
#include "core_cm3.h"


void Init(void);
void RccInit(void);
void GpioInit(void);
void AdcInit(void);
void NvicInit(void);

uint16_t value;

const int color[12] = { WHITE, CYAN, BLUE, RED, MAGENTA, LGRAY, GREEN, YELLOW, BROWN, BRRED, GRAY };


// TODO
int main() {
    Init();
    LCD_Clear(WHITE);

    uint16_t x = 30, y = 0;
    LCD_ShowString(100, 20, "TEAM_03", BLACK, WHITE);
    x = 90;
    y += 25;

    uint16_t touch_x, touch_y;
    while (1) {
        Touch_GetXY(&touch_x, &touch_y, 1);
        
        Convert_Pos(touch_x, touch_y, &touch_x, &touch_y);

        // Draw a circle at the touched location
        LCD_DrawCircle(touch_x, touch_y, 5);
        LCD_ShowNum(100, 120, touch_x, 3, BLACK, WHITE);
        LCD_ShowNum(100, 150, touch_y, 3, BLACK, WHITE);
        LCD_ShowNum(100, 170, value, 4, BLACK, WHITE);
    }
}


void Init(void) {
    SystemInit();
    RccInit();
    GpioInit();
    AdcInit();
    NvicInit();

    LCD_Init();
    Touch_Configuration();
    Touch_Adjust();
}


void RccInit(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
}


void GpioInit(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// TODO
void AdcInit(void) {
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_28Cycles5);
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1)) {}

    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1)) {}

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


void NvicInit(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// TODO
void ADC1_2_IRQHandler() {
    if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET) {
        value = ADC_GetConversionValue(ADC1);
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    }
}
