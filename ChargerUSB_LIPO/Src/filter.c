#include "filter.h"
/*------------------------------------------------------------
 * ������ ���������� ��������
 * ����������� LEN_FILTER �������� � ��������� ������� ��������������
 * ��� ������ ���������� ������� �������, ���������� ����� �������
 */
uint32_t GetFilterNum(tFilter* filter, uint32_t newvalue)
{
	uint32_t res=0;

	//��������� ���������� �������� � �������
	if(filter->numsValue < 	LEN_FILTER)
	{
	// ��� ������������ �������, ��������� ����� ��������
		filter->values[filter->numsValue] = newvalue;
		filter->numsValue++;
	}
	else
	{
	// ���� ������ ��������, ���������� ����� ��������
		for(uint16_t i=0; i < (filter->numsValue-1); i++)
		{
			filter->values[i]=filter->values[i+1];
		}
		filter->values[LEN_FILTER-1] = newvalue;
	}
	
	// ���������� �������� ���������������
	for(uint16_t i=0; i<filter->numsValue; i++)
	{
		res += filter->values[i]/filter->numsValue;
	}
	filter->value=res;
	return res;
}

/*------------------------------------------------------------
 * ������ ���������� ��������
 * ����������� LEN_FILTER �������� � ��������� ������� ��������������
 * ��� ������ ���������� ������� �������, ���������� ����� �������
 * ��� ������ ��������� �������� �� ���������� ������� delta, ���������� 
 * ����� �������
 */
uint32_t GetFilterNumDelta(tFilter* filter, uint32_t newvalue, uint32_t delta)
{
	// ��� ������ ������ �������� �� ������� ����������, ���������� ����� �������
	if((newvalue > filter->value+delta) || (newvalue < filter->value-delta))
	{
		filter->numsValue = 0;
	}
	
	return GetFilterNum(filter, newvalue);
}


