#include "key.h"

/* ---------- 应用层动作函数（当前全部翻转LED） ---------- */
// static void Key1_Action(void) {
//     HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
// }

// static void Key2_Action(void) {
//     HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
// }

// static void Key3_Action(void) {
//     HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
// }

// static void Key4_Action(void) {
//     HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
// }

// static void Key5_Action(void) {
//     HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
// }
void Key1_Action(void) {
    KeyEvent_t event = KEY_LEFT;
    osMessageQueuePut(keyEventQueueHandle, &event, 0, 0);
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}
void Key2_Action(void) {
    KeyEvent_t event = KEY_UP;
    osMessageQueuePut(keyEventQueueHandle, &event, 0, 0);
}
void Key3_Action(void) {
    KeyEvent_t event = KEY_DOWN;
    osMessageQueuePut(keyEventQueueHandle, &event, 0, 0);
}
void Key4_Action(void) {
    KeyEvent_t event = KEY_RIGHT;
    osMessageQueuePut(keyEventQueueHandle, &event, 0, 0);
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}
void Key5_Action(void) {
    KeyEvent_t event = KEY_CONFIRM;
    osMessageQueuePut(keyEventQueueHandle, &event, 0, 0);
}

/* ---------- 按键实体数组 ---------- */
Key_t keys[KEY_NUM] = {
    {KEY_1_GPIO_Port, KEY_1_Pin, 1, 0, Key1_Action},
    {KEY_2_GPIO_Port, KEY_2_Pin, 1, 0, Key2_Action},
    {KEY_3_GPIO_Port, KEY_3_Pin, 1, 0, Key3_Action},
    {KEY_4_GPIO_Port, KEY_4_Pin, 1, 0, Key4_Action},
    {KEY_5_GPIO_Port, KEY_5_Pin, 1, 0, Key5_Action},
};

/* ---------- 按键扫描 ---------- */
// void Key_Scan(void)
// {
//     for (int i = 0; i < KEY_NUM; i++)
//     {
//         uint8_t cur = (HAL_GPIO_ReadPin(keys[i].port, keys[i].pin) == GPIO_PIN_RESET) ? 0 : 1;
        
//         if (keys[i].last_state == 1 && cur == 0 && keys[i].action_done == 0)
//         {
//             osDelay(20);
//             if (HAL_GPIO_ReadPin(keys[i].port, keys[i].pin) == GPIO_PIN_RESET)
//             {
//                 if (keys[i].action)
//                 {
//                     keys[i].action();
//                 }
//                 keys[i].action_done = 1;
//             }
//         }
        
//         if (cur == 1)
//         {
//             keys[i].action_done = 0;
//         }
        
//         keys[i].last_state = cur;
//     }
// }