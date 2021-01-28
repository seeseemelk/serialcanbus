#include "at.h"
#include "commands.h"
#include "config.h"
#include "errors.h"
#include "state.h"

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
	atSend(VERSION);
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
		atError(ERR_INVALID_PARAMETER);
		break;
	}
}

static void cmdEchoGet(State* state, const char* command)
{
	if (state->at.echo == true)
		atSend("1");
	else
		atSend("0");
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
		atError(ERR_INVALID_BAUD_RATE);
		break;
	}
}

static void cmdBaudGet(State* state, const char* command)
{
	sprintf(g_output, "%d", state->baudrate);
	atSend(g_output);
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
			state->can.speed = i;
			state->can.dev->mcp2515_configRate(speed);
			atOk();
			return;
		}
	}
	atError(ERR_INVALID_SPEED);
	return;
}

static void cmdSpeedGet(State* state, const char* command)
{
	int speed = canSpeeds[state->can.speed];
	sprintf(g_output, "%d", speed);
	atSend(g_output);
	atOk();
}

static void cmdReadRegister(State* state, const char* command)
{
	int address = -1;
	sscanf(command, "READREG=%x", &address);
	if (address < 0 || address > 255)
	{
		atError(ERR_INVALID_ADDRESS);
		return;
	}

	byte value = state->can.dev->mcp2515_readRegister(address);
	sprintf(g_output, "%x", value);
	atSend(g_output);
	atOk();
}

static void cmdWriteRegister(State* state, const char* command)
{
	int address = -1;
	int value = -1;
	sscanf(command, "WRITEREG=%x,%x", &address, &value);

	if (address < 0 || address > 255)
	{
		atError(ERR_INVALID_ADDRESS);
		return;
	}

	if (value < 0 || value > 255)
	{
		atError(ERR_INVALID_VALUE);
		return;
	}

	state->can.dev->mcp2515_setRegister(address, value);
	atOk();
}

static void cmdRead(State* state, const char* command)
{
	Frame* frame = canGetFrame(state->can);
	if (frame == nullptr)
	{
		atError(ERR_NO_DATA);
		return;
	}

	sprintf(g_output, "%lu,%d,", frame->id, frame->length);
	int end = strlen(g_output);

	for (size_t i = 0; i < frame->length; i++)
	{
		sprintf(g_output + end, "%02x", frame->data[i]);
		end += 2;
	}

	atSend(g_output);
	atOk();
}

static void cmdReadGet(State* state, const char* command)
{
	sprintf(g_output, "%d", canAvailableFrames(state->can));
	atSend(g_output);
	atOk();
}

static void cmdWrite(State* state, const char* command)
{
	unsigned long id;
	int length;
	byte data[8];

	int end = 0;
	sscanf(command, "WRITE=%lu,%d,%n", &id, &length, &end);

	if (end == 0)
	{
		atError(ERR_INVALID_PARAMETER);
		return;
	}

	for (size_t i = 0; i < length; i++)
	{
		int value;
		sscanf(command + end, "%02x", &value);
		data[i] = value;
		end += 2;
	}

	state->can.dev->sendMsgBuf(id, 0, length, data);

	atOk();
}

#define COMMAND(NAME, CALLBACK) {.name=NAME, .callback=(AtCallback*) &CALLBACK}
const AtCommand g_callbacks[] =
{
	COMMAND("VERSION?", cmdVersionGet),
	COMMAND("ECHO=", cmdEcho),
	COMMAND("ECHO?", cmdEchoGet),
	COMMAND("BAUD=", cmdBaud),
	COMMAND("BAUD?", cmdBaudGet),
	COMMAND("SPEED=", cmdSpeed),
	COMMAND("SPEED?", cmdSpeedGet),
	COMMAND("READREG=", cmdReadRegister),
	COMMAND("WRITEREG=", cmdWriteRegister),
	COMMAND("READ", cmdRead),
	COMMAND("READ?", cmdReadGet),
	COMMAND("WRITE=", cmdWrite),
	{NULL, NULL}
};
