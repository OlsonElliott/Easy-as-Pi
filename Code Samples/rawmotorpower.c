///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                     Raw Motor Power vs PID COntrolled Speed
//
// Small little program to demonstrate the NXT integrated PID controller for motor speed. Two different
// graphs are created showing open loop and closed loop motor control.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

//
// RobotC supports procedure "prototypes". These are simply declarations for the "header" of a
// procedure so that the procedure/subroutine can be called before it is actually defined.
//
void initializeGraphs();
void addDataPointOpenLoop(int nXValue, int nEncoderCounts);
void addDataPointClosedLoop(int nXValue, int nEncoderCounts);

//
// Standard 'C' does not support scalar 'const' declarations. RobotC does. It's nicer than using
//     "#define kGraphWidth 100"
//
const int kGraphOpenLoopLeft =  0;
const int kGraphClosedLoopLeft = 50;
const int kGraphTop    		= 55;
const int kGraphBottom 		=  0;
const int kGraphHeight 		= kGraphTop - kGraphBottom + 1;
const int kGraphWidth 		= 100 / 2;

int index;
int nPower;

//
// Note variables can be initialized in the declaration statement with the use of an optional
// "= " clause after the declaration.
//
int   nMaxGraphPoint = kGraphHeight;
float fGraphScaler   = 1;
int   nGraphOpenLoop[kGraphWidth];
int   nGraphClosedLoop[kGraphWidth];

task main()
{
	nVolume = 2;

	//
	// Grab LCD screen and initialize display
	//
	initializeGraphs();
	eraseDisplay();
	nxtDisplayTextLine(0, "  Reg vs Unreg");
	nxtDisplayTextLine(1, "  Steady State");
	nxtDisplayTextLine(2, " Speed Analysis");

	//
	// Wait for motor to be connected
	//
	if (false)
	{
		bFloatDuringInactiveMotorPWM = false;
		nMotorEncoder[motorA] = 0;
		motor[motorA] = 100;
		while (nMotorEncoder[motorA] < 10)
		{
			PlaySound(soundBeepBeep);
			nxtDisplayTextLine(6, " Connect Motor");
			nxtDisplayTextLine(7, "  to Port 1");
			wait1Msec(400);
			nxtDisplayTextLine(6, "");
			nxtDisplayTextLine(7, "");
			wait1Msec(400);
		}
		motor[motorA] = 0;
		wait1Msec(0x100);
	}

	eraseDisplay();
	bFloatDuringInactiveMotorPWM = true;
	for (index = kGraphWidth - 1; index >= 0; --index)
	{
		const int kWaitDurationStop 				= 500;
		const int kWaitDurationRampToSpeed 	= 500;
		const int kWaitDurationMeasure 			= 1000;

		nPower = index * 2 + 2;
		nxtDisplayTextLine(0, " UnReg  %2d  Reg ", nPower);
		if (4 == index % 5)
		{
			PlayTone( 750, 5);
			PlayTone(1500, 5);
		}
		alive();

		// Wait for motor stopped condition

		motor[motorA] = 0;
		wait1Msec(kWaitDurationStop);

		// wait for steady state speed
		//
		// Unregulated speed
		//
		nMotorPIDSpeedCtrl[motorA]   = mtrNoReg;
		motor[motorA] = 0;
		motor[motorA] = nPower;
		wait1Msec(kWaitDurationRampToSpeed); // wait for steady state speed
		nMotorEncoder[motorA] = 0;
		wait1Msec(kWaitDurationMeasure);
		addDataPointOpenLoop(index, nMotorEncoder[motorA]);

		// Wait for motor stopped condition

		motor[motorA] = 0;
		wait1Msec(kWaitDurationStop);

		//
		// Regulated speed for 0.5 second at selected power level
		//
		nMotorPIDSpeedCtrl[motorA]   = mtrSpeedReg;
		motor[motorA] = 0;
		motor[motorA] = nPower;
		wait1Msec(kWaitDurationRampToSpeed); // wait for steady state speed
		nMotorEncoder[motorA] = 0;
		wait1Msec(kWaitDurationMeasure);
		addDataPointClosedLoop(index, nMotorEncoder[motorA]);
	}
	nxtDisplayTextLine(0, " Unreg vs  Reg ");
	while (true)
	{
		// wait for user to cancel program
		alive();
	}
}


void initializeGraphs()
{
	//
	// Zero out the histogram entries
	//
	int i;
	for (i = 0; i < kGraphWidth; ++i)
	{
		nGraphOpenLoop[i] = 0;
		nGraphClosedLoop[i] = 0;
	}
	return;
}


void rescaleGraphs(int nCurrentMaximumValue)
{
	int i;

	nxtEraseRect(kGraphOpenLoopLeft,   kGraphTop, kGraphOpenLoopLeft   + kGraphWidth - 1, kGraphBottom);
	nxtEraseRect(kGraphClosedLoopLeft, kGraphTop, kGraphClosedLoopLeft + kGraphWidth - 1, kGraphBottom);
	nxtDrawLine(kGraphOpenLoopLeft, kGraphTop - 1, kGraphClosedLoopLeft + kGraphWidth - 1, kGraphTop - 1);
	nMaxGraphPoint = nCurrentMaximumValue;
	nxtDisplayString(1, "%d", nMaxGraphPoint);
	if (nCurrentMaximumValue == 0)
		nCurrentMaximumValue = 1;
	fGraphScaler = ((float) kGraphHeight) / nCurrentMaximumValue;

	for (i = 0; i < kGraphWidth; ++ i)
	{
		if (nGraphOpenLoop[i] != 0)
		  nxtSetPixel(i + kGraphOpenLoopLeft, kGraphBottom + (int) (nGraphOpenLoop[i] * fGraphScaler));
		if (nGraphClosedLoop[i] != 0)
		  nxtSetPixel(i + kGraphClosedLoopLeft, kGraphBottom + (int) (nGraphClosedLoop[i] * fGraphScaler));
	}
	return;
}

void addDataPointOpenLoop(int nXValue, int nEncoderCounts)
{
	if (nXValue > (kGraphWidth - 1))
		nXValue = kGraphWidth - 1;

	nGraphOpenLoop[nXValue] = nEncoderCounts;
	if (nEncoderCounts < nMaxGraphPoint)
		nxtSetPixel(kGraphOpenLoopLeft + nXValue, kGraphBottom + (int) (nEncoderCounts * fGraphScaler));
	else if (nEncoderCounts > nMaxGraphPoint)
		rescaleGraphs(nEncoderCounts);
	return;
}

void addDataPointClosedLoop(int nXValue, int nEncoderCounts)
{
	if (nXValue > (kGraphWidth - 1))
		nXValue = kGraphWidth - 1;

	nGraphClosedLoop[nXValue] = nEncoderCounts;
	if (nEncoderCounts < nMaxGraphPoint)
		nxtSetPixel(kGraphClosedLoopLeft + nXValue, kGraphBottom + (int) (nEncoderCounts * fGraphScaler));
	else if (nEncoderCounts > nMaxGraphPoint)
		rescaleGraphs(nEncoderCounts);
	return;
}

