

#include "stm32g030xx.h"
#include "app_image.h"


uint8_t app_image_buf[] = "\
:020000040800F2\
:1020000060060020652100086D2100086F2100088E\
:1020100000000000000000000000000000000000C0\
:102020000000000000000000000000007121000816\
:102030000000000000000000732100087521000866\
:1020400077210008000000007721000877210008B0\
:102050007721000877210008772100087721000800\
:102060000000000077210008772100087721000890\
:102070007721000877210008772100080000000080\
:102080007721000800000000000000007721000810\
:1020900000000000092200087721000877210008CD\
:1020A00077210008772100087721000877210008B0\
:1020B0007721000800000000000000000000000080\
:1020C00000F002F800F03EF812A030C808382418DA\
:1020D0002D18AC4201D100F035F80FCCA246AB462A\
:1020E0000CA730CF2D1B083F3F197C427F1E544464\
:1020F0002D090426F441641EAC4201D300F022F8FD\
:102100000126334200D0FB1A3343984754465D46BC\
:10211000DFE700008002000090020000704700002E\
:102120000023002400250026103A01D378C1FBD8F3\
:10213000520700D330C100D50B6070471FB51FBDDB\
:1021400010B510BD00F02DF81146FFF7F7FF00F0B5\
:10215000F9F800F045F803B4FFF7F2FF03BC00F014\
:102160004BF800000748804707480047FEE7FEE7B6\
:10217000FEE7FEE7FEE7FEE704480549054A064B91\
:102180007047000007220008C120000860000020FE\
:102190006006002060020020600200207047704747\
:1021A0007047754600F024F8AE4605006946534670\
:1021B000C008C000854618B020B5FFF7DDFF60BC41\
:1021C000B64600260027C0C5C0C5C0C5C0C5C0C52D\
:1021D000C0C5C0C5C0C5403D490849008D467047CF\
:1021E00010B50446C046C0462046FFF7B4FF10BDF8\
:1021F000004870470000002001491820ABBEFEE7F0\
:102200002600020070477047204908680122904369\
:1022100008605F4908681022504008607047C04657\
:1022200080B582B014490868012201925304184312\
:1022300008601249124808607D208000114908603A\
:1022400011490868104308601520009000F022F83A\
:102250000098019900F034F8019A0C4908680223AB\
:10226000984308600868904308600949086810436B\
:10227000086002B080BDC0464010024028440140C2\
:102280007F3E00002C4401400C44014010440140BA\
:102290000044014081B0014668460170002140566B\
:1022A000002809D4FFE7684601781F20014001207B\
:1022B000884002490860FFE701B0704700E100E094\
:1022C00070B582B0024601A802700091002140560C\
:1022D000002816D4FFE7002401A908570322D3439E\
:1022E0001840174A805809578C07E50EFF24AC4068\
:1022F000A043009CA401E4B2AC40204319408850A4\
:102300001AE001A908780F24204008380322D3439B\
:1023100018400A4A8058002549578D07EE0EFF25C0\
:10232000B540A843009DAD01EDB2B5402843214022\
:10233000083919408850FFE702B070BD1CED00E07D\
:1023400000E400E080B582B0002001900D490E4805\
:1023500008600E4908680122104308600C490868AB\
:1023600010239843086005200107086803231B0217\
:10237000984308600868120210430860FFF750FF96\
:10238000FFE7FEE708ED00E00020000834100240FF\
:1023900014000050A4230008000000206006000084\
:0423A00020210008F0\
:04000005080020C10E\
:00000001FF\
";



typedef struct app_image_t{
	uint32_t BaseAddr;
	uint32_t CurrAddr;
	uint8_t  CurrChar;
	uint8_t  Checksum;
	uint16_t Reserved1;
	uint32_t IndexCnt;
	uint8_t  ByteCnt;
	uint8_t  Reserved2;
	uint16_t AddrOffset;
	uint8_t  RecType;
	uint8_t  Reserved3;
	uint16_t Reserved4;
	uint8_t  DataBlock[64];
	uint8_t  DataBlock8b[64];
	uint16_t DataBlock16b[32];
	uint32_t DataBlock32b[16];
	uint64_t DataBlock64b[8];
	uint16_t TotalDataBytes;
	uint16_t TotalDataHWords;
	uint16_t TotalDataWords;
	uint16_t TotalDataDWords;
}app_image_t;

app_image_t AppImageData;


void App_Image_Init_Data(void)
{
	AppImageData.BaseAddr = 0;
	AppImageData.IndexCnt = 0;
	AppImageData.ByteCnt = 0;
	AppImageData.AddrOffset = 0;
	AppImageData.RecType = 0;
	AppImageData.TotalDataBytes = 0;
	AppImageData.TotalDataHWords = 0;
	AppImageData.TotalDataWords = 0;
	AppImageData.TotalDataDWords = 0;
}


void App_Image_Skip_Until_Data(void)
{
	while(app_image_buf[AppImageData.IndexCnt] != ':')
	{
		//timeout
		AppImageData.IndexCnt++;
	}
	AppImageData.IndexCnt++;
}

uint8_t App_Image_Char_To_Num(uint8_t char_data)
{
	//process 0-9
	if( (char_data >= 48) && (char_data <= 57) )
	{
		return (char_data - 48);
	}
	
	//process A-F
	else if( (char_data >= 65) && (char_data <= 70) )
	{
		return (char_data - 55);
	}
	
	//process a-f
	else if( (char_data >= 97) && (char_data <= 102) )
	{
		return (char_data - 87);
	}
	
	//error
	else
	{
		return 0xFF;
	}
}

uint8_t App_Image_Checksum_Get(void)
{
	uint8_t tmp, data[2];
	uint16_t index;
	AppImageData.Checksum = 0;
	index = AppImageData.IndexCnt;
	while( (app_image_buf[index] != '\r') && 
		   (app_image_buf[index] != '\n') && 
	       (app_image_buf[index] != ':')  )
	{
		data[0] = app_image_buf[index++];
		data[1] = app_image_buf[index++];
		tmp   = App_Image_Char_To_Num(data[0]);
		tmp <<= 4;
		tmp  |= App_Image_Char_To_Num(data[1]);
		//only for debugging
		AppImageData.CurrChar  = tmp;
		//calculate checksum
		AppImageData.Checksum += AppImageData.CurrChar;
	}
	return AppImageData.Checksum;
}


uint8_t App_Image_Byte_Count_Get(void)
{
	uint8_t tmp, data[2];
	data[0] = app_image_buf[AppImageData.IndexCnt++];
	data[1] = app_image_buf[AppImageData.IndexCnt++];
	tmp   = App_Image_Char_To_Num(data[0]);
	tmp <<= 4;
	tmp  |= App_Image_Char_To_Num(data[1]);
	AppImageData.ByteCnt = tmp;
	return tmp;
}

uint16_t App_Image_Addr_Offset_Get(void)
{
	uint8_t  data[4];
	uint16_t offset;
	data[0] = app_image_buf[AppImageData.IndexCnt++];
	data[1] = app_image_buf[AppImageData.IndexCnt++];
	data[2] = app_image_buf[AppImageData.IndexCnt++];
	data[3] = app_image_buf[AppImageData.IndexCnt++];
	offset   = App_Image_Char_To_Num(data[0]);
	offset <<= 4;
	offset  |= App_Image_Char_To_Num(data[1]);
	offset <<= 4;
	offset  |= App_Image_Char_To_Num(data[2]);
	offset <<= 4;
	offset  |= App_Image_Char_To_Num(data[3]);
	AppImageData.AddrOffset = offset;
	return offset;
}

uint16_t App_Image_Rec_Type_Get(void)
{
	uint8_t  tmp, data[2];
	data[0] = app_image_buf[AppImageData.IndexCnt++];
	data[1] = app_image_buf[AppImageData.IndexCnt++];
	tmp   = App_Image_Char_To_Num(data[0]);
	tmp <<= 4;
	tmp  |= App_Image_Char_To_Num(data[1]);
	AppImageData.RecType = tmp;
	return tmp;
}

void App_Image_Copy_Data_Block(void)
{
	uint8_t  loop_cnt, tmp, data[2], index;
	index = 0; 
	for(loop_cnt = 0; loop_cnt < AppImageData.ByteCnt; loop_cnt++)
	{
		data[0] = app_image_buf[AppImageData.IndexCnt++];
		data[1] = app_image_buf[AppImageData.IndexCnt++];
		tmp   = App_Image_Char_To_Num(data[0]);
		tmp <<= 4;
		tmp  |= App_Image_Char_To_Num(data[1]);
		AppImageData.DataBlock[index++] = tmp;
	}
}

void App_Image_Segreget_Fields(void)
{
	uint8_t loop_cnt, tmp;
	App_Image_Skip_Until_Data();
	App_Image_Checksum_Get();
	App_Image_Byte_Count_Get();
	App_Image_Addr_Offset_Get();
	App_Image_Rec_Type_Get();
	App_Image_Copy_Data_Block();
	
	//extended linear address
	if(AppImageData.RecType == 0x04)
	{
		AppImageData.BaseAddr  = AppImageData.DataBlock[0];
		AppImageData.BaseAddr <<= 8;
		AppImageData.BaseAddr |= AppImageData.DataBlock[1];
		AppImageData.BaseAddr <<= 16;
		AppImageData.CurrAddr  = AppImageData.BaseAddr; 
	}
	
	//check data address
	else if(AppImageData.RecType == 0x00)
	{
		AppImageData.CurrAddr = AppImageData.BaseAddr + AppImageData.AddrOffset;
		
		//make 8 bit data buffer (change endianness)
		tmp = AppImageData.ByteCnt;
		for(loop_cnt = 0; loop_cnt < tmp; loop_cnt++)
		{
			if( (loop_cnt & 1) == 0)
			{
				AppImageData.DataBlock8b[loop_cnt] = AppImageData.DataBlock[loop_cnt+1];
			}
			else
			{
				AppImageData.DataBlock8b[loop_cnt] = AppImageData.DataBlock[loop_cnt-1];
			}
		}
		AppImageData.TotalDataBytes += tmp;
		
		//make 16 bit data buffer
		tmp /= 2;
		for(loop_cnt = 0; loop_cnt < tmp; loop_cnt++)
		{
			AppImageData.DataBlock16b[loop_cnt]   = AppImageData.DataBlock8b[(loop_cnt << 1) + 0];
			AppImageData.DataBlock16b[loop_cnt] <<= 8;
			AppImageData.DataBlock16b[loop_cnt]  |= AppImageData.DataBlock8b[(loop_cnt << 1) + 1];
		}
		//if 16 bit buffer is odd numbered
		if(tmp & 1)
		{
			AppImageData.DataBlock16b[tmp] = 0xFFFF;
			tmp++;
		}
		AppImageData.TotalDataHWords += tmp;
		
		//make 32 bit data buffer (reverse data order)
		tmp /= 2;
		for(loop_cnt = 0; loop_cnt < tmp; loop_cnt++)
		{
			AppImageData.DataBlock32b[loop_cnt]   = AppImageData.DataBlock16b[(loop_cnt << 1) + 1];
			AppImageData.DataBlock32b[loop_cnt] <<= 16;
			AppImageData.DataBlock32b[loop_cnt]  |= AppImageData.DataBlock16b[(loop_cnt << 1) + 0];
		}
		//if 32 bit buffer is odd numbered
		if(tmp & 1)
		{
			AppImageData.DataBlock32b[tmp] = 0xFFFFFFFF;
			tmp++;
		}
		AppImageData.TotalDataWords += tmp;
		
		//make 64 bit data buffer
		tmp /= 2;
		for(loop_cnt = 0; loop_cnt < tmp; loop_cnt++)
		{
			AppImageData.DataBlock64b[loop_cnt]   = AppImageData.DataBlock32b[(loop_cnt << 1) + 0];
			AppImageData.DataBlock64b[loop_cnt] <<= 32;
			AppImageData.DataBlock64b[loop_cnt]  |= AppImageData.DataBlock32b[(loop_cnt << 1) + 1];
		}
		//if 64 bit buffer is odd numbered
		if(tmp & 1)
		{
			AppImageData.DataBlock64b[tmp] = 0xFFFFFFFFFFFFFFFF;
			tmp++;
		}
		AppImageData.TotalDataDWords += tmp;
	}
}


uint8_t App_Image_Byte_Cnt_Get(void)
{
	return AppImageData.ByteCnt;
}

uint32_t App_Image_Current_Addr_Get(void)
{
	return AppImageData.CurrAddr;
}

uint64_t App_Image_Data_Get(uint8_t index)
{
	return AppImageData.DataBlock64b[index];
}