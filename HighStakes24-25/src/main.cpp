#include "main.h"
#include <math.h>

//declaring ports n stuff
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({15,17,19});//left motors
pros::MotorGroup right_mg({16,18,20});//right motors
pros::ADIDigitalOut mogoMech ('H');//mogo mech piston
pros::MotorGroup LeftWall({-6}); // lower part of intake
pros::MotorGroup RightWall({7}); //upper part of intake
pros::Distance intakeSensor(9);
pros::MotorGroup upperIntake({3});
pros::MotorGroup lowerIntake({2});
pros::Rotation yAxis(11);

bool mogoTriggered = false;
bool intakeToggle = false;
bool intakeReverse = false;
bool wallStakeToggle = false;
int distance = 10000000;
bool wallStakeLock = false;
int updateDelay = 0;

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
		double sens = 1.25;
		left_mg.move((spd + turn * sens)*-1);
		right_mg.move((spd - turn * sens)*-1);

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
			mogoTriggered = !mogoTriggered;
		}

		int intakeSpeed = int(master.get_analog(ANALOG_RIGHT_Y))*-1;
		upperIntake.move(intakeSpeed);
		lowerIntake.move(intakeSpeed);


		if(mogoTriggered == true){
			mogoMech.set_value(true);
		} else {
			mogoMech.set_value(false);
		}

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
			wallStakeToggle = !wallStakeToggle;
		}

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
			wallStakeLock = !wallStakeLock;
		}

		if(wallStakeLock){
			LeftWall.move_absolute(350,1200);
			RightWall.move_absolute(350,1200);
		}

		if(wallStakeToggle && wallStakeLock == false){
			LeftWall.move_absolute(1400,1200);
			RightWall.move_absolute(1400,1200);
		} else if (wallStakeLock == false && wallStakeToggle == false) {
			LeftWall.move_absolute(0,600);
			RightWall.move_absolute(0,600);
		}
		pros::delay(20);

		if(updateDelay<6){
			++updateDelay;
		} else {
			updateDelay = 0;
			master.set_text(0, 0, std::to_string(mogoTriggered) + " " + std::to_string(wallStakeToggle));
		}
	}
}