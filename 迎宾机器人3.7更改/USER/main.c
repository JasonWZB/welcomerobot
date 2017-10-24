#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "rs485.h"
#include "hc05.h"
#include "usart3.h"
#include "timer.h"
#include "pid.h"
#include "pwm.h"
#include "led.h"
#include "uart4.h"
#include "math.h"
#include "route.h"
/*��������*/

void system_init(void);
void lanyakongzhi(void);
void lanyafashuju(void);
void shengyinkongzhi(void);
void InitVar(int walk,int terminus,int seeter,int beginselect,int overselect);
/*��������*/

u8 rs485qianjinbuf[8] = {0x01,0x06,0x00,0xc8,0x00,0xa1,0xc9,0x8c};	
u8 rs485zuozhuanbuf[8] = {0x01,0x06,0x00,0xc8,0x00,0xa2,0x89,0x8d};	
u8 rs485youzhuanbuf[8] = {0x01,0x06,0x00,0xc8,0x00,0xa3,0x48,0x4d};	
u8 rs485houtuibuf[8] = {0x01,0x06,0x00,0xc8,0x00,0xa4,0x09,0x8f};	
u8 rs485tingzhibuf[8] = {0x01,0x06,0x00,0xc8,0x00,0xff,0x48,0x74};	
u8 rs485buzoubuf[8] = {0x01,0x06,0x00,0xc8,0x00,0xb1,0xc8,0x40};	
/*��������*/
extern u16 distance;//���������
extern u8 time_flag;//���������
extern int waydirection;//֧·ѡ��ķ��� 1�� -1�� 0ֻ��һ��
u8 bitt=0;
u8 reclen=0; 
u8 reclen4=0;
u8 fflag = 0;
u8 xuhao = 0;
u8 cishu = 0;
int xpo = 0;
int ypo = 0;
int zpo = 0;
u8 zhongjian = 0;
u16 xxian,yxian;
//�������־λ����ʹ���ж�
int walkstate=0,terminusstate=0;
#define seekfootON walkstate=1
#define seekfootOFF walkstate=0
#define seekpointON terminusstate=1
#define seekpointOFF terminusstate=0
int seeterminus=0;//���Ѱ��ӡ�����м�⵽���յ�
//֧·ѡ���־λ
int selectbegin=0,selectover=0;
#define seekwayON selectbegin=1
#define seekwayOFF selectbegin=0
#define seekwayON selectbegin=1
#define seekwayON selectbegin=1
int numb,numc;
extern int zxpo,zypo;
/**/

//[m3]  Ůƽ��
//[m51] ������
//[m52] ������

//���Կ��ԣ����Ż�
void pointTopoint(int nowpoint,int nextpoint,int roadtime,int pointenddistance)
{
	InitVar(0,0,0,0,0);
	kongzhidong(0,0);
	closetopoint(pointenddistance,2,150);
	chooseroad(nowpoint,nextpoint);
	kongzhidong(0,0);
	delay_ms(1000);
	ontheroad(roadtime,10);
}


int main(void)
{
	system_init();
	pointTopoint(302,401,6,25);
	pointTopoint(401,501,6,25);

}

void InitVar(int walk,int terminus,int seeter,int beginselect,int overselect)
{
	walkstate=walk;//Ѱ��ӡ
	terminusstate=terminus;//Ѱ����յ�
	seeterminus=seeter;//�յ����־λ
	selectbegin=beginselect;//ִ��֧·ѡ���� 
	selectover=overselect;//ѡ��֧·��ɱ�־
}


void system_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����
	delay_init(168);  //��ʼ����ʱ����
  usart3_init(9600);
	RS485_Init(9600);
	usart4_init(9600);
	//HMISendstart();
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200
	LED2_Init();
	TIM13_PWM_Init(1000-1,12-1); //84000000/12/1000 =7KHZ
	TIM4_PWM_Init(1000-1,12-1);  //84000000/12/1000 =7KHZ
	USART3_RX_STA=0;
	USART4_RX_STA=0;
	
	
	TIM_SetCompare1(TIM14,0);
	TIM_SetCompare2(TIM3,0);
}



void lanyafashuju(void)
{
	u3_printf("x:%d",xpo);
	u3_printf("y:%d",ypo);
	u3_printf("//");
}

void shengyinkongzhi(void)
{
	if(USART4_RX_STA&0X8000)			//���յ�һ��������
	{
		reclen4=USART4_RX_STA&0X7FFF;	//�õ����ݳ���
		USART4_RX_BUF[reclen4]=0;	 	//���������
		USART4_RX_STA=0;	 
//		printf("%s/t",USART3_RX_BUF);
//		printf("reci/t");
		if(USART4_RX_BUF[0] == 1)
		{
		
			RS485_Send_Data(rs485qianjinbuf,8);
			
		}
		else if(USART4_RX_BUF[0] == 2)
		{
			RS485_Send_Data(rs485zuozhuanbuf,8);
		}
		else if(USART4_RX_BUF[0] == 3)
		{
			RS485_Send_Data(rs485youzhuanbuf,8);
		}
		else if(USART4_RX_BUF[0] == 4)
		{
			RS485_Send_Data(rs485houtuibuf,8);
		}
		else if(USART4_RX_BUF[0] == 5)
		{
			RS485_Send_Data(rs485tingzhibuf,8);
		}
		else if(USART4_RX_BUF[0] == 6)   //xx����
		{
		
		}
		else if(USART4_RX_BUF[0] == 7)
		{
			
		}
		delay_ms(10);
		usartt4 = 0;
	}	 			
	
}



