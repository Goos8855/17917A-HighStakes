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
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup left_mg({1,3,5});
	pros::MotorGroup right_mg({2,4,6});
	pros::ADIDigitalOut mogoMech(1, false);
	pros::ADIDigitalOut mogoArm(2, false);
	pros::Rotation xSensor(20);
	pros::Rotation ySensor(19);
	pros::ADIDigitalIn mogoTrigger(3);
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
	pros::MotorGroup left_mg({1,3,5});
	pros::MotorGroup right_mg({2,4,6});
	pros::ADIDigitalOut mogoMech(1, false);
	pros::ADIDigitalOut mogoArm(2, false);
	pros::ADIDigitalIn mogoTrigger(3);

	bool mogoArmed = false;

	lcdClear();
	pros::lcd::print(1, "Driver Control");

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		// Arcade control scheme
		int leftTank = master.get_analog(ANALOG_LEFT_Y);
		int rightTank = master.get_analog(ANALOG_RIGHT_Y);
		double sens = 0.6;
		left_mg.move((leftTank * sens)*-1);
		right_mg.move((rightTank * sens)*-1);
		pros::delay(20);

		//triggering mogo arm
		if(master.get_digital(DIGITAL_L1)){
			mogoArm.set_value(0);
		} 
		if(master.get_digital(DIGITAL_L2)){
			mogoArm.set_value(1);
		}

		//arming and triggering mogo mech
		if(master.get_digital(DIGITAL_Y) || mogoArmed == true){
			mogoArmed = true;
			if(mogoTrigger.get_value()){
				mogoMech.set_value(1);
				mogoArmed = false;
			}
		} else {
			mogoMech.set_value(0);
		}
	}
}