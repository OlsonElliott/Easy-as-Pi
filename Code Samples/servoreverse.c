/* Pass in a speed between -100 and 100, inclusive */
void setContinuousServo(TServoIndex theServo, int speed, int reversed = 0) {
    if(!reversed) {
        /* Speed is now between 0 and 200 */
        speed += 100;
        /* Speed is now between 0 and 255 */
        speed = (int) ( ((float) speed) * ((255) / (200)) );
    } else {
        /* Speed is now between 200 and 0 */
        speed = abs(speed - 100);
        /* Speed is now between 255 and 0 */
        speed = (int) ( ((float) speed) * ((255) / (200)) );
    }

    servo[theServo] = speed;
}