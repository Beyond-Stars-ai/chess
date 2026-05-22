#ifndef __KEY_H
#define __KEY_H

#include "main.h"

/* ---------- 按键数量 ---------- */
#define KEY_NUM  5

/* ---------- 动作回调类型 ---------- */
typedef void (*KeyAction_t)(void);

/* ---------- 按键结构体 ---------- */
typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    uint8_t last_state;      // 1:释放  0:按下
    uint8_t action_done;     // 防重复触发标志
    KeyAction_t action;      // 按下时回调的动作
} Key_t;

/* ---------- 外部声明 ---------- */
extern Key_t keys[KEY_NUM];

// void Key_Scan(void);

#endif /* __KEY_H */