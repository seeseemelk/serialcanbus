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
	g_state.can = &g_can;
	pinMode(LED_AUX, OUTPUT);
	Serial.begin(g_state.baudrate);

	while (CAN_OK != g_can.begin(CAN_500KBPS))
	{
		atError("Could not initialize CAN BUS shield");
		delay(100);
	}

	while (!Serial) {}

    atSend("READY");
	atSetCallback(g_state.at, g_callbacks, &g_state);
	ledOn();
}

void loop()
{
	atReadCommand(g_state.at);
	//if (commandBufferLength > 0 && commandBuffer)
	//{

	//}
    //eventSerialCan();
    //setMode();
    //dataModeSend();
    //dataModeRecv();
}

// END FILE
