#include "Wifi.h"
#include "usart.h"
void sendcmd(uint8_t *cmd)
{
	printf("%s\r",cmd);	//发送命令
}
void UartSendChar( char chr )
{
	if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)&chr, 1)!= HAL_OK)
  {
    /* Transfer error in transmission process */
    Error_Handler();    
  }
  /*##-6- Wait for the end of the transfer ###################################*/  
  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)
  {
  }
}
/* 
此文档关于如何从透传模式进入到命令模式，和如何从命令模式回到透传模式。适用于USR-WIFI232低功耗系列模块(-T-G-S-H)
注：包含一些伪代码，请根据应用进行修改。
*/

//#define MAX_RX_BUF_LEN 100 //the max receive buffer length, you might change it for your application

//char mode; //记录当前模式状态, 1 命令模式, 0 透传模式
//char uart_rx_buf[MAX_RX_BUF_LEN]; //接收缓冲区
//int uart_rx_cnt; //记录接收的字节数
//void DelayMS( int ms ); //延时函数，单位毫秒
//void UartSendChar( char chr ); //串口发送一个字节
//vodi UartSendString( char* str ); //串口发送字符串

////让模块从透传模式进入命令模式
//int Module2CMD()
//{
//	if( !mode ) //确保模式没有在命令模式
//	{
//		uart_rx_cnt = 0; //清零计数器
//		memset( uart_rx_buf, 0, MAX_RX_BUF_LEN ); //清零缓冲区
//		DelayMS(50); //延时50ms
//		UartSendChar('+'); //向模块发送一个'+'
//		DelayMS(10); //延时10ms
//		UartSendChar('+');
//		DelayMS(10);
//		UartSendChar('+');
//		DelayMS(400);

//		if ( 'a' == uart_rx_buf[0] )
//		{
//			UartSendChar('a'); //向模块发送一个'a'
//			DelayMS(10);
//			return 1;
//		}
//		else if ( '+' == uart_rx_buf[0] && '+' == uart_rx_buf[1] && '+' == uart_rx_buf[2] )
//		{
//			return 1;
//		}
//		else return 0;
//	}
//	else return 1;
//}

////让模块从命令模式回到透传模式
//int Module2Pass()
//{
//	if( mode ) //确保模块已在命令模式
//	{
//		uart_rx_cnt = 0; //清零计数器
//		memset( uart_rx_buf, 0, MAX_RX_BUF_LEN ); //清零缓冲区
//		UartSendString("AT+ENTM\r"); //发送"ENTM"命令，让模块回到透传模式
//		DelayMS(200); //延时200ms
//		if ( uart_rx_cnt >= 7 && 'o' == uart_rx_buf[1] && 'k' == uart_rx_buf[2] ) return 1;
//		else if ( uart_rx_cnt >= 16 && 'o' == uart_rx_buf[10] && 'k' == uart_rx_buf[11] ) return 1;
//		return 0;
//	}
//	else return 1;
//}


//int main()
//{
//	int i;

//	//.........

//	//让模块进入命令模式
//	while ( the nReady pin is high ) nop; //等待模块启动完成，“nReady”脚为低电平有效
//	i = 3; //尝试操作3次
//	while ( i )
//	{
//		if ( Module2CMD() ) break;
//		else i--;
//	}

//	if ( i > 0 ) //检查i的值，如果i>0则模块已进入命令模式，否则进入命令模式失败。
//	{
//		//可以向模块发送AT命令，例如：获取模块的版本号
//		uart_rx_cnt = 0; // //清零计数器
//		memset( uart_rx_buf, 0, MAX_RX_BUF_LEN ); //清零缓冲区
//		UartSendString("AT+VER\r");
//		//如果未开启回显，缓冲区里的内容可能是："+ok=V1.0.04a\r\n"
//		//如果已开启回显，缓冲区里的内容可能是："AT+VER\n\r+ok=V1.0.04a\r\n"
//	}
//	else
//	{
//		//添加容错处理
//	}

//	//.........

//	//让模块回到透传模式
//	while ( the nReady pin is high ) nop; //等待模块启动完成，“nReady”脚为低电平有效
//	i = 3; //尝试操作3次
//	while ( i )
//	{
//		if ( Module2Pass() ) break;
//		else i--;
//	}

//	if ( i <= 0 ) //检查i的值，如果i>0则模块已进入透传模式，否则进入透传模式失败。
//	{
//		//添加容错处理
//	}

//	//.........
//}
void wifi_init(void)
{
HAL_Delay(2000);
	UartSendChar('+');
	HAL_Delay(10);
	UartSendChar('+');
	HAL_Delay(10);
	UartSendChar('+');
	HAL_Delay(400);
	UartSendChar('a');
	HAL_Delay(1300);
	sendcmd("AT+WMODE=STA");
	HAL_Delay(1000);
//	sendcmd("AT+WSSSID=TP-LINK_4F");
	sendcmd("AT+WSSSID=lab");
	HAL_Delay(1000);
//	sendcmd("AT+WSKEY=WPA2PSK,AES,qwer123789");
	sendcmd("AT+WSKEY=WPA2PSK,AES,lab123456");
	HAL_Delay(1000);
	sendcmd("AT+NETP=TCP,Server,1992,192.168.9.192");
	HAL_Delay(1000);
	sendcmd("AT+WANN=static,192.168.9.192,,255.255.255.0,192.168.9.1");
	HAL_Delay(2000);
	sendcmd("AT+ENTM\r"); //发送"ENTM"命令，让模块回到透传模式
		HAL_Delay(200); //延时200ms
}

