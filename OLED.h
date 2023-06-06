#pragma once
#include <cstdint>
#include "mbed.h"
#include "OLED_Font.h"
#define ON 1
#define OFF 0

//0.96寸OLED显示屏，四针I2C通信，分辨率128*64
class OLED
{
	enum Adress{
		Oled_address = 0x78,		//OLED地址
	};
	enum Header
	{
		Select_Row = 0xB0,				//选择行的命令头
		Select_Column_upper = 0x10,		//选择列数字高四位的命令头
		Select_Column_lower = 0x00,		//选择列数字低四位的命令头
	};
	enum Cmd {
		Display_Off = 0xAE,						//关显示
		Display_On = 0xAF,						//开显示
		
		Set_Clock_Frequency_to = 0xD5,	//设置显示时钟分频比/振荡器频率
		Default_Clock_Frequency = 0x80,	//默认显示时钟分频比/振荡器频率

		Set_Multiplex_Ratio_to = 0xA8,			//设置复用率
		Default_Multiplex_Ratio = 0x3F,			//默认复用率

		Set_Display_Offset_to = 0xD3,			//设置显示偏移
		No_Display_Offset = 0x00,				//默认显示偏移为0

		Set_Display_Start_Line_to = 0x40,		//设置显示开始行
		Line_0 = 0x00,		//默认显示开始行为第0行

		Normal_Display_Horizontal = 0xA1,		//横向正常显示
		Inverse_Display_Horizontal = 0xA0,		//横向反转显示
		Normal_Display_Vertical = 0xC8,			//纵向正常显示
		Inverse_Display_Vertical = 0xC0,		//纵向反转显示

		Set_COM_Pins_Hardware_Configuration = 0xDA,		//设置COM引脚硬件配置
		Alternative_COM_Pins_Configuration = 0x12,		//使用备选引脚配置，并禁用左右反置

		Set_ContrastLevel_to = 0x81,				//设置对比度
		Default_ContrastLevel = 0xCF,				//默认对比度
		Set_Precharge_Period_to = 0xD9,				//设置预充电周期
		Default_Precharge_Period = 0xF1,			//默认预充电周期

		Set_VCOMH_Level_to = 0xDB,					//设置VCOMH电压
		Default_VCOMH_Level = 0x30,					//默认VCOMH电压0.83 x VCC
		//设置整个显示打开/关闭 //正常显示模式，显示GDDRAM中的数据
		Entire_Display_On = 0xA4,					//正常显示模式，显示GDDRAM中的数据
		Entire_Display_Off = 0xA5,					//全屏显示模式，忽略GDDRAM中的数据
		//设置正常/倒转显示 //正常显示
		Normal_Display = 0xA6,						//正常显示
		Inverse_Display = 0xA7,						//倒转显示


		Charge_Pump_Setting = 0x8D,				//设置电荷泵
		Charge_Pump_Enable = 0x14,				//开启电荷泵
		Charge_Pump_Disable = 0x10,				//关闭电荷泵
	};
public:

	OLED(PinName sda, PinName scl); //构造函数,并在构造中完成初始化

	void Clear(void);
	void ShowChar(uint8_t Line, uint8_t Column, char Char);
	void ShowString(uint8_t Line, uint8_t Column, const char* String);
	void ShowNumber(uint8_t Line, uint8_t Column, uint32_t Number);
	void ShowSignedNumber(uint8_t Line, uint8_t Column, int32_t Number);
	void ShowBinary(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);


private:

	void SendCommand(uint8_t Command);	//写命令
	void SendData(uint8_t Data);			//写数据


	void SwitchDisplay(bool sta);			//切换显示的开或关
	void SetClockFrequency();		//设置显示时钟分频比/振荡器频率
	void SetMultiplexRatio();		//设置复用率
	void SetDisplayOffset();		//设置显示偏移
	void SetDisplayStartLine();		//设置显示开始行
	void SetDisplayMode();			//设置显示模式
	void SetCOMPinConfiguration();	//设置COM引脚硬件配置
	void SetContrastLevel();		//设置对比度
	void SetPrechargePeriod();		//设置预充电周期
	void SetVCOMHLevel();			//设置VCOMH电压
	void SetDisplay();				//设置显示
	void OpenChargePump();			//设置并开启充电泵










	//因为分辨率为128*64，每一个字符占8*16，所以分成16行4列字符
	void SetCoordinate(uint8_t Y, uint8_t X);	//设置基于16行4列的光标，左上角为0,0
	void SelectUpperPosition(uint8_t Line, uint8_t Column);
	void SelectLowerPosition(uint8_t Line, uint8_t Column);

private:
	I2C i2c; //使用I2C通信
};

