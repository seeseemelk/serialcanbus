# Alternative firmware for Longan Serial Can Bus Module
This is an alternative firmware for the Longan Serial Can Bus Module.
The original firmware can be found [on github](https://github.com/Longan-Labs/Serial_CAN_Bus)

## UART Settings
The UART is configured to run at 9600 baud by default, but this can be changed using a command.

## Commands
Any of the commands in the table below can be sent.
Each command must be terminated by a `\n` character.
The response of a command may consist of one or more lines, each terminated by a `\n`.
The final response line of a command is always `OK` if the command succeeded, or `ERROR: <a description>` when an error occured.

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
VERSION=<version>
```

| Field | Description |
|---|---|
| `version` | A string describing the firmware version |

Example:
```
VERSION?
VERSION=0.1.0
OK
```
---------------------------------------------------------------------
### `ECHO=`
Enables or disables the echo.

Command:
```
ECHO=<state>
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
ECHO=<state>
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
BAUD=<baud>
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
SPEED=<speed>
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
READREG=<value>
```

| Field | Description |
|---|---|
| `value` | A hexadecimal value between `0` and `FF`. This is the value returned by the MCP2515. |

Example:
```
READREG=29
REG=f0
OK
```

---------------------------------------------------------------------
