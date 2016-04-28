// ==================================================
//	Created by Kevin Smith
//	Copyright 2016
//	
//	Arduino class for the HC_SR04 ultrasonic shield
// ==================================================

#include "HC_SR04.h"
#include <Arduino.h>

#define SPEED_OF_SOUND_US_PER_CM 29.4117647   // 1 / (340m/s * 100cm/1m * 0.000001s/us)
#define SPEED_OF_SOUND_US_PER_IN 74.7058420   // 1 / (340m/s * 39.3701in/1m * 0.000001s/us)

#define MIN_TRIGGER_TIME 10

HC_SR04::HC_SR04(int trigPin, int echoPin)
{
	m_trigPin = trigPin;
	m_echoPin = echoPin;

	pinMode(m_trigPin, OUTPUT);
	pinMode(m_echoPin, INPUT);

	digitalWrite(m_trigPin, LOW);
	digitalWrite(m_echoPin, LOW);
}

HC_SR04::~HC_SR04()
{
}

float HC_SR04::PulseForInches()
{
	unsigned long distance;

	// trigger pulse for 10 microseconds (minimum amount)
	digitalWrite(m_trigPin, HIGH);
	delayMicroseconds(MIN_TRIGGER_TIME);

	// turn off trigger
	digitalWrite(m_trigPin, LOW);

	// listen for pulse
	distance = pulseIn(m_echoPin, HIGH);

	return (float)(distance * 0.5f / SPEED_OF_SOUND_US_PER_IN);
}

float HC_SR04::PulseForCentimeters()
{
	unsigned long distance;

	// trigger pulse for 10 microseconds (minimum amount)
	digitalWrite(m_trigPin, HIGH);
	delayMicroseconds(MIN_TRIGGER_TIME);

	// turn off trigger
	digitalWrite(m_trigPin, LOW);

	// listen for pulse
	distance = pulseIn(m_echoPin, HIGH);

	return (float)(distance * 0.5f / SPEED_OF_SOUND_US_PER_CM);
}
