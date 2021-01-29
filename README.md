# Alternative firmware for Longan Serial Can Bus Module
This is an alternative firmware for the Longan Serial Can Bus Module.
The original firmware can be found [on github](https://github.com/Longan-Labs/Serial_CAN_Bus)

## UART Settings
The UART is configured to run at 9600 baud by default, but this can be changed using a command.

## Commands
Any of the commands in the table below can be sent.
Each command must be terminated by a `\n` character.
The response of a command may consist of one or more lines, each terminated by a `\n`.
The final response line of a command is always `OK` if the command succeeded, or `ERROR: <code>` when an error occured.

The following commands are available:

---------------------------------------------------------------------
### `VERSION?`
Prints the version of the firmware.

Command:
```
VERSION?
```

Response:
```
<version>
```

| Field | Description |
|---|---|
| `version` | A number describing the firmware version |

Example:
```
VERSION?
VERSION=001
OK
```
---------------------------------------------------------------------
### `ECHO=`
Enables or disables the echo.

Command:
```
ECHO=<sate>
```

| Parameter | Description |
| `state` | `0` when echo should be disabled, `1` when echo should be enabled. |

---------------------------------------------------------------------
### `ECHO?`
Gets whether commands are echoed or not.

Command:
```
ECHO?
```

Response:
```
<state>
```

| Field | Description |
|---|---|
| `state` | Is `1` if commands are echoed, `0` if commands are not echoed. |

---------------------------------------------------------------------
### `BAUD=`
Sets the baud rate of the serial port.

Command:
```
BAUD=<baud>
```

| Parameter | Description |
|---|---|
| `baud` | The desired baud rate. Possible values are: `9600`, `115200`. |

---------------------------------------------------------------------
### `BAUD?`
Gets the current baud rate of the serial port.

Command:
```
BAUD?
```

Response:
```
<baud>
```

| Field | Description |
|---|---|
| `baud` | The current baud rate of the serial port. |

---------------------------------------------------------------------
### `SPEED=`
Sets the speed of the CAN bus.

Command:
```
SPEED=<speed>
```

| Parameter | Description |
|---|---|
| `speed` | The speed of the CAN bus in kilobits per second. Possible values are: `5`, `10`, `20`, `25`, `31`, `33`, `40`, `50`, `80`, `83`, `95`, `100`, `125`, `200`, `250`, `500`, `666`, `1000` |

---------------------------------------------------------------------
### `SPEED?`
Gets the current speed of the CAN bus.

Command:
```
SPEED?
```

Response:
```
<speed>
```

| Field | Description |
|---|---|
| `speed` | The speed of the CAN bus. |

---------------------------------------------------------------------
### `READREG=`
Reads a register from the MCP2515.

Command:
```
READREG=<address>
```

| Parameter | Description |
|---|---|
| `address` | A hexadecimal value between `0` and `FF`. A register at this address will be read from the MCP2515. |

Response:
```
<value>
```

| Field | Description |
|---|---|
| `value` | A hexadecimal value between `0` and `FF`. This is the value returned by the MCP2515. |

Example:
```
READREG=29
f0
OK
```

---------------------------------------------------------------------
### `WRITEREG=`
Writes to a register of the MCP2515.

Command:
```
WRITEREG=<address>,<value>
```

| Parameter | Description |
|---|---|
| `address` | A hexadecimal value between `0` and `FF`. To the register at this address will the data be written. |
| `value` | A hexadecimal value between `0` and `FF`. The value that will be written to the register. |

---------------------------------------------------------------------
### `READ`
Reads the next frame in the buffer.

Command:
```
READ
```

Response:
```
<id>,<length>,<data>
```

| Field | Description |
|---|---|
| `id` | The ID to which the message was sent. |
| `length` | The number of bytes of data that were received. |
| `data` | The data that was encoded within the can message. It is encoded in hexadecimal, and contains between 0 to 8 bytes of data. |

Example:
```
READ
120,3,414243
OK
```

---------------------------------------------------------------------
### `READ?`
Gets the number of frames currently in the buffer.

Command:
```
READ?
```

Response:
```
<count>
```

| Field | Description |
|---|---|
| `count` | The number of packets currently stored in a buffer. |

---------------------------------------------------------------------
### `WRITE=`
Sends a CAN message.

Command:
```
WRITE=<id>,<length>,<data>
```

| Parameter | Description |
|---|---|
| `id` | The ID of the CAN message. |
| `length` | The number of bytes to send. |
| `data` | The data to send. This should be encoded in hexadecimal. |

Example:
```
WRITE=121,2,410a
OK
```

---------------------------------------------------------------------

## Errors
The following error codes can be returned by the firmware:
| Error Code | Description |
|---|---|
| 1 | An unknown command was executed. |
| 2 | A parameter was given an invalid value. |
| 3 | The baud rate specified is invalid or not supported. |
| 4 | The CAN bus speed specified is invalid or not supported. |
| 5 | The register address to read from is invalid. |
| 6 | The value to write to the register is invalid. |
| 7 | No CAN frames available for reading. |
