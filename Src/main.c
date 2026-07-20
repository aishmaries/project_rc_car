/*
 * Created on Jun 11
 * */

#include "servo.h"
#include "led.h"
#include "camera.h"

int main(void) {

	servo_init();

	center_servo();
	delay(500);
	turn_cw();

	while(1){
		if(frame_ready){
			read_enable();
			read_frame();
			frame_ready = 0;
		}
	}


}


