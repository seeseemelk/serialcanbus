#ifndef AT_H
#define AT_H

#include "config.h"

#include <stdlib.h>

#define AT_READBUFFER_SIZE 48
#define OUTPUT_SIZE 128

typedef void(AtCallback)(void* arg, const char* arguments);

extern char g_output[OUTPUT_SIZE];

struct AtCommand
{
	const char* name;
	AtCallback* callback;
};

struct AtState
{
	char readBuffer[AT_READBUFFER_SIZE];
	size_t readBufferLength = 0;
	bool echo = UART_DO_ECHO;

	AtCommand* callbacks;
	void* callbackArg;
};

void atError(int error);
void atSend(const char* line);
void atOk(void);
void atUrc(const char* line);

void atSetCallback(AtState& state, AtCommand* callbacks, void* arg);

void atReadCommand(AtState& state);

#endif
