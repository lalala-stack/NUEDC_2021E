
#include "queue.h"


/**
 * @brief 初始化24位队列
 * @param q 队列指针
 * @param buffer 外部提供的缓冲区
 * @param buf_size 缓冲区大小（字节数），必须是3的倍数
 * @return 初始化是否成功
 */
bool Queue24_Init(Queue24 *q, uint8_t *buffer, uint16_t buf_size) {
    if (buf_size % 3 != 0) return false; // 缓冲区大小必须是3的倍数
    
    q->data = buffer;
    q->capacity = buf_size / 3;  // 每个元素占3字节
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    return true;
}

/**
 * @brief 将24位数据加入队列
 * @param q 队列指针
 * @param value 24位数据（仅低24位有效）
 * @return 是否入队成功
 */
bool Queue24_Enqueue(Queue24 *q, uint32_t value) {
    if (q->count >= q->capacity) return false; // 队列已满
    
    uint16_t total_bytes = q->capacity * 3;
    uint16_t pos = q->tail;
    
    // 存储数据（小端格式：低字节在前）
    q->data[pos] = (uint8_t)(value & 0xFF);         // 最低8位
    q->data[(pos + 1) % total_bytes] = (uint8_t)((value >> 8) & 0xFF);  // 中间8位
    q->data[(pos + 2) % total_bytes] = (uint8_t)((value >> 16) & 0xFF); // 最高8位
    
    // 更新尾指针（每次移动3字节）
    q->tail = (pos + 3) % total_bytes;
    q->count++;
    return true;
}

/**
 * @brief 从队列取出24位数据
 * @param q 队列指针
 * @param value 存储取出数据的指针
 * @return 是否出队成功
 */
bool Queue24_Dequeue(Queue24 *q, uint32_t *value) {
    if (q->count == 0) return false; // 队列为空
    
    uint16_t total_bytes = q->capacity * 3;
    uint16_t pos = q->head;
    
    // 读取数据（小端格式）
    uint32_t low = q->data[pos];
    uint32_t mid = q->data[(pos + 1) % total_bytes];
    uint32_t high = q->data[(pos + 2) % total_bytes];
    
    *value = low | (mid << 8) | (high << 16);
    
    // 更新头指针（每次移动3字节）
    q->head = (pos + 3) % total_bytes;
    q->count--;
    return true;
}

/**
 * @brief 检查队列是否为空
 * @param q 队列指针
 * @return 是否为空
 */
bool Queue24_IsEmpty(Queue24 *q) {
    return q->count == 0;
}

/**
 * @brief 检查队列是否已满
 * @param q 队列指针
 * @return 是否已满
 */
bool Queue24_IsFull(Queue24 *q) {
    return q->count >= q->capacity;
}

/**
 * @brief 获取队列当前元素数量
 * @param q 队列指针
 * @return 元素数量
 */
uint16_t Queue24_Size(Queue24 *q) {
    return q->count;
}

/**
 * @brief 清空队列
 * @param q 队列指针
 */
void Queue24_Clear(Queue24 *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}