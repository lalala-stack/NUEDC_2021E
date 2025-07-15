#ifndef AD_DA_H         // ���ͷ�ļ�����
#define AD_DA_H

#include "main.h"
#include "stdio.h"

#define PREAMBLE_NOT_FOUND 0xFFFF  // δ�ҵ���ͷ�ķ���ֵ

#define WAITING_FOR_SYNC 0
#define RECEIVING_DATA   1

extern uint8_t state ;  // ��ʼ״̬
extern uint8_t sync_shift_reg ;        // ͬ��ͷ��λ�Ĵ�����8λ��
extern uint16_t data_buffer ;          // Ŀ�����ݻ�������16λ��
extern uint8_t bit_counter ;  


static const uint8_t NIBBLE_MAP[16] = {
// ԭʼֵ: 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
            0, 1, 2, 3, 4, 5, 6, 8, 9,10,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

uint16_t decode_nrz(uint8_t samples[], uint8_t* decoded_data, uint32_t sample_rate, uint16_t length, float threshold, uint16_t bit_duration, uint8_t is_nrzi);
uint16_t decode_rz(uint16_t samples[], uint8_t* decoded_data, uint32_t sample_rate, uint16_t length, uint16_t threshold_vol, float threshold_duty_ratio, uint16_t bit_duration);
uint16_t detect_preamble(const uint8_t* decoded_data, uint16_t length,const uint8_t* preamble, uint16_t preamble_bits,uint16_t min_matches);
void process_bit(uint8_t current_bit);

extern uint8_t mapped[4];

#endif
