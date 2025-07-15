#include <stdint.h>
#include <stdbool.h>
#include "main.h"


// ���нṹ��
typedef struct {
    uint8_t *data;      // ���ݴ洢������
    uint16_t capacity;  // ����������Ԫ�ظ�����
    uint16_t head;      // ����ͷָ�루�ֽ�������
    uint16_t tail;      // ����βָ�루�ֽ�������
    uint16_t count;     // ��ǰԪ������
} Queue24;

bool Queue24_Init(Queue24 *q, uint8_t *buffer, uint16_t buf_size);
bool Queue24_Enqueue(Queue24 *q, uint32_t value) ;
bool Queue24_Dequeue(Queue24 *q, uint32_t *value);
bool Queue24_IsEmpty(Queue24 *q);
bool Queue24_IsFull(Queue24 *q);
uint16_t Queue24_Size(Queue24 *q);
void Queue24_Clear(Queue24 *q);
