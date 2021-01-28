#ifndef CAN_H
#define CAN_H

#include "config.h"
#include "mcp_can_dfs.h"
#include "mcp_can.h"

#define FRAME_BUFFER 9

struct Frame
{
	unsigned long id;
	int length;
	byte data[8];
};

struct CanState
{
	MCP_CAN* dev;
	int speed = CAN_DEFAULT_SPEED;

	int writeIndex = 0;
	int readIndex = 0;
	Frame frames[FRAME_BUFFER];
};

void canReadMessages(CanState& state);
int canAvailableFrames(CanState& state);
Frame* canGetFrame(CanState& state);

#endif
