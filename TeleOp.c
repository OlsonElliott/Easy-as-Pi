#pragma config(Hubs,	S1, HTMotor,	HTMotor,	HTServo,	none)
#pragma config(Sensor, S1,		 ,							 sensorI2CMuxController)
#pragma config(Motor,	 motorA,					 ,						 tmotorNXT, PIDControl)
#pragma config(Motor,	 motorC,					 ,						 tmotorNXT, PIDControl)
#pragma config(Motor,	 mtr_S1_C1_1,			motorR,				 tmotorTetrix, openLoop)
#pragma config(Motor,	 mtr_S1_C1_2,			motorL,				 tmotorTetrix, openLoop, reversed)
#pragma config(Motor,	 mtr_S1_C2_1,			FlagMotor,		 tmotorTetrix, openLoop)
#pragma config(Motor,	 mtr_S1_C2_2,			motorWinch,		 tmotorTetrix, openLoop)
#pragma config(Servo,	 srvo_S1_C3_1,		servoFlip,						tServoStandard)
#pragma config(Servo,	 srvo_S1_C3_2,		hangServo,						tServoStandard)
#pragma config(Servo,	 srvo_S1_C3_3,		servo3,								tServoNone)
#pragma config(Servo,	 srvo_S1_C3_4,		servo4,								tServoNone)
#pragma config(Servo,	 srvo_S1_C3_5,		servo5,								tServoNone)
#pragma config(Servo,	 srvo_S1_C3_6,		servo6,								tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard							 !!*//

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//													 Tele-Operation Mode Code Template
//
// This file contains a template for simplified creation of an tele-op program for an FTC
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"	 //Include file to "handle" the Bluetooth messages.
#define JOYSTICK_MIN 10

const int rotateSpeed = 30;		 //speed for flag motor
int servoMoveRange = 140;
float hangArmPos=255.0;//starting position for lifting arm
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//																		initializeRobot
//
// Prior to the start of tele-op mode, you may want to perform some initialization on your robot
// and the variables within your program.
//
// In most cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
#define MOTOR_NUM								5
#define MOTOR_MAX_VALUE					127
#define MOTOR_MIN_VALUE					(-127)
#define MOTOR_DEFAULT_SLEW_RATE 7			 // Default will cause 375mS from full fwd to rev
#define MOTOR_FAST_SLEW_RATE		256			// essentially off
#define MOTOR_TASK_DELAY				15			// task 1/frequency in mS (about 66Hz)
#define MOTOR_DEADBAND					10

// Array to hold requested speed for the motors
int motorReq[ MOTOR_NUM ];

// Array to hold "slew rate" for the motors, the maximum change every time the task
// runs checking current mootor speed.
int motorSlew[ MOTOR_NUM ];
task MotorSlewRateTask()
{
	int motorIndex;
	int motorTmp;

	// Initialize stuff
	for(motorIndex=0;motorIndex<MOTOR_NUM;motorIndex++)
	{
		motorReq[motorIndex] = 0;
		motorSlew[motorIndex] = MOTOR_DEFAULT_SLEW_RATE;
	}

	// run task until stopped
	while( true )
	{
		// run loop for every motor
		for( motorIndex=0; motorIndex<MOTOR_NUM; motorIndex++)
		{
			// So we don't keep accessing the internal storage
			motorTmp = motor[ motorIndex ];

			// Do we need to change the motor value ?
			if( motorTmp != motorReq[motorIndex] )
			{
				// increasing motor value
				if( motorReq[motorIndex] > motorTmp )
				{
					motorTmp += motorSlew[motorIndex];
					// limit
					if( motorTmp > motorReq[motorIndex] )
						motorTmp = motorReq[motorIndex];
				}

				// decreasing motor value
				if( motorReq[motorIndex] < motorTmp )
				{
					motorTmp -= motorSlew[motorIndex];
					// limit
					if( motorTmp < motorReq[motorIndex] )
						motorTmp = motorReq[motorIndex];
				}

				// finally set motor
				motor[motorIndex] = motorTmp;
			}
		}

		// Wait approx the speed of motor update over the spi bus?
		wait1Msec( MOTOR_TASK_DELAY );
	}
}

void moveLiftArm()
{
	if (joy1Btn(6)&&hangArmPos>0)
	{
		hangArmPos -=0.1;
	}
	else if (joy1Btn(8)&&hangArmPos<255)
	{
		hangArmPos += 0.1;
	}
	servo[hangServo] = (int) hangArmPos;
	//wait10Msec(servo[hangServo]);

	//move autonomous arm out of the way

	if (hangArmPos<245)
	{
		servo[servoFlip]=127;
	}
	else if (hangArmPos>245)
	{
		servo[servoFlip]=0;
	}
}

void operateWinch()
{

	if (joy1Btn(2))
	{
		motor[motorWinch]=100;
	}

	if (joy1Btn(3))
	{
		motor[motorWinch]= -100;
	}
	if (joy1Btn(2)==0 && joy1Btn(3)==0)
	{
		motor[motorWinch]=0;
	}
}

void initializeRobot()
{
// Move servo1 to position to starting position
servo[hangServo]=hangArmPos;
}

// Operate the two drive motors
void driveMotors()
{
	int joyLeft = joystick.joy1_y1;
	int joyRight = -joystick.joy1_y2;

	// If the left or right joysticks are engaged beyond a minimum threshold, operate the drive
	// motors; else stop them. The threshold is needed because a joystick that's not being touched
	// can still show small non-zero values.
	// Uses exponential increase in speed to help smooth the control at slow speeds.
	if ((abs(joyLeft) > JOYSTICK_MIN) || (abs(joyRight) > JOYSTICK_MIN))
	{
		if (abs(joyLeft) < 80)
		{
			if (joyLeft > 0)
				joyLeft = -1*pow(1+.056,joyLeft);
			else
				joyLeft = 1*pow(1+.056,abs(joyLeft));
		}

		if (abs(joyRight) < 80)
		{
			if (joyRight > 0)
				joyRight = -1*pow(1+.056,joyRight);
			else
				joyRight = 1*pow(1+.056,abs(joyRight));
		}

		motorReq[motorL] = (joyLeft)*100/127;
		motorReq[motorR] = (joyRight)*-100/127;
		//motor[motorL] = (joyLeft)*100/127;
		//motor[motorR] = (joyRight)*-100/127;
	}
	else
	{
		motorReq[motorL] = 0;
		motorReq[motorR] = 0;
		//motor[motorL] = 0;
		//motor[motorR] = 0;
	}
}

void driveMotors (int lspeed, int rspeed)
{
//motorReq[motorL] = lspeed;
//motorReq[motorR] = rspeed;
motor[motorL] = lspeed;
motor[motorR] = rspeed;
}

//Raise Flag
void flagControl(int direction)
{
	if (direction == 0)	{
		motor[FlagMotor] = 0 ;		 // turn motor off
	}
	else	{
		motor[FlagMotor] = rotateSpeed*direction ;		 // turn flag
	}
}

int getFlagButtons()
{
	int button;
	if (joy1Btn(7))//Button for control of the motor to turn flag
		button = -1;
	else if (joy1Btn(5))
		button = 1;
	else
		button = 0;
	return button;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//																				 Main Task
//
// The following is the main code for the tele-op robot operation. Customize as appropriate for
// your specific robot.
//
// Game controller / joystick information is sent periodically (about every 50 milliseconds) from
// the FMS (Field Management System) to the robot. Most tele-op programs will follow the following
// logic:
//	 1. Loop forever repeating the following actions:
//	 2. Get the latest game controller / joystick settings that have been received from the PC.
//	 3. Perform appropriate actions based on the joystick + buttons settings. This is usually a
//			simple action:
//			*	 Joystick values are usually directly translated into power levels for a motor or
//				 position of a servo.
//			*	 Buttons are usually used to start/stop a motor or cause a servo to move to a specific
//				 position.
//	 4. Repeat the loop.
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
	//waitForStart();		// wait for start of tele-op phase
	//StartTask(MotorSlewRateTask);
	while (true)
	{
		///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////
		////																									 ////
		////			Add your robot specific tele-op code here.	 ////
		////																									 ////
		///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////

		getJoystickSettings(joystick);	 // Obtain current game controller settings

		// Display the settings on the NXT Brick
		nxtDisplayString(0, "joy1_x1: %d", joystick.joy1_x1);
		nxtDisplayString(1, "joy1_y1: %d", joystick.joy1_y1);
		nxtDisplayString(2, "joy1_x2: %d", joystick.joy1_x2);
		nxtDisplayString(3, "joy1_y2: %d", joystick.joy1_y2);
		nxtDisplayString(4, "Buttons: %d", joystick.joy1_Buttons);
		nxtDisplayString(5, "TopHat:	%d", joystick.joy1_TopHat);
		// Drive Motors
		flagControl(getFlagButtons());
		moveLiftArm();
		operateWinch();
		driveMotors();
	}
}
