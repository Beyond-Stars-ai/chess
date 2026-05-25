#ifndef _SENSORS_H
#define _SENSORS_H

#include <main.h>

// 读取9个传感器状态（低电平=有棋子，高电平=无棋子）
void read_sensors(uint8_t *sensors);

// 检测传感器变化（返回变化的索引，-1表示无变化）
int detect_move(uint8_t *prev, uint8_t *curr);

#endif