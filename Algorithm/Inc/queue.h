#include <stdint.h>
#include <stdbool.h>
#include "main.h"


// 队列结构体
typedef struct {
    uint8_t *data;      // 数据存储缓冲区
    uint16_t capacity;  // 队列容量（元素个数）
    uint16_t head;      // 队列头指针（字节索引）
    uint16_t tail;      // 队列尾指针（字节索引）
    uint16_t count;     // 当前元素数量
} Queue24;

bool Queue24_Init(Queue24 *q, uint8_t *buffer, uint16_t buf_size);
bool Queue24_Enqueue(Queue24 *q, uint32_t value) ;
bool Queue24_Dequeue(Queue24 *q, uint32_t *value);
bool Queue24_IsEmpty(Queue24 *q);
bool Queue24_IsFull(Queue24 *q);
uint16_t Queue24_Size(Queue24 *q);
void Queue24_Clear(Queue24 *q);
