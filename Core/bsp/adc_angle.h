/* adc_angle.h */
#ifndef ADC_ANGLE_H
#define ADC_ANGLE_H

#include "main.h"          // 包含 hadc1 等外设句柄

extern ADC_HandleTypeDef hadc1;

/* 用户可调参数 */
#define ADC_MAX_VAL        4095.0f
#define ANGLE_MAX_DEG      270.0f   // 电位计有效角度范围
#define ADC_SAMPLE_COUNT   8        // 滑动平均滤波点数

/* 对外输出的当前角度值（单位：度），volatile 供 ISR 或多任务使用 */
extern volatile float current_angle_deg;

/* 函数声明 */
void ADC_Angle_Init(void);                      // 可选：初始化滤波缓冲区
uint32_t ReadADC_Filtered(void);                // 读取ADC并返回滤波值
float ADC_to_Angle(uint32_t adc_raw);           // 原始值转角度
float GetCurrentAngle(void);                    // 一键获取当前角度

#endif /* ADC_ANGLE_H */