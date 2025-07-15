#include "AD_DA.h"

uint8_t mapped[4];

// ȫ�ֱ���
uint8_t state = WAITING_FOR_SYNC;  // ��ʼ״̬
uint8_t sync_shift_reg = 0;        // ͬ��ͷ��λ�Ĵ�����8λ��
uint16_t data_buffer = 0;          // Ŀ�����ݻ�������16λ��
uint8_t bit_counter = 0;           // Ŀ������λ������



/**
 * @brief ����ǹ����루֧��NRZ��NRZI���룩
 * @param samples[]: �����������飨ADC����ֵ����Χ0-4095��Ӧ��ѹ0-3.3V��
 * @param decoded_data: �洢������
 * @param sample_rate: ������ (Hz)�����ڼ�����ر߽�
 * @param length: �������鳤��
 * @param threshold: ��ƽ�ж���ֵ��ͨ��ΪVcc/2��˫����ʱ�迼��������ֵ��
 * @param is_nrzi: ��������ѡ��0=NRZ, 1=NRZI��
 * @retval �����Ķ��������ݳ���
 */
uint16_t decode_nrz(uint8_t samples[], uint8_t* decoded_data, uint32_t sample_rate, uint16_t length, float threshold, uint16_t bit_duration, uint8_t is_nrzi){
    uint16_t bit_count = 0;            // ����λ����
    uint8_t last_level = samples[0] > threshold ? 1 : 0;  // ��ʼ��ƽ
	uint32_t samples_per_bit = (sample_rate * bit_duration) / 1000000;

    for (uint16_t i = 1; i < length; i += samples_per_bit){
		uint16_t high_count = 0;
		uint16_t current_level = 0;
		// ��������ڸߵ�ƽ����
        for(uint16_t j = i; j < i + samples_per_bit; j++){
            if(samples[j] > threshold){
                high_count++;
            }
        }
		// �ж�ռ�ձ��Ƿ����0.5��ȥ����
		if(high_count/samples_per_bit > 0.5){
			current_level = 1;
		}
		else{
			current_level = 0;
		}
        
        // NRZI���룺��ƽ�����ʾ"1"�������ʾ"0"
        if (is_nrzi){
            if (current_level != last_level){
                decoded_data[bit_count++] = 1;  // �������1
            }
			else{
                decoded_data[bit_count++] = 0;  // �������0
            }
        } 
        // NRZ���룺ֱ���жϵ�ƽ
        else{
            decoded_data[bit_count++] = current_level;
        }
        last_level = current_level;
    }
    return bit_count;  // ���ؽ���������λ��
}

/**
 * @brief ��������루֧�ֵ�����/˫���ԣ�
 * @param samples[]: �����������飨ADC����ֵ��
 * @param decoded_data: �洢������
 * @param sample_rate: ������ (Hz)
 * @param length: �������鳤��
 * @param threshold_vol: ��ƽ�ж���ֵ��ͨ��ΪVcc/2��2048��
 * @param threshold_duty_ratio: ռ�ձ��ж���ֵ�������϶�Ϊ1��δ����Ϊ0��ͨ��Ϊ0.5��
 * @param bit_duration: �������صĳ���ʱ�䣨΢�룩
 * @retval �����Ķ��������ݳ���
 */
uint16_t decode_rz(uint16_t samples[], uint8_t* decoded_data, uint32_t sample_rate, uint16_t length, uint16_t threshold_vol, float threshold_duty_ratio, uint16_t bit_duration){
    uint16_t bit_count = 0;
    uint32_t samples_per_bit = (sample_rate * bit_duration) / 1000000;

    for(uint16_t i = 0; i < length; i += samples_per_bit){
        uint16_t high_count = 0;
        // ��������ڸߵ�ƽ����
        for(uint16_t j = i; j < i + samples_per_bit; j++){
            if(samples[j] > threshold_vol){
                high_count++;
            }
        }
		// �ж�ռ�ձ��Ƿ������ֵ
		if((float)high_count/samples_per_bit > threshold_duty_ratio){
			decoded_data[bit_count] = 1;
		}
		else{
			decoded_data[bit_count] = 0;
		}
		bit_count++;
    }
    return bit_count;
}

/**
 * @brief �ڽ��������в���ָ����ͷ
 * @param decoded_data: �����Ķ�������������(0/1ֵ)
 * @param length: �����������鳤��
 * @param preamble: Ŀ���ͷ��������
 * @param preamble_bits: ��ͷ���еı��س���
 * @param min_matches: ��Сƥ�������(0��ʾ��Ҫ��ȫƥ��)
 * @retval ƥ��λ�õ���ʼ����(-1��ʾδ�ҵ�)
 */
uint16_t detect_preamble(const uint8_t* decoded_data, uint16_t length,const uint8_t* preamble, uint16_t preamble_bits,uint16_t min_matches){
    // �������
    if (!decoded_data || !preamble || length == 0 || preamble_bits == 0) 
        return PREAMBLE_NOT_FOUND;
    
    // ������Сƥ����ֵ����ȫƥ��ʱ��Ϊpreamble_bits��
    if (min_matches == 0 || min_matches > preamble_bits) 
        min_matches = preamble_bits;

    // �����õ��ݴ���ƣ�����Ĵ����������
    const uint16_t max_errors = preamble_bits - min_matches;
    
    // �������п��ܵ���ʼλ��
    for (uint16_t i = 0; i <= length - preamble_bits; i++){
        uint16_t match_count = 0;
        uint16_t errors = 0;
        
        // ��鵱ǰ��ʼλ���Ƿ�ƥ��
        for (uint16_t j = 0; j < preamble_bits; j++){
            if (decoded_data[i + j] == preamble[j]){
                match_count++;
            } else {
                errors++;
                // ��ǰ��ֹ�������󳬹��������ֵ
                if (errors > max_errors) break;
            }
        }
        
        // ����Ƿ�ﵽ��Сƥ��Ҫ��
        if (match_count >= min_matches) {
            return i;  // ����ƥ�����ʼλ��
        }
    }
    
    return PREAMBLE_NOT_FOUND;  // δ�ҵ����������İ�ͷ
}

// ����ÿλ���ݵĺ������ڽ����жϻ���ѭ���е��ã�
void process_bit(uint8_t current_bit) {
    switch (state) {
        // ״̬1�����֡ͷ 00111100 (0x3C)
        case WAITING_FOR_SYNC:
            sync_shift_reg = (sync_shift_reg << 1) | current_bit; // ������λ
            if (sync_shift_reg == 0x3C) { // ƥ��0x3C (00111100)
                state = RECEIVING_DATA;
                bit_counter = 16;   // ׼������16λ����
                data_buffer = 0;    // ������ݻ�����
            }
            break;

        // ״̬2������Ŀ������
        case RECEIVING_DATA:
            data_buffer = (data_buffer << 1) | current_bit; // ������λ
            bit_counter--;
            
            if (bit_counter == 0) { // �ѽ���16λ
                // Ŀ�������Ѿ�����data_buffer
				// ��ֲ�ӳ��4��4λ��
                uint8_t nibbles[4] = {
                    (data_buffer >> 12) & 0x0F, // ���4λ
                    (data_buffer >> 8)  & 0x0F,
                    (data_buffer >> 4)  & 0x0F,
                    data_buffer        & 0x0F  // ���4λ
                };
                
                uint8_t valid = 1;
                
                // Ӧ��ӳ�����
                for (uint8_t i = 0; i < 4; i++) {
                    mapped[i] = NIBBLE_MAP[nibbles[i]];
                    if (mapped[i] == 0xFF) valid = 0;
                }
                

                // ����״̬�����������һ֡
                state = WAITING_FOR_SYNC;
                sync_shift_reg = 0;
            }
            break;
    }
}


