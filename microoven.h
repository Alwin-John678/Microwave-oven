/* 
 * File:   microoven.h
 * Author: ASUS
 *
 * Created on 11 February, 2025, 9:25 PM
 */

#ifndef MICROOVEN_H
#define	MICROOVEN_H

void power_on_screen(void);
void clear_screen(void);
void display_menu_screen(void);
void set_time(unsigned char key,unsigned char reset_flag);
void set_temp(unsigned char key,unsigned char reset_flag);
void heat_food(void);

void time_display(void);

#endif	/* MICROOVEN_H */

