// ==================================================
//	Created by Kevin Smith
//	Copyright 2016
//	
//	Disney Arduino robot code
// ==================================================

#ifndef ROBOT_H
#define ROBOT_H

#include "HC_SR04.h"    // required for sonar

class Robot
{
public:
	enum State
	{
		State_READY,
		State_SOLVING,
		State_FINISHED
	};

	enum Direction
	{
		Direction_FORWARD,
		Direction_BACKWARD,
		Direction_LEFT,
		Direction_RIGHT
	};

	Robot();
	~Robot();
	void InitializeRobot();
	void UpdateRobot();
	State GetRobotState() { return m_robotState; }

private:
	void SetDirection(Direction dir);
	void SetLeftMotorPwmTarget(float pwmSpeed, float accelerationStep = 0.01f);
	void SetRightMotorPwmTarget(float pwmSpeed, float accelerationStep = 0.01f);
	void SlowTo(float pwmSpeed, float decelerateStep);
	void AccelerateTo(float pwmSpeed, float accelerateStep);
	void UpateMotorSpeeds();
	void SolveMazeUpdate();

	State m_robotState;					// current state of robot

	// sonar objects
	HC_SR04* m_sonarLeft;
	HC_SR04* m_sonarRight;

	// motors
	float m_motorLeftPwm;				// current motor pwm speeds
	float m_motorRightPwm;
	float m_motorLeftPwmTarget;			// target motor pwm speeds
	float m_motorRightPwmTarget;
	float m_motorLeftStep;				// amount of acceleration applied each update
	float m_motorRightStep;
};

#endif	// ROBOT_H