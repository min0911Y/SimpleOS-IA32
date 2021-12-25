#include "int.h"
#define BCD_HEX(n)	((n >> 4) * 10) + (n & 0xf) 
#define CMOS_CUR_SEC	0x0			//CMOS拞摉慜昩?(BCD)
#define CMOS_ALA_SEC	0x1			//CMOS拞?寈昩?(BCD)
#define CMOS_CUR_MIN	0x2			//CMOS拞摉慜暘?(BCD)
#define CMOS_ALA_MIN	0x3			//CMOS拞?寈暘?(BCD)
#define CMOS_CUR_HOUR	0x4			//CMOS拞摉慜彫?(BCD)
#define CMOS_ALA_HOUR	0x5			//CMOS拞?寈彫?(BCD)
#define CMOS_WEEK_DAY	0x6			//CMOS拞堦廃拞摉慜揤(BCD)
#define CMOS_MON_DAY	0x7			//CMOS拞堦寧拞摉慜擔(BCD)
#define CMOS_CUR_MON	0x8			//CMOS拞摉慜寧?(BCD)
#define CMOS_CUR_YEAR	0x9			//CMOS拞摉慜擭?(BCD)
#define CMOS_DEV_TYPE	0x12		//CMOS拞??婍奿幃
#define CMOS_CUR_CEN	0x32		//CMOS拞摉慜悽?(BCD)
#define cmos_index 0x70
#define cmos_data 0x71
unsigned char read_cmos (unsigned char p)
{
	unsigned char data;
	io_out8(cmos_index, p);
	data = io_in8(cmos_data);
	io_out8(cmos_index, 0x80);
	return data;
}
unsigned int get_hour_hex()
{
	return BCD_HEX(read_cmos(CMOS_CUR_HOUR));
}
unsigned int get_min_hex()
{
	return BCD_HEX(read_cmos(CMOS_CUR_MIN));
}
unsigned int get_sec_hex()
{
	return BCD_HEX(read_cmos(CMOS_CUR_SEC));
}
unsigned int get_day_of_month()
{
	return BCD_HEX(read_cmos(CMOS_MON_DAY));
}
unsigned int get_day_of_week()
{
	return BCD_HEX(read_cmos(CMOS_WEEK_DAY));
}
unsigned int get_mon_hex()
{
	return BCD_HEX(read_cmos(CMOS_CUR_MON));
}
unsigned int get_year()
{
	return (BCD_HEX(read_cmos(CMOS_CUR_CEN))*100)+BCD_HEX(read_cmos(CMOS_CUR_YEAR))-30 + 2010;
}
