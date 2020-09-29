#ifndef __filter_H
#define __filter_H
#ifdef __cplusplus
 extern "C" {
	 
#endif

#include <stdint.h>
	 
#define LEN_FILTER 40	 

struct stFilter
{
	uint32_t values[LEN_FILTER];			// ��������
	uint16_t numsValue;							//���������� ���������� ��������
	uint32_t value;									//������� �������� �������
};

typedef struct stFilter tFilter;

uint32_t GetFilterNum(tFilter* filter, uint32_t newvalue);
uint32_t GetFilterNumDelta(tFilter* filter, uint32_t newvalue, uint32_t delta);

	 
#ifdef __cplusplus
}
#endif
#endif /* __filter_H */
