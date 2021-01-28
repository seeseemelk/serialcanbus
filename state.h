#ifndef STATE_H
#define STATE_H

#include "at.h"
#include "can.h"
#include "config.h"

struct State
{
	AtState at;
	CanState can;

	int baudrate = UART_DEFAULT_SPEED;
};

#endif
