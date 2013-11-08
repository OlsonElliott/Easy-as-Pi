//*!!Sensor,    S4,                sonar, sensorSONAR9V,      ,                  !!*//
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                       Comparing Float vs Brake Mode for Motors
//
// This program provides two graphs of distance travelled vs various motor power levels. It
// compares using "float" ("coast") during the inactive PWM cycles vs "brake" mode during the
// inactive PWM cycles.
//
// Brake mode provides a more linear relationship between powre level and speed.
//
// Also, for some strange reason, in float mode the motors do not begin to mode until the power
// level is almost 40.
//
// For each measurement point, the motors are first allowed to ramp up to a steady tate speed (i.e.
// 300 msec) and then the number of encoder counts accumulated over the next 100 msec is
// measured.
//
// Besides demonstrating the difference between float and brake modes. this program also illustrates
// how relatively easy it is to draw a graph on the NXT LCD display.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

//
// RobotC supports procedure "prototypes". These are simply declarations for the "header" of a
// procedure so that the procedure/subroutine can be called before it is actually defined.
//
void initializeGraphs();
void addDataPointFloat(int nXValue, int nEncoderCounts);
void addDataPointBrake(int nXValue, int nEncoderCounts);

//
// Standard 'C' does not support scalar 'const' declarations. RobotC does. It's nicer than using
//     "#define kGraphWidth 100"
//
const int kGraphFloatLeft =  0;
const int kGraphBrakeLeft = 50;
const int kGraphTop    		= 56;
const int kGraphBottom 		=  0;
const int kGraphHeight 		= kGraphTop - kGraphBottom + 1;
const int kGraphWidth 		= 100 / 2;

int index;
int nPower;

//
// Note variables can be initialized in the declaration statement with the use of an optional
// "= " clause after the declaration.
//
int   nMaxGraphPoint = 1;
float fGraphScaler   = 1;
int   nGraphFloat[kGraphWidth];
int   nGraphBrake[kGraphWidth];

task main()
{
	nVolume = 2;

	//
	// Grab LCD screen and initialize display
	//
	initializeGraphs();
	eraseDisplay();
	nxtDisplayTextLine(0, " Brake vs Float");
	nxtDisplayTextLine(1, "  Acceleration");
	nxtDisplayTextLine(2, "   Analysis");
	nxtDisplayTextLine(3, "(100 Msec Tick)");

	//
	// Wait for motor to be connected
	//
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

	eraseDisplay();
	for (index = kGraphWidth - 1; index >= 0; --index)
	{
		nPower = index * 2 + 2;
		nxtDisplayTextLine(0, " Float %2d Brake", nPower);
		PlayTone( 750, 5);
		PlayTone(1500, 5);
		alive();

		//
		// Float speed for 0.5 second at selected power level
		//
		bFloatDuringInactiveMotorPWM = false;
		motor[motorA] = 0;  // Stop the motor
		wait1Msec(300);     // wait for stopped
		bFloatDuringInactiveMotorPWM = true;
		motor[motorA] = nPower;
		nMotorEncoder[motorA] = 0;
		wait1Msec(100);
		addDataPointFloat(index, nMotorEncoder[motorA]);
		//
		// Brake speed for 0.5 second at selected power level
		//
		bFloatDuringInactiveMotorPWM = false;
		motor[motorA] = 0;  // Stop the motor
		wait1Msec(300);     // wait for stopped
		bFloatDuringInactiveMotorPWM = false;
		motor[motorA] = nPower;
		nMotorEncoder[motorA] = 0;
		wait1Msec(100);
		addDataPointBrake(index, nMotorEncoder[motorA]);
	}
	nxtDisplayTextLine(0, " Float vs Brake");
	while (true)
	{
		// wait for user to cancel program
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
		nGraphFloat[i] = 0;
		nGraphBrake[i] = 0;
	}
	return;
}


void rescaleGraphs(int nCurrentMaximumValue)
{
	int i;

	nxtEraseRect(kGraphFloatLeft, kGraphTop, kGraphFloatLeft + kGraphWidth - 1, kGraphBottom);
	nxtEraseRect(kGraphBrakeLeft, kGraphTop, kGraphBrakeLeft + kGraphWidth - 1, kGraphBottom);
	nMaxGraphPoint = nCurrentMaximumValue;
	nxtDisplayString(1, "%d", nMaxGraphPoint);
	if (nCurrentMaximumValue == 0)
		nCurrentMaximumValue = 1;
	fGraphScaler = ((float) kGraphHeight) / nCurrentMaximumValue;
	for (i = 0; i < kGraphWidth; ++ i)
	{
		if (nGraphFloat[i] != 0)
		  nxtSetPixel(i + kGraphFloatLeft, kGraphBottom + (int) (nGraphFloat[i] * fGraphScaler));
		if (nGraphBrake[i] != 0)
		  nxtSetPixel(i + kGraphBrakeLeft, kGraphBottom + (int) (nGraphBrake[i] * fGraphScaler));
	}
	return;
}

void addDataPointFloat(int nXValue, int nEncoderCounts)
{
	if (nXValue > (kGraphWidth - 1))
		nXValue = kGraphWidth - 1;

	nGraphFloat[nXValue] = nEncoderCounts;
	if (nEncoderCounts < nMaxGraphPoint)
		nxtSetPixel(kGraphFloatLeft + nXValue, kGraphBottom + (int) (nEncoderCounts * fGraphScaler));
	else if (nEncoderCounts > nMaxGraphPoint)
		rescaleGraphs(nEncoderCounts);
	return;
}

void addDataPointBrake(int nXValue, int nEncoderCounts)
{
	if (nXValue > (kGraphWidth - 1))
		nXValue = kGraphWidth - 1;

	nGraphBrake[nXValue] = nEncoderCounts;
	if (nEncoderCounts < nMaxGraphPoint)
		nxtSetPixel(kGraphBrakeLeft + nXValue, kGraphBottom + (int) (nEncoderCounts * fGraphScaler));
	else if (nEncoderCounts > nMaxGraphPoint)
		rescaleGraphs(nEncoderCounts);
	return;
}

