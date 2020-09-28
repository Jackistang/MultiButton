/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 */

#ifndef _MULTI_BUTTON_H_
#define _MULTI_BUTTON_H_

#include "stdint.h"
#include "string.h"

//According to your need to modify the constants.
#define TICKS_INTERVAL    5	//ms
#define DEBOUNCE_TICKS    3	//MAX 8
#define SHORT_TICKS       (300 /TICKS_INTERVAL)
#define LONG_TICKS        (1000 /TICKS_INTERVAL)


typedef void (*BtnCallback)(void*);

typedef enum {
	PRESS_DOWN = 0,
	PRESS_UP,
	PRESS_REPEAT,
	SINGLE_CLICK,
	DOUBLE_CLICK,
	LONG_PRESS_START,
	LONG_PRESS_HOLD,
	number_of_event,
	NONE_PRESS
}PressEvent;
// TODO 各字段的详细意思说明
typedef struct Button {
	uint16_t ticks;			//* 本次PRESS_DOWN之后的ticks计数值。调用周期由用户调用button_ticks()的周期决定。 
	uint8_t  repeat : 4;	//* 按键连续按下的次数，连续按下一次即为单击，连续按下两次即为双击，还可以用repeat来判断连续按键。最大值为15
	uint8_t  event : 4;		//* 当前的事件。用4个bit来存储事件数，一共可以存储16个事件，目前只使用了7个。
	uint8_t  state : 3;		//* 状态机里的状态值。
	uint8_t  debounce_cnt : 3;		//* 去抖动，详见multi_button.c: 67。由于只有3bit，因此DEBOUNCE_TICKS最大值为8。
	uint8_t  active_level : 1;		//* 按键按下（激活）的有效电平
	uint8_t  button_level : 1;		//* 按键当前电平
	uint8_t  (*hal_button_Level)(void);		//* 读取按键引脚电平的函数指针，由用户调用 button_init() 时提供
	BtnCallback  cb[number_of_event];		//* 该按键的回调函数数组，每一个事件对应一个回调函数。
	struct Button* next;	//* 链表指针，用于组成按键结构链表。
}Button;

#ifdef __cplusplus
extern "C" {
#endif

void button_init(struct Button* handle, uint8_t(*pin_level)(), uint8_t active_level);
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb);
PressEvent get_button_event(struct Button* handle);
int  button_start(struct Button* handle);
void button_stop(struct Button* handle);
void button_ticks(void);

#ifdef __cplusplus
}
#endif

#endif
