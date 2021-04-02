#include "hal.h"

unsigned int sw_num[] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};
unsigned int leds_num[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
                           GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9,
                           GPIO_PIN_11, GPIO_PIN_12};
unsigned int delay = 500;

bool check(){
    return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_SET &&
           HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8) == GPIO_PIN_RESET &&
           HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10) == GPIO_PIN_RESET &&
           HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12) == GPIO_PIN_RESET;
}

void red() {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
}

void green() {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

void yellow() {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

void stop_animation(){
    red();
    HAL_Delay(200);
    GPIO_PinState state = GPIO_PIN_SET;
    while (state == GPIO_PIN_SET) state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15);
    green();
}

void play_animation() {
    for (int i = 7, j = 6; i >= 0; i--, j--){
        if (j < 0) j = 7;
        HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, leds_num[j], GPIO_PIN_SET);
        HAL_Delay(delay);
        HAL_GPIO_TogglePin(GPIOD, leds_num[i]);
        HAL_GPIO_TogglePin(GPIOD, leds_num[j]);
        if (!check()) return;
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == GPIO_PIN_RESET) stop_animation();
    }
}

void sw_status_output() {
    for (int i = 0; i < 4; i++) {
        GPIO_PinState state = HAL_GPIO_ReadPin(GPIOE, sw_num[i]);
        HAL_GPIO_WritePin(GPIOD, leds_num[i], state);
    }
}

int umain() {
    while (true) {
        if (check()) {
            HAL_GPIO_WritePin(GPIOD, leds_num[0], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, leds_num[1], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, leds_num[2], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, leds_num[3], GPIO_PIN_RESET);
            green();
            play_animation();
        } else {
            yellow();
            sw_status_output();
        }
    }
}