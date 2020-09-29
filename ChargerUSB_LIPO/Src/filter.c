#include "filter.h"
/*------------------------------------------------------------
 * Фильтр плавающего среднего
 * накапливает LEN_FILTER значений и вычисляет среднее арифметическое
 * при полном заполнении массива фильтра, происходит сдвиг массива
 */
uint32_t GetFilterNum(tFilter* filter, uint32_t newvalue)
{
	uint32_t res=0;

	//проверяем накопления значений в фильтре
	if(filter->numsValue < 	LEN_FILTER)
	{
	// при незаполненом массиве, дополняем новое значение
		filter->values[filter->numsValue] = newvalue;
		filter->numsValue++;
	}
	else
	{
	// если массив заполнен, производим сдвиг значений
		for(uint16_t i=0; i < (filter->numsValue-1); i++)
		{
			filter->values[i]=filter->values[i+1];
		}
		filter->values[LEN_FILTER-1] = newvalue;
	}
	
	// вычисление среднего арифметического
	for(uint16_t i=0; i<filter->numsValue; i++)
	{
		res += filter->values[i]/filter->numsValue;
	}
	filter->value=res;
	return res;
}

/*------------------------------------------------------------
 * Фильтр плавающего среднего
 * накапливает LEN_FILTER значений и вычисляет среднее арифметическое
 * при полном заполнении массива фильтра, происходит сдвиг массива
 * при выходе входящего значения за допустимые пределы delta, происходит 
 * сброс фильтра
 */
uint32_t GetFilterNumDelta(tFilter* filter, uint32_t newvalue, uint32_t delta)
{
	// при выходе нового значения за пределы допустимых, происходит сброс фильтра
	if((newvalue > filter->value+delta) || (newvalue < filter->value-delta))
	{
		filter->numsValue = 0;
	}
	
	return GetFilterNum(filter, newvalue);
}


