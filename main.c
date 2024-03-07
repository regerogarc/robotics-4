#pragma config(Sensor, S3,     Colour,         sensorEV3_Color)
#pragma config(Sensor, S4,     US,             sensorEV3_Ultrasonic)
#pragma config(Motor,  motorC,          LeftMotor,     tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorA,          RightMotor,    tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          FlipMotor,     tmotorEV3_Medium, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define SIGHT_THRESHOLD		80
#define FLIP_THRESHOLD 		5

// Global Variables --------------------------------------------------------------------------------------------------------------------------------------

int white_colour;

int stopped = 0;
int distance = 0;

// Driver functions ---------------------------------------------------------------------------------------------------------------------------

void display_motor_states(int left_speed, int right_speed)
{
	displayCenteredTextLine(4, "Left motor: %d%%", left_speed);
	displayCenteredTextLine(5, "Right motor: %d%%", right_speed);
}

void turn()
{
	// move both motors in the opposite direction to rotate the robot
	stopped = 0;
	setMotorReversed(LeftMotor, 1);
	setMotorReversed(RightMotor, 0);
}

void forwards()
{
	stopped = 0;
	setMotorReversed(LeftMotor, 0);
	setMotorReversed(RightMotor, 0);
}

void backwards()
{
	stopped = 0;
	setMotorReversed(LeftMotor, 1);
	setMotorReversed(RightMotor, 1);
}

void stopRobot(void)
{
	// stop the robot's motors from moving
	stopped = 1;
}

// MUTEX LOCK ------------------------------------------------------------------------------------------------------------------------------------------------------------------

int state_lock = 0;

void aquire_state_lock() {
	displayCenteredTextLine(8, "Aquire lcok");
	while(state_lock){}
	state_lock = 1;
	displayCenteredTextLine(8, "Lock aquired");
}

void release_state_lock()
{
	displayCenteredTextLine(8, "Lock released");
	state_lock = 0;
}

// STATE MACHINE ---------------------------------------------------------------------------------------------------------------------------------------------------------------

enum States {
	SEARCHING,
	MOVING,
	EDGE_EVASION,
	ATTACK
};

int interrupt = 0;

States current_state = SEARCHING;

// State functions
void searching()
{
	displayCenteredTextLine(2, "Searching");
	turn();
	while(1)
	{
		if (interrupt) return;

		distance = SensorValue[US];
		if (distance < SIGHT_THRESHOLD)
		{
			displayCenteredTextLine(4, "Target Aquired");
			stopRobot();
			aquire_state_lock();
			current_state = MOVING;
			release_state_lock();
			return;
		}
		else {
			displayCenteredTextLine(4, "Searching");
		}
	}
}

void attack()
{
	displayCenteredTextLine(2, "Attacking");
	setMotorTarget(FlipMotor, 3600, 100);

	while(1)
	{
		if (interrupt) return;

		distance = SensorValue[US];
		if (distance > FLIP_THRESHOLD)
		{
			displayCenteredTextLine(4, "Target Out of Range");

			// Reset the flipper and change back to searching state
			setMotorTarget(FlipMotor, 0, 100);
			aquire_state_lock();
			current_state = SEARCHING;
			release_state_lock();
			return;
		}
		else if (getMotorEncoder(FlipMotor) == 3600)
		{
			setMotorTarget(FlipMotor, 0, 100);
		}
		else if (getMotorEncoder(FlipMotor) == 0)
		{
			setMotorTarget(FlipMotor, 3600, -100);
		}
	}
}

void moving()
{
	displayCenteredTextLine(2, "Moving");
	forwards();
	while(1)
	{
		if (interrupt) return;

		distance = SensorValue[US];
		if (distance > SIGHT_THRESHOLD)
		{
			displayCenteredTextLine(4, "Target Lost");
			stopRobot();
			aquire_state_lock();
			current_state = SEARCHING;
			release_state_lock();
			return;
		}
		else if (distance < FLIP_THRESHOLD)
		{
			displayCenteredTextLine(4, "Target In Range, Attacking");
			stopRobot();
			aquire_state_lock();
			current_state = ATTACK;
			release_state_lock();
			return;
		}
		else
		{
			displayCenteredBigTextLine(4, "Approaching Target");
		}
	}
}

void edge_evasion()
{
	displayCenteredTextLine(2, "Evading Edge");

	//drive(-100);
	delay(3000);
	stopRobot();

	//target_angle = getGyroDegrees();

	aquire_state_lock();
	current_state = SEARCHING;
	release_state_lock();
	return;
}


// TASKS --------------------------------------------------------------------------------------------------------------------------------------------------------


task drive_task()
{
	
	resetMotorEncoder(LeftMotor);
	resetMotorEncoder(RightMotor);
	while(1)
	{
		if (!stopped)
		{
			moveMotorTarget(LeftMotor, 720, drive_speed);
			moveMotorTarget(RightMotor, 720, drive_speed);
			waitUntilMotorStop(LeftMotor);
			waitUntilMotorStop(RightMotor);
		}
	}

}

task edge_detection()
{
	while(1)
	{
		displayCenteredTextLine(6, "Colour value: %d", SensorValue[Colour]);
		displayCenteredTextLine(7, "Colour thresh: %d", white_colour/3);

		if (SensorValue[Colour] < white_colour/3)
		{
			aquire_state_lock();
			current_state = EDGE_EVASION;
			interrupt = 1;
			release_state_lock();
		}
	}
}

task main()
{
	// Calibrate the colour sensor
	delay(500);

	int colour_average = 0;
	for (int i = 0; i < 20; i++)
	{
		colour_average += SensorValue[Colour];
		delay(25);
	}
	white_colour = colour_average/20;

	// Calibrate the flipper motor
	resetMotorEncoder(FlipMotor);

	//startTask(edge_detection);
	drive_speed = 100;
	startTask(drive);
	while(1)
	{
		//setMotorSync(LeftMotor, RightMotor, 0, 100);
		//delay(8000);
		//setMotorSync(LeftMotor, RightMotor, -1, 100);
		//delay(8000);
	}

	while(0)
	{
		//state_function[current_state]();
		// run the correct function for the current state
		switch (current_state)
		{
			case SEARCHING:
				searching();
				break;
			case MOVING:
				moving();
				break;
			case EDGE_EVASION:
				edge_evasion();
				break;
			case ATTACK:
				attack();
				break;
		}

		// Reset the interrupt flag
		interrupt = 0;
	}
}
