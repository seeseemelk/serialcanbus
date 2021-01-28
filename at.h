#ifndef AT_H
#define AT_H

#include <stdlib.h>

#define AT_READBUFFER_SIZE 64

typedef void(AtCallback)(void* arg, const char* arguments);

struct AtCommand
{
	const char* name;
	AtCallback* callback;
};

struct AtState
{
	char readBuffer[AT_READBUFFER_SIZE];
	size_t readBufferLength = 0;
	bool echo = true;

	AtCommand* callbacks;
	void* callbackArg;
};

void atError(const char* line);
void atSend(const char* line);
void atOk(void);

void atSetCallback(AtState& state, AtCommand* callbacks, void* arg);

void atReadCommand(AtState& state);

#endif
