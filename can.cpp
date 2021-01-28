#include "can.h"
#include "mcp_can.h"

#include "at.h"

static Frame* canAlloc(CanState& state)
{
	size_t next = (state.writeIndex + 1) % FRAME_BUFFER;
	if (next == state.readIndex)
		return nullptr;

	Frame* frame = state.frames + state.writeIndex;
	state.writeIndex = next;
	return frame;
}

void canReadMessages(CanState& state)
{
	if (state.dev->checkReceive() != CAN_MSGAVAIL)
	{
		return;
	}

	unsigned long id;
	byte length;
	byte data[MAX_CHAR_IN_MESSAGE];
	byte result = state.dev->readMsgBufID(&id, &length, data);

	Frame* frame = canAlloc(state);
	if (frame == nullptr)
	{
		return;
	}

	frame->id = id;
	frame->length = length;
	memcpy(frame->data, data, (length > 8) ? 8 : length);
}

int canAvailableFrames(CanState& state)
{
	int read = state.readIndex;
	int write = state.writeIndex;

	if (write < read)
		write += FRAME_BUFFER;
	return write - read;
}

Frame* canGetFrame(CanState& state)
{
	if (canAvailableFrames(state) == 0)
		return nullptr;
	Frame* frame = state.frames + state.readIndex;
	state.readIndex = (state.readIndex + 1) % FRAME_BUFFER;
	return frame;
}
