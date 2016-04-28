// ==================================================
//	Created by Kevin Smith
//	Copyright 2016
//	
//	Disney Arduino robot code
// ==================================================

#include "Robot.h"
#include <arduino.h>

// left motor
#define MOTOR_LF_PIN 4
#define MOTOR_LB_PIN 5
#define MOTOR_LEFT_PWM 7

// right motor
#define MOTOR_RF_PIN 2
#define MOTOR_RB_PIN 3
#define MOTOR_RIGHT_PWM 6

// maximum pwm of motor (0-255)
#define MOTOR_SPEED_MAX 180
#define MOTOR_SPEED_MIN 50

// sonar pins
#define SONAR_LEFT_TRIGGER_PIN 10
#define SONAR_LEFT_ECHO_PIN 11
#define SONAR_RIGHT_TRIGGER_PIN 8
#define SONAR_RIGHT_ECHO_PIN 9

Robot::Robot()
{
}

Robot::~Robot()
{
	delete m_sonarLeft;
	delete m_sonarRight;
}

void Robot::InitializeRobot()
{
	pinMode(MOTOR_LF_PIN, OUTPUT);		// motor left forward pin
	pinMode(MOTOR_LB_PIN, OUTPUT);		// motor left back pin
	pinMode(MOTOR_RF_PIN, OUTPUT);		// motor right forward pin
	pinMode(MOTOR_RB_PIN, OUTPUT);		// motor right back pin
	pinMode(MOTOR_LEFT_PWM, OUTPUT);	// pulse width module for left motor
	pinMode(MOTOR_RIGHT_PWM, OUTPUT);	// pulse width module for right motor

	// create sonar objects
	m_sonarLeft = new HC_SR04(SONAR_LEFT_TRIGGER_PIN, SONAR_LEFT_ECHO_PIN);
	m_sonarRight = new HC_SR04(SONAR_RIGHT_TRIGGER_PIN, SONAR_RIGHT_ECHO_PIN);

	m_motorSpeedRight = 0;
	m_motorSpeedLeft = 0;

	m_robotState = State_READY;
}

void Robot::UpdateRobot()
{
	switch (m_robotState)
	{
	case Robot::State_READY:
		m_robotState = State_SOLVING;
		break;
	case Robot::State_SOLVING:
		SolvingUpdate();
		break;
	case Robot::State_FINISHED:
		break;
	default:
		break;
	}
}

void Robot::SolvingUpdate()
{
	float leftSonarIn = m_sonarLeft->PulseForInches();
	float rightSonarIn = m_sonarRight->PulseForCentimeters();

	Serial.print("Left Sonar:  ");
	Serial.println(leftSonarIn);
	Serial.print("Right Sonar: ");
	Serial.println(rightSonarIn);

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_FORWARD);
	AccelerateTo(MOTOR_SPEED_MAX, 1);
	delay(1500);

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_RIGHT);
	AccelerateTo(128, 1);
	delay(500);

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_FORWARD);
	AccelerateTo(MOTOR_SPEED_MAX, 1);
	delay(1500);

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_RIGHT);
	AccelerateTo(128, 1);
	delay(500);

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_FORWARD);
	AccelerateTo(MOTOR_SPEED_MAX, 1);
	delay(1500);

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_RIGHT);
	AccelerateTo(128, 1);
	delay(500);

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_FORWARD);
	AccelerateTo(MOTOR_SPEED_MAX, 1);
	delay(1500);

	//------

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_BACKWARD);
	AccelerateTo(MOTOR_SPEED_MAX, 1);
	delay(1500);

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_LEFT);
	AccelerateTo(128, 1);
	delay(500);

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_BACKWARD);
	AccelerateTo(MOTOR_SPEED_MAX, 1);
	delay(1500);

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_LEFT);
	AccelerateTo(128, 1);
	delay(500);

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_BACKWARD);
	AccelerateTo(MOTOR_SPEED_MAX, 1);
	delay(1500);

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_LEFT);
	AccelerateTo(128, 1);
	delay(500);

	SlowTo(MOTOR_SPEED_MIN, 1);
	SetDirection(Direction_BACKWARD);
	AccelerateTo(MOTOR_SPEED_MAX, 1);
	delay(1500);
}

void Robot::SetDirection(Direction dir)
{
	switch (dir)
	{
	case Direction_FORWARD:
		digitalWrite(MOTOR_LF_PIN, HIGH);
		digitalWrite(MOTOR_LB_PIN, LOW);
		digitalWrite(MOTOR_RF_PIN, HIGH);
		digitalWrite(MOTOR_RB_PIN, LOW);
		break;
	case Direction_BACKWARD:
		digitalWrite(MOTOR_LF_PIN, LOW);
		digitalWrite(MOTOR_LB_PIN, HIGH);
		digitalWrite(MOTOR_RF_PIN, LOW);
		digitalWrite(MOTOR_RB_PIN, HIGH);
		break;
	case Direction_LEFT:
		digitalWrite(MOTOR_LF_PIN, LOW);
		digitalWrite(MOTOR_LB_PIN, HIGH);
		digitalWrite(MOTOR_RF_PIN, HIGH);
		digitalWrite(MOTOR_RB_PIN, LOW);
		break;
	case Direction_RIGHT:
		digitalWrite(MOTOR_LF_PIN, HIGH);
		digitalWrite(MOTOR_LB_PIN, LOW);
		digitalWrite(MOTOR_RF_PIN, LOW);
		digitalWrite(MOTOR_RB_PIN, HIGH);
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
void Robot::SlowTo(float pwmSpeed, float decelerateStep)
{
	while (m_motorSpeedLeft > pwmSpeed && m_motorSpeedRight > pwmSpeed)
	{
		m_motorSpeedLeft -= decelerateStep;
		if (m_motorSpeedLeft < pwmSpeed)
			m_motorSpeedLeft = pwmSpeed;

		m_motorSpeedRight -= decelerateStep;
		if (m_motorSpeedRight < pwmSpeed)
			m_motorSpeedRight = pwmSpeed;

		analogWrite(MOTOR_LEFT_PWM, m_motorSpeedLeft);
		analogWrite(MOTOR_RIGHT_PWM, m_motorSpeedRight);
	}

	m_motorSpeedLeft = pwmSpeed;
	m_motorSpeedRight = pwmSpeed;

	analogWrite(MOTOR_LEFT_PWM, m_motorSpeedLeft);
	analogWrite(MOTOR_RIGHT_PWM, m_motorSpeedRight);

	digitalWrite(MOTOR_LF_PIN, LOW);
	digitalWrite(MOTOR_LB_PIN, LOW);
	digitalWrite(MOTOR_RF_PIN, LOW);
	digitalWrite(MOTOR_RB_PIN, LOW);
}

////////////////////////////////////////////////////////////////////////////////
void Robot::AccelerateTo(float pwmSpeed, float accelerateStep)
{
	while (m_motorSpeedLeft < pwmSpeed && m_motorSpeedRight < pwmSpeed)
	{
		m_motorSpeedLeft += accelerateStep;
		if (m_motorSpeedLeft > pwmSpeed)
			m_motorSpeedLeft = pwmSpeed;

		m_motorSpeedRight += accelerateStep;
		if (m_motorSpeedRight > pwmSpeed)
			m_motorSpeedRight = pwmSpeed;

		analogWrite(MOTOR_LEFT_PWM, m_motorSpeedLeft);
		analogWrite(MOTOR_RIGHT_PWM, m_motorSpeedRight);
	}

	m_motorSpeedLeft = pwmSpeed;
	m_motorSpeedRight = pwmSpeed;

	analogWrite(MOTOR_LEFT_PWM, m_motorSpeedLeft);
	analogWrite(MOTOR_RIGHT_PWM, m_motorSpeedRight);
}