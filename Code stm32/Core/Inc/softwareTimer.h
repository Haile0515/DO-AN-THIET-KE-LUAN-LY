/*
 * softwareTimer.h
 *
 *  Created on: Dec 19, 2024
 *      Author: ADMIN
 */

#ifndef INC_SOFTWARETIMER_H_
#define INC_SOFTWARETIMER_H_

extern int timer_flag0;
extern int timer_counter0;

extern int timer_flag1;
extern int timer_counter1;

void setTimer0(int duration);
void setTimer1(int duration);
void timer_run();

#endif /* INC_SOFTWARETIMER_H_ */
