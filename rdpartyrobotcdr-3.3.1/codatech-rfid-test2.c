#pragma config(Sensor, S1,     CTRFID,              sensorI2CCustom9V)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
 * $Id: codatech-rfid-test2.c 133 2013-03-10 15:15:38Z xander $
 */

/**
 * codatech-rfid.h provides an API for the Codatex RFID sensor.  This program
 * demonstrates how to use that API to use the sensor in continuous mode.
 *
 * Changelog:
 * - 0.1: Initial release
 * - 0.2: Removed common.h include
 *
 * Credits:
 * - Big thanks to Sylvain Cacheux for writing the initial drivers.
 *
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 3.59 AND HIGHER. 

 * Xander Soldaat (xander_at_botbench.com)
 * 20 february 2011
 * version 0.2
 */

#include "drivers/codatech-rfid.h"

string transponderID;

task main()
{
  nNxtButtonTask  = -2;
  nxtDisplayCenteredTextLine(0, "Codatex");
  nxtDisplayCenteredBigTextLine(1, "RFID");
  nxtDisplayCenteredTextLine(3, "Test 2");
  nxtDisplayCenteredTextLine(5, "Connect sensor");
  nxtDisplayCenteredTextLine(6, "to S1");
  wait1Msec(2000);

  eraseDisplay();
  nxtDisplayCenteredTextLine(3, "Start single");
  nxtDisplayCenteredTextLine(4, "reading loop");
  wait1Msec(2000);
  eraseDisplay();

  // Set up the sensor for continuous readings.
  CTRFIDsetContinuous(CTRFID);

  // loop for transponder readings with continuous read function
  while(nNxtButtonPressed == kNoButton) {
    // read the transponder
    if (!CTRFIDreadTransponder(CTRFID, transponderID)) {
      eraseDisplay();
      nxtDisplayTextLine(3, "Error reading");
      nxtDisplayTextLine(4, "from sensor!");
      wait10Msec(5000);
      StopAllTasks();
    }
    nxtDisplayCenteredTextLine(3, "Transponder ID:");
    nxtDisplayCenteredTextLine(4, "%s", transponderID);

    // Be sure to add about 200ms after each read
    // or you end up getting 0000000000 as a transponder address
    wait1Msec(200);
  }
}

/*
 * $Id: codatech-rfid-test2.c 133 2013-03-10 15:15:38Z xander $
 */
