#ifndef COMMAND_QUEUE_H_
#define COMMAND_QUEUE_H_

#include <Arduino.h>

#define SEND_QUEUE_SIZE (128)
#define RECV_QUEUE_SIZE (256)

extern xQueueHandle send_queue;
extern xQueueHandle recv_queue;

#endif