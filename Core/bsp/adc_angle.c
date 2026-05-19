/* adc_angle.c */
#include "adc_angle.h"

/* 静态数组和索引用于滑动平均滤波 */
static uint16_t adc_buffer[ADC_SAMPLE_COUNT] = {0};
static uint8_t  adc_index = 0;

/* 全局变量定义 */
volatile float current_angle_deg = 0.0f;

void ADC_Angle_Init(void)
{
    for (int i = 0; i < ADC_SAMPLE_COUNT; i++)
        adc_buffer[i] = 0;
    adc_index = 0;
    current_angle_deg = 0.0f;
}

/**
  * @brief  读取 ADC 并返回经过滑动平均滤波的原始值
  * @retval ADC 原始值（0~4095）的平均值
  */
uint32_t ReadADC_Filtered(void)
{
    uint32_t sum = 0;

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        adc_buffer[adc_index] = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    adc_index = (adc_index + 1) % ADC_SAMPLE_COUNT;

    for (int i = 0; i < ADC_SAMPLE_COUNT; i++)
    {
        sum += adc_buffer[i];
    }
    return sum / ADC_SAMPLE_COUNT;
}

/**
  * @brief  将 ADC 原始值映射为角度（度）
  * @param  adc_raw 滤波后的 ADC 值
  * @retval 角度值（度）
  */
float ADC_to_Angle(uint32_t adc_raw)
{
    return (adc_raw / ADC_MAX_VAL) * ANGLE_MAX_DEG;
}

float GetCurrentAngle(void)
{
    uint32_t raw = ReadADC_Filtered();
    current_angle_deg = ADC_to_Angle(raw);
    return current_angle_deg;
}