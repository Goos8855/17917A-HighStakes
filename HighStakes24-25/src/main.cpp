#include "main.h"
#include "pros/apix.h"

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
	pros::ADIDigitalOut mogoMech('H'); //mobile goal clamp
	pros::ADIDigitalOut wallMech ('G'); //wall stake thingy
	pros::MotorGroup intake({8});
	pros::MotorGroup upperIntake({7});
	pros::Distance intakeSensor(9);
	pros::Distance mogoSensor(20); //distance sensor that checks for a mobile goal
	pros::lcd::set_text(2, "OK");

}

void disabled() { // when robot is disabled by the field
	lcdClear();
	pros::lcd::set_text(6, "17917A: A nother Robot");
	pros::lcd::set_text(1, "Disabled");
}

void competition_initialize() { //pre-auto stuff here, runs after initialize()
	
}

void autonomous() { //put auto stuff here
	lcdClear();
	pros::lcd::set_text(6,"17917A: A nother Robot");
	pros::lcd::set_text(1, "Running Auto");
	pros::MotorGroup left_mg({1,3,5});
	pros::MotorGroup right_mg({2,4,6});
	while(true) {
		
	}
}

void opcontrol() { //manual control, will run automatically if not connected to field
	//included control stuff
	pros::lcd::set_text(6,"17917A: A nother Robot");
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup left_mg({1,3,5}); //left motors
	pros::MotorGroup right_mg({2,4,6}); //right motors
	pros::ADIDigitalOut mogoMech ('H');//mogo mech piston
	pros::ADIDigitalOut wallMech ('G'); //wall stake thingy
	pros::MotorGroup intake({8}); // lower part of intake
	pros::MotorGroup upperIntake({7}); //upper part of intake
	pros::Distance intakeSensor(11);
	pros::MotorGroup wallStake({-20}); // basket motor thingy (what do you call that?)

	bool mogoTriggered = false;
	bool intakeToggle = false;
	bool intakeReverse = false;
	bool wallStakeToggle = false;
	int distance = 10000000;

	lcdClear();
	pros::lcd::print(1, "Driver Control");

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		int spd = (int(master.get_analog(ANALOG_LEFT_X))*-1);
		int turn = (int(master.get_analog(ANALOG_LEFT_Y))*-1);
		double sens = 0.6;
		left_mg.move((spd + turn * sens)*-1);
		right_mg.move((spd - turn * sens)*-1);

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
			mogoTriggered = !mogoTriggered;
		}

		int intakeSpeed = int(master.get_analog(ANALOG_RIGHT_Y));

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
			intakeToggle = !intakeToggle;
		}

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
			intakeReverse = !intakeReverse;
		}

		if(master.get_analog(ANALOG_RIGHT_Y) == 0){

			distance = intakeSensor.get();
			pros::lcd::print(3, "%d", distance);

			if(intakeToggle == true && intakeReverse == false){
				if(intakeSensor.get() < 70){
					intake.brake();
				} else {
					intake.move(-127);
				}
				upperIntake.move(-127);
				wallStake.move(127);
			} else if(intakeToggle == true && intakeReverse == true){
				if(intakeSensor.get() > 70){
					intake.brake();
				} else {
					intake.move(127);
				}
				upperIntake.move(127);
				wallStake.move(-127);
			} else {
				intake.brake();
				upperIntake.brake();
				wallStake.brake();
			}
		} else {
			if(intakeSensor.get() < 70){
				intake.brake();
			} else {
				intake.move(master.get_analog(ANALOG_RIGHT_Y));
			}
			
			upperIntake.move(master.get_analog(ANALOG_RIGHT_Y));
			wallStake.move(master.get_analog(ANALOG_RIGHT_Y));
		}

		if(mogoTriggered == true){
			mogoMech.set_value(true);
		} else {
			mogoMech.set_value(false);
		}

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
			wallStakeToggle = !wallStakeToggle;
		}

		if(wallStakeToggle == true){
			wallMech.set_value(true);
		} else {
			wallMech.set_value(false);
		}

		pros::delay(20);
	}
}