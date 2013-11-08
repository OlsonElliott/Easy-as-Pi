void driveMotors()
{
	int joyLeft = joystick.joy1_y1;
	int joyRight = joystick.joy1_y2;

	// If the left or right joysticks are engaged beyond a minimum threshold, operate the drive
	// motors; else stop them. The threshold is needed because a joystick that's not being touched
	// can still show small non-zero values.
	// Uses exponential increase in speed to help smooth the control at slow speeds.
	if ((abs(joyLeft) > JOYSTICK_MIN) || (abs(joyRight) > JOYSTICK_MIN))
	{
		if (abs(joyLeft) < 80)
		{
			if (joyLeft > 0)
				joyLeft = 1*pow(1+.056,joyLeft);
			else
				joyLeft = -1*pow(1+.056,abs(joyLeft));
		}

		if (abs(joyRight) < 80)
		{
			if (joyRight > 0)
				joyRight = 1*pow(1+.056,joyRight);
			else
				joyRight = -1*pow(1+.056,abs(joyRight));
		}

		motor[motorL] = (joyLeft)*75/127;
		motor[motorR] = (joyRight)*75/127;   // Convert motor range of -75 to 75
	}
	else
	{
		motor[motorL] = 0;
		motor[motorR] = 0;
	}
}
