// ==================================================
//	Created by Kevin Smith
//	Copyright 2016
//	
//	Arduino class for the HC_SR04 ultrasonic shield
// ==================================================

#ifndef HC_SR04_H
#define HC_SR04_H

class HC_SR04
{
public:
	HC_SR04(int trigPin, int echoPin);
	~HC_SR04();

	float PulseForInches();			// returns inches from object to sonar
	float PulseForCentimeters();	// returns centimeters from object to sonar

private:
	int m_trigPin;					// HC_SR04 Trig pin
	int m_echoPin;					// HC_SR04 Echo pin
};

#endif  // HC_SR04_H
