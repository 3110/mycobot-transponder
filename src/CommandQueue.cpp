#include "CommandQueue.h"

xQueueHandle send_queue = xQueueCreate(SEND_QUEUE_SIZE, sizeof(uint8_t));
xQueueHandle recv_queue = xQueueCreate(RECV_QUEUE_SIZE, sizeof(uint8_t));
