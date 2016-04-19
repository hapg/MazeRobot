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

		float PulseForInches();
		float PulseForCentimeters();

	private:
		int m_trigPin;
		int m_echoPin;
};

#endif  // HC_SR04_H
