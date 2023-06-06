#include "mbed.h"
#include "OLED.h"
#include "OLED_Font.h"
#include <cstdint>

#define UPPER 0
#define LOWER 1

//上电时间
void PowerDelay()
{
	uint32_t i, j;
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
}

void OLED::SwitchDisplay(bool sta)			//切换显示的开或关
{
	if (sta == ON)
	{
		SendCommand(Display_On);
	}
	else
	{
		SendCommand(Display_Off);
	}
}
void OLED::SetClockFrequency()		//设置显示时钟分频比/振荡器频率
{
	SendCommand(Set_Clock_Frequency_to);
	SendCommand(Default_Clock_Frequency);				
}
void OLED::SetMultiplexRatio()		//设置复用率
{
	SendCommand(Set_Multiplex_Ratio_to);
	SendCommand(Default_Multiplex_Ratio);
}
void OLED::SetDisplayOffset()		//设置显示偏移
{
	SendCommand(Set_Display_Offset_to);
	SendCommand(No_Display_Offset);
}
void OLED::SetDisplayStartLine()		//设置显示开始行
{
	SendCommand(Set_Display_Start_Line_to);
	SendCommand(Line_0);
}
void OLED::SetDisplayMode()			//设置显示模式
{
	SendCommand(Normal_Display_Horizontal);	//设置左右方向，0xA1正常 0xA0左右反置
	SendCommand(Normal_Display_Vertical);	//设置上下方向，0xC8正常 0xC0上下反置
}
void OLED::SetCOMPinConfiguration()	//设置COM引脚硬件配置
{
	SendCommand(Set_COM_Pins_Hardware_Configuration);
	SendCommand(Alternative_COM_Pins_Configuration);
}
void OLED::SetContrastLevel()		//设置对比度
{
	SendCommand(Set_ContrastLevel_to);
	SendCommand(Default_ContrastLevel);
}
void OLED::SetPrechargePeriod()		//设置预充电周期
{
	SendCommand(Set_Precharge_Period_to);
	SendCommand(Default_Precharge_Period);
}
void OLED::SetVCOMHLevel()			//设置VCOMH电压
{
	SendCommand(Set_VCOMH_Level_to);
	SendCommand(Default_VCOMH_Level);
}
void OLED::SetDisplay()				//设置显示
{
	SendCommand(Entire_Display_On);
	SendCommand(Normal_Display);
}
void OLED::OpenChargePump()			//设置并开启充电泵
{
	SendCommand(Charge_Pump_Setting);
	SendCommand(Charge_Pump_Enable);
}

OLED::OLED(PinName sda, PinName scl) : i2c(sda, scl)
{


	PowerDelay();						//上电延时
	SwitchDisplay(OFF);					//关闭显示
	SetClockFrequency();				//设置显示时钟分频比/振荡器频率
	SetMultiplexRatio();				//设置复用率
	SetDisplayOffset();					//设置显示偏移
	SetDisplayStartLine();				//设置显示开始行
	SetDisplayMode();					//设置显示模式
	SetCOMPinConfiguration();			//设置COM引脚硬件配置
	SetContrastLevel();					//设置对比度
	SetPrechargePeriod();				//设置预充电周期
	SetVCOMHLevel();					//设置VCOMH电压
	SetDisplay();						//设置显示
	OpenChargePump();					//设置并开启充电泵
	SwitchDisplay(ON);					//打开显示

	Clear();				//OLED清屏
}
void OLED::SendCommand(uint8_t com)
{
	char command[2] = { 0x00, com };
	i2c.write(Oled_address, command, 2);
}
void OLED::SendData(uint8_t Data)
{
	char data[2] = { 0x40, Data };
	i2c.write(Oled_address, data, 2);
}
void OLED::SetCoordinate(uint8_t Y, uint8_t X)
{
	SendCommand(0xB0 | Y);						//设置Y位置
	SendCommand(0x10 | ((X & 0xF0) >> 4));		//设置X位置高4位
	SendCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}
uint32_t GetNumPower(uint32_t X, uint32_t Y)
{
	//计算X的Y次方
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}
uint8_t GetNumLength(uint32_t Num)
{
	//计算Num的位数
	uint8_t Length = 0;
	while (Num)
	{
		Num /= 10;
		Length++;
	}
    if(Length == 0)
    {
        Length = 1;
    }
	return Length;
}
uint8_t GetNumPart(uint32_t Num, uint8_t Part)
{
	//获取10进制Num的第Part位
	return (Num / GetNumPower(10, Part)) % 10;
}
uint32_t ToFountIndex(char Char)
{
	return Char - ' ';
}
uint8_t GetUpperPart(char Char, uint8_t Line)
{
	return FontMatrix[ToFountIndex(Char)][UPPER][Line];
}
uint8_t GetLowerPart(char Char, uint8_t Line)
{
	return FontMatrix[ToFountIndex(Char)][LOWER][Line];
}
void OLED::SelectUpperPosition(uint8_t Line, uint8_t Column)
{
	SetCoordinate((Line - 1) * 2, (Column - 1) * 8);
}
void OLED::SelectLowerPosition(uint8_t Line, uint8_t Column)
{
	SetCoordinate((Line - 1) * 2 + 1, (Column - 1) * 8);
}
void OLED::Clear()
{
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		SetCoordinate(j, 0);
		for (i = 0; i < 128; i++)
		{
			SendData(0x00);	//print blank space in every entity on the screen grid
		}
	}
}
void OLED::ShowChar(uint8_t Line, uint8_t Column, char Char)
{
	uint8_t i;
	SelectUpperPosition(Line, Column);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		SendData(GetUpperPart(Char, i));			//显示上半部分内容
	}

	SelectLowerPosition(Line, Column);		//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		SendData(GetLowerPart(Char, i));		//显示下半部分内容
	}
}
void OLED::ShowString(uint8_t Line, uint8_t Column, const char* String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		ShowChar(Line, Column + i, String[i]);
	}
}
void OLED::ShowNumber(uint8_t Line, uint8_t Column, uint32_t Number)
{
	//得到输出数字的十进制字符长度并存放在num_length中
	uint8_t Length = GetNumLength(Number);
	for (uint8_t i = 0; i < Length; i++)
	{
		uint8_t num_ith = GetNumPart(Number, Length - i - 1);//得到输出数字的第i位
		ShowChar(Line, Column + i, num_ith + '0');//在相应位置输出字符
	}
}
void OLED::ShowSignedNumber(uint8_t Line, uint8_t Column, int32_t Number)
{
	uint32_t Number1;
	if (Number >= 0)
	{
		ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	uint8_t Length = GetNumLength(Number1);
	for (uint8_t i = 0; i < Length; i++)
	{
		uint8_t num_ith = GetNumPart(Number1, Length - i - 1);
		ShowChar(Line, Column + i + 1, num_ith + '0');
	}
}
void OLED::ShowBinary(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)
	{
		ShowChar(Line, Column + i, Number / GetNumPower(2, Length - i - 1) % 2 + '0');
	}
}

