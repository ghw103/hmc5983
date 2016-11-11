#include "Wifi.h"
#include "usart.h"
void sendcmd(uint8_t *cmd)
{
	printf("%s\r",cmd);	//��������
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
���ĵ�������δ�͸��ģʽ���뵽����ģʽ������δ�����ģʽ�ص�͸��ģʽ��������USR-WIFI232�͹���ϵ��ģ��(-T-G-S-H)
ע������һЩα���룬�����Ӧ�ý����޸ġ�
*/

//#define MAX_RX_BUF_LEN 100 //the max receive buffer length, you might change it for your application

//char mode; //��¼��ǰģʽ״̬, 1 ����ģʽ, 0 ͸��ģʽ
//char uart_rx_buf[MAX_RX_BUF_LEN]; //���ջ�����
//int uart_rx_cnt; //��¼���յ��ֽ���
//void DelayMS( int ms ); //��ʱ��������λ����
//void UartSendChar( char chr ); //���ڷ���һ���ֽ�
//vodi UartSendString( char* str ); //���ڷ����ַ���

////��ģ���͸��ģʽ��������ģʽ
//int Module2CMD()
//{
//	if( !mode ) //ȷ��ģʽû��������ģʽ
//	{
//		uart_rx_cnt = 0; //���������
//		memset( uart_rx_buf, 0, MAX_RX_BUF_LEN ); //���㻺����
//		DelayMS(50); //��ʱ50ms
//		UartSendChar('+'); //��ģ�鷢��һ��'+'
//		DelayMS(10); //��ʱ10ms
//		UartSendChar('+');
//		DelayMS(10);
//		UartSendChar('+');
//		DelayMS(400);

//		if ( 'a' == uart_rx_buf[0] )
//		{
//			UartSendChar('a'); //��ģ�鷢��һ��'a'
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

////��ģ�������ģʽ�ص�͸��ģʽ
//int Module2Pass()
//{
//	if( mode ) //ȷ��ģ����������ģʽ
//	{
//		uart_rx_cnt = 0; //���������
//		memset( uart_rx_buf, 0, MAX_RX_BUF_LEN ); //���㻺����
//		UartSendString("AT+ENTM\r"); //����"ENTM"�����ģ��ص�͸��ģʽ
//		DelayMS(200); //��ʱ200ms
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

//	//��ģ���������ģʽ
//	while ( the nReady pin is high ) nop; //�ȴ�ģ��������ɣ���nReady����Ϊ�͵�ƽ��Ч
//	i = 3; //���Բ���3��
//	while ( i )
//	{
//		if ( Module2CMD() ) break;
//		else i--;
//	}

//	if ( i > 0 ) //���i��ֵ�����i>0��ģ���ѽ�������ģʽ�������������ģʽʧ�ܡ�
//	{
//		//������ģ�鷢��AT������磺��ȡģ��İ汾��
//		uart_rx_cnt = 0; // //���������
//		memset( uart_rx_buf, 0, MAX_RX_BUF_LEN ); //���㻺����
//		UartSendString("AT+VER\r");
//		//���δ�������ԣ�������������ݿ����ǣ�"+ok=V1.0.04a\r\n"
//		//����ѿ������ԣ�������������ݿ����ǣ�"AT+VER\n\r+ok=V1.0.04a\r\n"
//	}
//	else
//	{
//		//����ݴ���
//	}

//	//.........

//	//��ģ��ص�͸��ģʽ
//	while ( the nReady pin is high ) nop; //�ȴ�ģ��������ɣ���nReady����Ϊ�͵�ƽ��Ч
//	i = 3; //���Բ���3��
//	while ( i )
//	{
//		if ( Module2Pass() ) break;
//		else i--;
//	}

//	if ( i <= 0 ) //���i��ֵ�����i>0��ģ���ѽ���͸��ģʽ���������͸��ģʽʧ�ܡ�
//	{
//		//����ݴ���
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
	sendcmd("AT+ENTM\r"); //����"ENTM"�����ģ��ص�͸��ģʽ
		HAL_Delay(200); //��ʱ200ms
}

