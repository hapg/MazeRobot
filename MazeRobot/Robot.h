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
	void SlowTo(float pwmSpeed, float decelerateStep);
	void AccelerateTo(float pwmSpeed, float accelerateStep);
	void SolvingUpdate();

	State m_robotState;		// current state robot is in

	// sonar objects
	HC_SR04* m_sonarLeft;
	HC_SR04* m_sonarRight;

	// motors
	float m_motorSpeedRight;
	float m_motorSpeedLeft;
};

#endif	// ROBOT_H