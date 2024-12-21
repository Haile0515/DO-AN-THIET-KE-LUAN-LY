/*
 * softwareTimer.c
 *
 *  Created on: Dec 19, 2024
 *      Author: ADMIN
 */
#include "softwareTimer.h"

#define TICK 10

int timer_counter0 = 0;
int timer_flag0 = 0;

void setTimer0(int duration){
	timer_counter0 = duration/TICK;
	timer_flag0 = 0;
}

int timer_counter1 = 0;
int timer_flag1 = 0;

void setTimer1(int duration){
	timer_counter1 = duration/TICK;
	timer_flag1 = 0;
}

void timer_run(){
	if (timer_counter0 > 0) {
		timer_counter0--;
		if (timer_counter0 <= 0) {
			timer_flag0 = 1;
		}
	}
	if (timer_counter1 > 0) {
		timer_counter1--;
		if (timer_counter1 <= 0) {
			timer_flag1 = 1;
		}
	}
}

