#ifndef STATE_H
#define STATE_H

#include "at.h"
#include "mcp_can_dfs.h"
#include "mcp_can.h"

struct State
{
	AtState at;
	MCP_CAN* can;

	int baudrate = 9600;
	int canSpeed = CAN_500KBPS;
};

#endif
