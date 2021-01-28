#include "at.h"
#include "board.h"
#include "errors.h"

#include <Arduino.h>

/// Ugly, will move this into AtState later.
static bool g_urc = true;

char g_output[OUTPUT_SIZE];

static bool isCorrectCommand(const char* command, const char* input)
{
	size_t i = 0;
	for (;;)
	{
		if (command[i] == '\0' && input[i] == '\0')
			return true;
		else if (command[i] == '=' && input[i] == '=')
			return true;
		else if (command[i] != input[i])
			return false;
		i++;
	}
}

static void atExecute(AtState& state)
{
	AtCommand* command = state.callbacks;
	while (command->name != NULL && command->callback != NULL)
	{
		if (isCorrectCommand(command->name, state.readBuffer))
		{
			command->callback(state.callbackArg, state.readBuffer);
			return;
		}
		command++;
	}
	atError(ERR_UNKNOWN_COMMAND);
}

void atError(int code)
{
	Serial.print("ERROR: ");
	Serial.print(code);
	Serial.print("\n");
}

void atSend(const char* message)
{
	Serial.print(message);
	Serial.print("\n");
}

void atOk()
{
	Serial.print("OK\n");
}

void atUrc(const char* line)
{
	if (g_urc)
	{
		Serial.write("+");
		Serial.write(line);
		Serial.write("\n");
	}
}

void atSetCallback(AtState& state, AtCommand* callbacks, void* arg)
{
	state.callbacks = callbacks;
	state.callbackArg = arg;
}

void atReadCommand(AtState& state)
{
	int data = Serial.read();
	if (data == -1 || data == '\r')
		return;

	ledOff();
	if (state.echo)
	{
		Serial.write(data);
	}
	ledOn();

	if (data == '\n')
	{
		if (state.readBufferLength == 0)
			return;
		if (state.readBufferLength < AT_READBUFFER_SIZE)
		{
			state.readBuffer[state.readBufferLength] = '\0';
			atExecute(state);
		}
		else
		{
			atError("Command too long");
		}
		state.readBufferLength = 0;
		return;
	}

	if (state.readBufferLength < AT_READBUFFER_SIZE)
	{
		state.readBuffer[state.readBufferLength] = data;
		state.readBufferLength++;
	}
}
