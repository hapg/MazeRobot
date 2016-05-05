#include <arduino.h>
#include "Robot.h"

Robot* robot;

////////////////////////////////////////////////////////////////////////////////
void setup()
{
	Serial.begin(9600);

	robot = new Robot();
	robot->InitializeRobot();
}

////////////////////////////////////////////////////////////////////////////////
void loop()
{
	Robot::State state;

	while (robot->GetRobotState() != Robot::State_FINISHED)
	{
		robot->UpdateRobot();
	}

	delete robot;
}


/*
int pwm_lf = 3;
int pwm_lr = 2;
int pwm_rf = 4;
int pwm_rr = 5;

// motor tuning vars for maze navigating
int calSpeed = 165;   // tune value motors will run while auto calibration sweeping turn (0-255)
int turnSpeed = 200;  // tune value motors will run while turning (0-255)
int turnSpeedSlow = 125;  // tune value motors will run as they slow down from turning cycle to avoid overrun (0-255)
int drivePastDelay = 300; // tune value in mseconds motors will run past intersection to align wheels for turn

// pid loop vars
float error=0;
float lastError=0;
float PV =0 ;
float kp = 0;  // tune value in follow_line() function
//float ki = 0; // ki is not currently used
float kd =0;   // tune value in follow_line() function
int m1Speed=0;
int m2Speed=0;
int motorspeed=0;


// The path variable will store the path that the robot has taken.  It
// is stored as an array of characters, each of which represents the
// turn that should be made at one intersection in the sequence:
//  'L' for left
//  'R' for right
//  'S' for straight (going straight through an intersection)
//  'B' for back (U-turn)
// You should check to make sure that the path_length of your
// maze design does not exceed the bounds of the array.
char path[100] = "";
unsigned char path_length = 0; // the length of the path

void setup()
{
pinMode(pwm_lf, OUTPUT);  //Set control pins to be outputs
pinMode(pwm_lr, OUTPUT);
pinMode(pwm_rf, OUTPUT);
pinMode(pwm_rr, OUTPUT);

analogWrite(pwm_lf, 0);  //set both motors to stop at (100/255 = 39)% duty cycle (slow)
analogWrite(pwm_rf, 0);

//Serial.begin(115200);
Serial.begin(9600);

//calibrateSensor();

}


void loop()
{

// read calibrated sensor values and obtain a measure of the line position from 0 to 7000
// To get raw sensor values, call:
//  qtrrc.read(sensorValues); instead of unsigned int position = qtrrc.readLine(sensorValues);
unsigned int wall_angle = get_wall_angle();
unsigned int wall_distance = get_wall_distance();


//Serial.println(); // uncomment this line if you are using raw values
Serial.print(wall_angle); // comment this line out if you are using raw values
Serial.print("  ");
Serial.print(wall_distance);
Serial.print("  ");

// begin maze solving
MazeSolve(); // comment out and run serial monitor to test sensors while manually sweeping across line

Serial.print("::");
Serial.print(error);
Serial.print('\t');
Serial.print(PV);
Serial.print('\t');
Serial.print(m1Speed);
Serial.print('\t');
Serial.print(m2Speed);
Serial.print('\t');
Serial.print(lastError);
Serial.print('\t');

}  // end main loop



//line following subroutine
// PD Control
void follow_wall()  //follow the wall
{

lastError = 0;

while(1)
{

	wall_angle = get_wall_angle();
	wall_distance = get_wall_distance();

	error = (float)wall_distance - 6;

	// set the motor speed based on proportional and derivative PID terms
	// kp is the a floating-point proportional constant (maybe start with a value around 0.5)
	// kd is the floating-point derivative constant (maybe start with a value around 1)
	// note that when doing PID, it's very important you get your signs right, or else the
	// control loop will be unstable
	kp=.5;
	kd=1;

	PV = kp * error + kd * (error - lastError);
	lastError = error;

	//this codes limits the PV (motor speed pwm value)
	// limit PV to 55
	if (PV > 55)
	{

	PV = 55;
	}

	if (PV < -55)
	{
	PV = -55;
	}

	m1Speed = 200 + PV;
	m2Speed = 200 - PV;

	//set motor speeds
	analogWrite(pwm_lf, m2Speed);
	analogWrite(pwm_rf, m1Speed);

	// We use the inner six sensors (1 thru 6) to
	// determine if there is a line straight ahead, and the
	// sensors 0 and 7 if the path turns.
	if( wall_angle > 10)
	{
	// we need to turn left, not try to find the middle
	return;
	}

	else if( wall angle < -10)
	{
	// we need to turn right, not find the middle
	return;
	}

}

} // end follow_line



// Turns to the sent variable of
// 'L' (left), 'R' (right), 'S' (straight), or 'B' (back)
// Tune 'turnSpeed' at declaration
void turn(char dir)
{
switch(dir)
{
	// Turn left 90deg
	case 'L':
	analogWrite(pwm_lr, turnSpeed);
	analogWrite(pwm_rf, turnSpeed);

	wall_angle = get_wall_angle();
	while (wall_angle > 10)  // turn left until near parallel to wall
	{
		wall_angle = get_wall_angle();
	}

	// slow down speed
	analogWrite(pwm_lr, turnSpeedSlow);
	analogWrite(pwm_rf, turnSpeedSlow);

	while (wall_angle > 5)   // turn slight left
	{
		wall_angle = get_wall_angle();
	}

	// stop both motors
	analogWrite(pwm_rf, 0);  // stop right motor first to better avoid over run
	analogWrite(pwm_lr, 0);
	break;

	// Turn right 90deg
	case 'R':
	analogWrite(pwm_lf, turnSpeed);
	analogWrite(pwm_rr, turnSpeed);

	wall_angle = get_wall_angle();
	while (wall_angle < -10)  // turn left until near parallel to wall
	{
		wall_angle = get_wall_angle();
	}

	// slow down speed
	analogWrite(pwm_lf, turnSpeedSlow);
	analogWrite(pwm_rr, turnSpeedSlow);

	while (wall_angle < -5))   // turn slight right
	{
		wall_angle = get_wall_angle();
	}

	// stop both motors
	analogWrite(pwm_lf, 0);
	analogWrite(pwm_rr, 0);
	break;

	// Turn right 180deg to go back
	case 'B':

	// IAN: I'm not sure this case is needed. Isn't this just 2 right turns?
	analogWrite(pwm_rr, turnSpeed);
	analogWrite(pwm_lf, turnSpeed);

	wall_angle = get_wall_angle();
	wall_distance = get_wall_distance();

	while (wall_angle < -10)  // turn left until near parallel to wall
	{
		wall_angle = get_wall_angle();
		wall_distance = get_wall_distance();
	}

	// slow down speed
	analogWrite(pwm_rr, turnSpeedSlow);
	analogWrite(pwm_lr, turnSpeedSlow);

	// find center
	while (line_position < 3250)  // tune - wait for line position to find near center
	{
		line_position = qtrrc.readLine(sensorValues);
	}

	// stop both motors
	analogWrite(pwm_rr, 0);
	analogWrite(pwm_lr, 0);
	break;

	// Straight ahead
	case 'S':
	// do nothing
	break;
}
} // end turn



// Path simplification.  The strategy is that whenever we encounter a
// sequence xBx, we can simplify it by cutting out the dead end.  For
// example, LBL -> S, because a single S bypasses the dead end
// represented by LBL.
void simplify_path()
{
// only simplify the path if the second-to-last turn was a 'B'
if(path_length < 3 || path[path_length-2] != 'B')
	return;

int total_angle = 0;
int i;
for(i=1;i<=3;i++)
{
	switch(path[path_length-i])
	{
	case 'R':
		total_angle += 90;
	break;
	case 'L':
	total_angle += 270;
	break;
	case 'B':
	total_angle += 180;
	break;
	}
}

// Get the angle as a number between 0 and 360 degrees.
total_angle = total_angle % 360;

// Replace all of those turns with a single one.
switch(total_angle)
{
	case 0:
	path[path_length - 3] = 'S';
	break;
	case 90:
	path[path_length - 3] = 'R';
	break;
	case 180:
	path[path_length - 3] = 'B';
	break;
	case 270:
	path[path_length - 3] = 'L';
	break;
}

// The path is now two steps shorter.
path_length -= 2;

} // end simplify_path


// This function is called once, from the main loop
void MazeSolve()
{
// Loop until we have solved the maze.
while(1)
{
	// FIRST MAIN LOOP BODY
	follow_wall();

	// Drive straight a bit.  This helps us in case we entered the
	// intersection at an angle.
	analogWrite(pwm_lf, 200);
	analogWrite(pwm_rf, 200);
	delay(25);

	// These variables record whether the robot has seen a line to the
	// left, straight ahead, and right, whil examining the current
	// intersection.
	wall_angle = get_wall_angle();
	unsigned char dir = "S";
	if (wall_angle > 20){
	dir = "L";
	}
	else if (wall_angle < -20){
	dir = "R";
	}

	// Drive straight a bit more - this is enough to line up our
	// wheels with the intersection.
	analogWrite(pwm_lf, 200);
	analogWrite(pwm_rf, 200);
	delay(drivePastDelay);

	// Check for the ending spot.
	// If all six middle sensors are on dark black, we have
	// solved the maze.
	wall_angle = get_wall_angle();
	wall_distance = get_wall_distance();
	if( wall_distance < 0 && wall_angle > 90 ){
	// can't see wall, must be out of maze
	break;
	}

	// Make the turn indicated by the path.
	turn(dir);

	// Store the intersection in the path variable.
	path[path_length] = dir;
	path_length ++;

	// Simplify the learned path.
	simplify_path();
}

// Solved the maze!
return;

// Now enter an infinite loop - we can re-run the maze as many
// times as we want to.
while(1)
{
	//  maybe you would like to add a blinking led or a beeper.
	//  we just have it waiting patiently to be placed back on the starting line.
	analogWrite(pwm_a, 0);  // stop both motors
	analogWrite(pwm_b, 0);

	// while(1){}; // uncomment this line to cause infinite loop to test if end was found if your robot never seems to stop

	// hold motors while robot is sitting on end point
	line_position = qtrrc.readLine(sensorValues);
	while(sensorValues[1] > 200 && sensorValues[2] > 200 && sensorValues[3] > 200 && sensorValues[4] > 200 && sensorValues[5] > 200 && sensorValues[6] > 200)
	{
	line_position = qtrrc.readLine(sensorValues);
	delay(50);
	}

	// hold until the start line is seen again when the robot has been placed there again
	while(1)
	{
	line_position = qtrrc.readLine(sensorValues);
	if(sensorValues[2] > 200 || sensorValues[3] > 200 || sensorValues[4] > 200 || sensorValues[5] > 200 && sensorValues[0] < 200 && sensorValues[1] < 200 && sensorValues[6] < 200 && sensorValues[7] < 200)
	break;
	delay(50);
	}

	// delay to give you time to let go of the robot
	delay(2000);

	// Re-run the now solved maze.  It's not necessary to identify the
	// intersections, so this loop is really simple.
	int i;
	for(i=0;i<path_length;i++)
	{
	// SECOND MAIN LOOP BODY
	follow_line();

	// drive past intersection slightly slower and timed delay to align wheels on line
	digitalWrite(dir_a, LOW);
	analogWrite(pwm_a, 200);
	digitalWrite(dir_b, LOW);
	analogWrite(pwm_b, 200);
	delay(drivePastDelay); // tune time to allow wheels to position for correct turning

	// Make a turn according to the instruction stored in
	// path[i].
	turn(path[i]);
	}

	// Follow the last segment up to the finish.
	follow_line();

	// drive past intersection slightly slower and timed delay to align wheels on line
	digitalWrite(dir_a, LOW);
	analogWrite(pwm_a, 200);
	digitalWrite(dir_b, LOW);
	analogWrite(pwm_b, 200);
	delay(drivePastDelay); // tune time to allow wheels to position for correct turning

	// Now we should be at the finish!  Now move the robot again and it will re-run this loop with the solution again.

} // end running solved

} // end MazeSolve
*/
