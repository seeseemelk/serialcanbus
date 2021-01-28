#include "at.h"

#include <Arduino.h>

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
	atError("UNKNOWN COMMAND");
}

void atError(const char* message)
{
	Serial.print("ERROR: ");
	Serial.print(message);
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

void atSetCallback(AtState& state, AtCommand* callbacks, void* arg)
{
	state.callbacks = callbacks;
	state.callbackArg = arg;
}

void atReadCommand(AtState& state)
{
	int data = Serial.read();
	if (data == -1)
		return;

	if (state.echo)
	{
		Serial.write(data);
	}

	if (data == '\n')
	{
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
