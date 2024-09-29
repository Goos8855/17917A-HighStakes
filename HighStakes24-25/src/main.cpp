#include "main.h"

void lcdClear() {
	pros::lcd::clear_line(1);
	pros::lcd::clear_line(2);
	pros::lcd::clear_line(3);
	pros::lcd::clear_line(4);
	pros::lcd::clear_line(5);
}

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Starting...");
	pros::Controller master(pros::E_CONTROLLER_MASTER); //controller
	pros::MotorGroup left_mg({1,3,5}); //left wheels
	pros::MotorGroup right_mg({2,4,6}); //right wheels
	pros::ADIDigitalOut mogoMech1('A'); //mobile goal clamp
	pros::ADIDigitalOut mogoMech2('B'); //mobile goal clamp
	pros::Distance mogoSensor(20); //distance sensor that checks for a mobile goal
	pros::lcd::set_text(2, "OK");

}

void disabled() { // when robot is disabled by the field
	lcdClear();
	pros::lcd::set_text(1, "Disabled");
}

void competition_initialize() { //pre-auto stuff here, runs after initialize()
	
}

void autonomous() { //put auto stuff here
	lcdClear();
	pros::lcd::set_text(1, "Running Auto");
	pros::MotorGroup left_mg({1,3,5});
	pros::MotorGroup right_mg({2,4,6});
	while(true) {
		
	}
}

void opcontrol() { //manual control, will run automatically if not connected to field
	//included control stuff
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup left_mg({1,3,5}); //left motors
	pros::MotorGroup right_mg({2,4,6}); //right motors
	pros::ADIDigitalOut mogoMech1('A'); //mogo mech piston
	pros::ADIDigitalOut mogoMech2('B'); //mogo mech piston
	pros::Distance mogoSensor(20); //mogo distance sensor

	bool mogoArmed = false;

	lcdClear();
	pros::lcd::print(1, "Driver Control");

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		int spd = (int(master.get_analog(ANALOG_LEFT_X))*-1);
		int turn = (int(master.get_analog(ANALOG_LEFT_Y))*-1);
		double sens = 0.6;
		if(turn>10 || turn<-10){
			left_mg.move(spd);
			right_mg.move(spd);
		} else {
		left_mg.move((spd + turn * sens)*-1);
		right_mg.move((spd - turn * sens)*-1);	
		}

		//toggle mogo armed
		if(mogoArmed == false){
			while(master.get_digital(DIGITAL_R1)){
				mogoArmed = true;
			}	
		} else if(mogoArmed == true){
			while (master.get_digital(DIGITAL_R1))
			{
				mogoArmed = false;
			}	
		}

		//arming, toggling, and disarming the mogo mech
		if(mogoArmed == true && mogoSensor.get_distance()<110){
			mogoMech1.set_value(HIGH);
			mogoMech2.set_value(HIGH);
		} else if(mogoArmed == false){
			mogoMech1.set_value(LOW);
			mogoMech2.set_value(LOW);
		}

		pros::delay(20);
	}
}