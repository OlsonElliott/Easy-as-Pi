#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,           ,             tmotorNXT, PIDControl)
#pragma config(Motor,  motorB,          motorTurn,     tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          motorGrab,     tmotorNXT, PIDControl)
#pragma config(Motor,  mtr_S1_C1_1,     FlagMotor,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorR,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorRA,       tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    servoTurn,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Tele-Operation Mode Code Template
//
// This file contains a template for simplified creation of an tele-op program for an FTC
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

#define JOYSTICK_MIN 10
int wrist_pos=24;          //starting position for wrist servo
bool rotateOn = false;
const int rotateSpeed = 5;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of tele-op mode, you may want to perform some initialization on your robot
// and the variables within your program.
//
// In most cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void initializeRobot()
{
	// Place code here to initialize servos to starting positions.
	// Disabling sensors because of memory bug.
	SensorType[S2] = sensorNone;
	SensorType[S3] = sensorNone;
	SensorType[S4] = sensorNone;
                             // Move servo1 to position to starting position
}

// Operate the two drive motors

//Arm Rotate

void flagControl(int direction)
{
	if (direction == 0)	{
		motor[FlagMotor] = 0 ;     // turn motor off
	}
	else if (direction == 1) {
		motor[FlagMotor] = rotateSpeed ;     // rotate motor clockwise
	}
}

int getFlagButtons () {
  int buttonUp = joy1Btn(6);
	return buttonUp;
}

//Arm Raise


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the tele-op robot operation. Customize as appropriate for
// your specific robot.
//
// Game controller / joystick information is sent periodically (about every 50 milliseconds) from
// the FMS (Field Management System) to the robot. Most tele-op programs will follow the following
// logic:
//   1. Loop forever repeating the following actions:
//   2. Get the latest game controller / joystick settings that have been received from the PC.
//   3. Perform appropriate actions based on the joystick + buttons settings. This is usually a
//      simple action:
//      *  Joystick values are usually directly translated into power levels for a motor or
//         position of a servo.
//      *  Buttons are usually used to start/stop a motor or cause a servo to move to a specific
//         position.
//   4. Repeat the loop.
//
// Your program needs to continuously loop because you need to continuously respond to changes in
// the game controller settings.
//
// At the end of the tele-op period, the FMS will autonmatically abort (stop) execution of the program.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	initializeRobot();

	//waitForStart();   // wait for start of tele-op phase

	while (true)
	{
		///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////
		////                                                   ////
		////      Add your robot specific tele-op code here.   ////
		////                                                   ////
		///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////

		getJoystickSettings(joystick);   // Obtain current game controller settings

		// Display the settings on the NXT Brick
		nxtDisplayString(0, "joy1_x1: %d", joystick.joy1_x1);
		nxtDisplayString(1, "joy1_y1: %d", joystick.joy1_y1);
		nxtDisplayString(2, "joy1_x2: %d", joystick.joy1_x2);
		nxtDisplayString(3, "joy1_y2: %d", joystick.joy1_y2);
		nxtDisplayString(4, "Buttons: %d", joystick.joy1_Buttons);
		nxtDisplayString(5, "TopHat:  %d", joystick.joy1_TopHat);

		flagControl(getFlagButtons());

	}
}
