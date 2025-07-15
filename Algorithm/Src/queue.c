
#include "queue.h"


/**
 * @brief ��ʼ��24λ����
 * @param q ����ָ��
 * @param buffer �ⲿ�ṩ�Ļ�����
 * @param buf_size ��������С���ֽ�������������3�ı���
 * @return ��ʼ���Ƿ�ɹ�
 */
bool Queue24_Init(Queue24 *q, uint8_t *buffer, uint16_t buf_size) {
    if (buf_size % 3 != 0) return false; // ��������С������3�ı���
    
    q->data = buffer;
    q->capacity = buf_size / 3;  // ÿ��Ԫ��ռ3�ֽ�
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    return true;
}

/**
 * @brief ��24λ���ݼ������
 * @param q ����ָ��
 * @param value 24λ���ݣ�����24λ��Ч��
 * @return �Ƿ���ӳɹ�
 */
bool Queue24_Enqueue(Queue24 *q, uint32_t value) {
    if (q->count >= q->capacity) return false; // ��������
    
    uint16_t total_bytes = q->capacity * 3;
    uint16_t pos = q->tail;
    
    // �洢���ݣ�С�˸�ʽ�����ֽ���ǰ��
    q->data[pos] = (uint8_t)(value & 0xFF);         // ���8λ
    q->data[(pos + 1) % total_bytes] = (uint8_t)((value >> 8) & 0xFF);  // �м�8λ
    q->data[(pos + 2) % total_bytes] = (uint8_t)((value >> 16) & 0xFF); // ���8λ
    
    // ����βָ�루ÿ���ƶ�3�ֽڣ�
    q->tail = (pos + 3) % total_bytes;
    q->count++;
    return true;
}

/**
 * @brief �Ӷ���ȡ��24λ����
 * @param q ����ָ��
 * @param value �洢ȡ�����ݵ�ָ��
 * @return �Ƿ���ӳɹ�
 */
bool Queue24_Dequeue(Queue24 *q, uint32_t *value) {
    if (q->count == 0) return false; // ����Ϊ��
    
    uint16_t total_bytes = q->capacity * 3;
    uint16_t pos = q->head;
    
    // ��ȡ���ݣ�С�˸�ʽ��
    uint32_t low = q->data[pos];
    uint32_t mid = q->data[(pos + 1) % total_bytes];
    uint32_t high = q->data[(pos + 2) % total_bytes];
    
    *value = low | (mid << 8) | (high << 16);
    
    // ����ͷָ�루ÿ���ƶ�3�ֽڣ�
    q->head = (pos + 3) % total_bytes;
    q->count--;
    return true;
}

/**
 * @brief �������Ƿ�Ϊ��
 * @param q ����ָ��
 * @return �Ƿ�Ϊ��
 */
bool Queue24_IsEmpty(Queue24 *q) {
    return q->count == 0;
}

/**
 * @brief �������Ƿ�����
 * @param q ����ָ��
 * @return �Ƿ�����
 */
bool Queue24_IsFull(Queue24 *q) {
    return q->count >= q->capacity;
}

/**
 * @brief ��ȡ���е�ǰԪ������
 * @param q ����ָ��
 * @return Ԫ������
 */
uint16_t Queue24_Size(Queue24 *q) {
    return q->count;
}

/**
 * @brief ��ն���
 * @param q ����ָ��
 */
void Queue24_Clear(Queue24 *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}