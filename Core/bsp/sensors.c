#include "sensors.h"

void read_sensors(uint8_t *sensors) //*sensors等价于sensors[]
{
    sensors[0] = (HAL_GPIO_ReadPin(sensor_1_GPIO_Port, sensor_1_Pin) == GPIO_PIN_RESET) ? 1 : 0;
    sensors[1] = (HAL_GPIO_ReadPin(sensor_2_GPIO_Port, sensor_2_Pin) == GPIO_PIN_RESET) ? 1 : 0;
    sensors[2] = (HAL_GPIO_ReadPin(sensor_3_GPIO_Port, sensor_3_Pin) == GPIO_PIN_RESET) ? 1 : 0;
    sensors[3] = (HAL_GPIO_ReadPin(sensor_4_GPIO_Port, sensor_4_Pin) == GPIO_PIN_RESET) ? 1 : 0;
    sensors[4] = (HAL_GPIO_ReadPin(sensor_5_GPIO_Port, sensor_5_Pin) == GPIO_PIN_RESET) ? 1 : 0;
    sensors[5] = (HAL_GPIO_ReadPin(sensor_6_GPIO_Port, sensor_6_Pin) == GPIO_PIN_RESET) ? 1 : 0;
    sensors[6] = (HAL_GPIO_ReadPin(sensor_7_GPIO_Port, sensor_7_Pin) == GPIO_PIN_RESET) ? 1 : 0;
    sensors[7] = (HAL_GPIO_ReadPin(sensor_8_GPIO_Port, sensor_8_Pin) == GPIO_PIN_RESET) ? 1 : 0;
}

int detect_move(uint8_t *prev, uint8_t *curr)
{
    for (int i = 0; i < 9; i++) {  // 遍历9个传感器位置
        if (curr[i] != prev[i])    // 比较当前位置的当前状态和上一次状态
            return i;              // 发现变化，立即返回变化位置的索引
    }
    return -1;                     // 所有位置都没有变化，返回-1
}