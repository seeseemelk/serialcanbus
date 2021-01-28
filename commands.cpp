#include "commands.h"
#include "state.h"
#include "at.h"

#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

static const int canSpeeds[] = {
	0, 5, 10, 20, 25, 31, 33, 40,
	50, 80, 83, 95, 100, 125, 200,
	250, 500, 666, 1000
};

static void cmdVersionGet(State* state, const char* command)
{
	atSend("VERSION=0.1.0");
	atOk();
}

static void cmdEcho(State* state, const char* command)
{
	char c = 0;
	sscanf(command, "ECHO=%c", &c);
	switch (c)
	{
	case '0':
		state->at.echo = false;
		atOk();
		break;
	case '1':
		state->at.echo = true;
		atOk();
		break;
	default:
		atError("Invalid parameter");
		break;
	}
}

static void cmdEchoGet(State* state, const char* command)
{
	if (state->at.echo == true)
		atSend("ECHO=1");
	else
		atSend("ECHO=0");
	atOk();
}

static void cmdBaud(State* state, const char* command)
{
	int baudrate = 0;
	sscanf(command, "BAUD=%d", &baudrate);

	switch (baudrate)
	{
	case 9600:
	case 115200:
		state->baudrate = baudrate;
		atOk();
		Serial.begin(baudrate);
		break;
	default:
		atError("Invalid baud rate");
		break;
	}
}

static void cmdBaudGet(State* state, const char* command)
{
	char buf[32];
	sprintf(buf, "BAUD=%d", state->baudrate);
	atSend(buf);
	atOk();
}

static void cmdSpeed(State* state, const char* command)
{
	int speed = 0;
	sscanf(command, "SPEED=%d", &speed);
	for (size_t i = 1; i < sizeof(canSpeeds) / sizeof(int); i++)
	{
		if (speed == canSpeeds[i])
		{
			state->baudrate = speed;
			state->can->mcp2515_configRate(speed);
			atOk();
			return;
		}
	}
	atError("Invalid speed");
	return;
}

static void cmdSpeedGet(State* state, const char* command)
{
	int speed = canSpeeds[state->canSpeed];
	char message[32];
	sprintf(message, "SPEED=%d", speed);
	atSend(message);
	atOk();
}

static void cmdReadRegister(State* state, const char* command)
{
	int address;
	sscanf(command, "READREG=%x", &address);
	byte value = state->can->mcp2515_readRegister(address);
	char message[32];
	sprintf(message, "READREG=%x", value);
	atSend(message);
	atOk();
}

#define COMMAND(NAME, CALLBACK) {.name=NAME, .callback=(AtCallback*) &CALLBACK}
AtCommand g_callbacks[] =
{
	COMMAND("VERSION?", cmdVersionGet),
	COMMAND("ECHO=", cmdEcho),
	COMMAND("ECHO?", cmdEchoGet),
	COMMAND("BAUD=", cmdBaud),
	COMMAND("BAUD?", cmdBaudGet),
	COMMAND("SPEED=", cmdSpeed),
	COMMAND("SPEED?", cmdSpeedGet),
	COMMAND("READREG=", cmdReadRegister),
	{NULL, NULL}
};
