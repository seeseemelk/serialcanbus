#include "at.h"
#include "board.h"
#include "commands.h"
#include "mcp_can.h"
#include "state.h"

#include <Arduino.h>
#include <SPI.h>

static State g_state;
static MCP_CAN g_can(9);

void setup()
{
	g_state.can.dev = &g_can;
	Serial.begin(g_state.baudrate);

	while (CAN_OK != g_can.begin(g_state.can.speed))
	{
		atError("Could not initialize CAN BUS shield");
		delay(100);
	}

    g_can.init_Mask(0, 0, 0);
	g_can.init_Mask(1, 0, 0);

    g_can.init_Filt(0, 0, 0);
    g_can.init_Filt(1, 0, 0);
    g_can.init_Filt(2, 0, 0);
    g_can.init_Filt(3, 0, 0);
    g_can.init_Filt(4, 0, 0);
    g_can.init_Filt(5, 0, 0);

	while (!Serial) {}

    atSend("READY");
	atSetCallback(g_state.at, g_callbacks, &g_state);

	pinMode(LED_AUX, OUTPUT);
	ledOn();
}

void loop()
{
	atReadCommand(g_state.at);
	canReadMessages(g_state.can);
}
