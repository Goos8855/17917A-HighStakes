#include "main.h"

//declaring ports n stuff
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({15,17,19});//left motors
pros::MotorGroup right_mg({16,18,20});//right motors
pros::ADIDigitalOut mogoMech ('H');//mogo mech piston
pros::MotorGroup LeftWall({-8}); // lower part of intake
pros::MotorGroup RightWall({7}); //upper part of intake
pros::Distance intakeSensor(9);

bool mogoTriggered = false;
bool intakeToggle = false;
bool intakeReverse = false;
bool wallStakeToggle = false;
int distance = 10000000;

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
	
	pros::lcd::set_text(2, "OK");

}

void disabled() { // when robot is disabled by the field
	lcdClear();
	pros::lcd::set_text(1, "Disabled");
}

void competition_initialize() { //pre-auto stuff here, runs after initialize()
	
}

void pid(double dx, double dy) { //proportional, integral, derivative
	
}

void autonomous() { //put auto stuff here
	lcdClear();
	pros::lcd::set_text(1, "Running Auto");
		
}

void opcontrol() { //manual control, will run automatically if not connected to field
	//included control stuff

	lcdClear();
	pros::lcd::print(1, "Driver Control");

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		int spd = (int(master.get_analog(ANALOG_LEFT_X)));
		int turn = (int(master.get_analog(ANALOG_LEFT_Y)));
		double sens = 1;
		left_mg.move((spd + turn * sens)*-1);
		right_mg.move((spd - turn * sens)*-1);

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
			mogoTriggered = !mogoTriggered;
		}

		int intakeSpeed = int(master.get_analog(ANALOG_RIGHT_Y));

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
			intakeReverse = !intakeReverse;
		}

		LeftWall.move(master.get_analog(ANALOG_RIGHT_Y));
		RightWall.move(master.get_analog(ANALOG_RIGHT_Y));



		if(mogoTriggered == true){
			mogoMech.set_value(true);
		} else {
			mogoMech.set_value(false);
		}

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
			wallStakeToggle = !wallStakeToggle;
		}
		pros::delay(20);
	}
}