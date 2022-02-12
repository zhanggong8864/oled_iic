#include "OLED_I2C.h"
#include "bsp_i2c_gpio.h"
#include "bsp_systick.h"
#include "codetab.h"

uint8_t DataBuffer[8][128];
uint8_t Bef[3];//����ǰһ�����ݵļ�������1.Ҫд�ڵڼ�ҳ2.0x01Ҫ�ƶ���λ3.дʲô����
uint8_t Cur[3];//��ǰǰһ������1.Ҫд�ڵڼ�ҳ2.0x01Ҫ�ƶ���λ3.дʲô����

/*
*********************************************************************************************************
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
uint8_t OLED_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;
	
	i2c_Start();		/* ���������ź� */

	i2c_SendByte(_Address|OLED_I2C_WR);/* �����豸��ַ */
	ucAck = i2c_WaitAck();	/* ����豸��ACKӦ�� */

	i2c_Stop();			/* ����ֹͣ�ź� */

	return ucAck;
}


 /**
  * @brief  I2C_WriteByte����OLED�Ĵ�����ַдһ��byte������
  * @param  addr���Ĵ�����ַ
	*					data��Ҫд�������
  * @retval ��
  */
void I2C_WriteByte(uint8_t addr,uint8_t data){
	
	i2c_Start();//����I2C����
	
	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(OLED_ADDRESS|OLED_I2C_WR);
	
	/*�ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* OLED������Ӧ�� */
	}
		
	i2c_SendByte(addr);//���ͼĴ�����ַ
	
	/*�ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* OLED������Ӧ�� */
	}

	i2c_SendByte(data);//��������
	
	/*�ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* OLED������Ӧ�� */
	}	
	
 /* ����I2C����ֹͣ�ź� */
	i2c_Stop();

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();

}


 /**
  * @brief  WriteCmd����OLEDд������
  * @param  I2C_Command���������
  * @retval ��
  */
void WriteCmd(unsigned char I2C_Command)//д����
{
	I2C_WriteByte(0x00, I2C_Command);
}


 /**
  * @brief  WriteDat����OLEDд������
  * @param  I2C_Data������
  * @retval ��
  */
void WriteDat(unsigned char I2C_Data)//д����
{
	I2C_WriteByte(0x40, I2C_Data);
}


 /**
  * @brief  OLED_Init����ʼ��OLED
  * @param  ��
  * @retval ��
  */
void OLED_Init(void)
{
	i2c_CfgGpio();				 /*I2C���ߵ�GPIO��ʼ��*/
	Delay_ms(1000);		// 1s,�������ʱ����Ҫ,�ϵ����ʱ��û�д�����������
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}


 /**
  * @brief  OLED_SetPos�����ù��
  * @param  x,�ڼ�ҳ
	*					y,�ڼ���
  * @retval ��
  */
void OLED_SetPos(unsigned char x, unsigned char y) //������ʼ������
{ 
	WriteCmd(0xb0+x);
	WriteCmd((y&0x0f)|0x00);//LOW
	WriteCmd(((y&0xf0)>>4)|0x10);//HIGHT
}

 /**
  * @brief  OLED_Fill�����������Ļ
  * @param  fill_Data:Ҫ��������
	* @retval ��
  */
void OLED_Fill(void)//ȫ�����
{
	uint8_t i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<128;j++)
		{
			DataBuffer[i][j]=0xff;
		}
	}
	Write_DataBuffer();
}

 /**
  * @brief  OLED_CLS������
  * @param  ��
	* @retval ��
  */
void OLED_CLS(void)//����
{
	uint8_t i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<128;j++)
		{
			DataBuffer[i][j]=0x00;
		}
	}
	Write_DataBuffer();
}


 /**
  * @brief  OLED_ON����OLED�������л���
  * @param  ��
	* @retval ��
  */
void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}


 /**
  * @brief  OLED_OFF����OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
  * @param  ��
	* @retval ��
  */
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ�
	WriteCmd(0XAE);  //OLED����
}

//��ʾ�ַ���
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); ch[] -- Ҫ��ʾ���ַ���; TextSize -- �ַ���С(1:6*8 ; 2:8*16)
	// Description    : ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ�� 
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				//OLED_SetPos(x,y);
				OLED_SetPos(y,x);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				//OLED_SetPos(x,y);
				OLED_SetPos(y,x);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				//OLED_SetPos(x,y+1);
				OLED_SetPos(y+1,x);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

//��ʾ����
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); N:������codetab.h�е�����
	// Description    : ��ʾcodetab.h�еĺ���,16*16����
	unsigned char wm=0;
	unsigned int  adder=32*N;
	//OLED_SetPos(x , y);
	OLED_SetPos(y,x);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	//OLED_SetPos(x,y + 1);
	OLED_SetPos(y+1,x);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}
//��ʾͼƬ
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	// Parameters     : x0,y0 -- ��ʼ������(x0:0~127, y0:0~7); x1,y1 -- ���Խ���(������)������(x1:1~128,y1:1~8)
	// Description    : ��ʾBMPλͼ
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		//OLED_SetPos(x0,y);
		OLED_SetPos(y,x0);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}


void Before_State_Update(uint8_t y)//����y��ֵ�����ǰһ�����ݵ��йز���
{
	Bef[0]=7-y/8;
	Bef[1]=7-y%8;
	Bef[2]=1<<Bef[1];
}
void Current_State_Update(uint8_t y)//����Yֵ�������ǰ���ݵ��йز���
{
	Cur[0]=7-y/8;//����д�ڵڼ�ҳ
	Cur[1]=7-y%8;//0x01Ҫ�ƶ���λ��
	Cur[2]=1<<Cur[1];//Ҫдʲô����
}

//��ʾ���
/*0<=x<=127,0<=y<=63��Ļ����һ�������ᣬ���½���ԭ��*/
void OLED_DrawPoint(uint8_t x,uint8_t y)/*����x�Ǻ����꣬y�������꣬�ڣ�x,y������һ����*/
{
	if(x>127||y>63)
		return;
	uint8_t page,move,data;
	
	page=7-y/8;//����д�ڵڼ�ҳ
	move=7-y%8;//0x01Ҫ�ƶ���λ��
	data=0x01<<move;//Ҫдʲô����
	
	OLED_SetPos(page,x);
	WriteDat(data);
}
//��ʾ����
void OLED_DrawWave(uint8_t x,uint8_t y)
{

	int8_t page_sub;
	uint8_t page_buff,i,j;
	Current_State_Update(y);//����Yֵ�������ǰ���ݵ��йز���
	page_sub=Bef[0]-Cur[0];//��ǰֵ��ǰһ��ֵ��ҳ����Ƚ�
	//ȷ����ǰ�У�ÿһҳӦ��дʲô����
	if(page_sub>0)
	{
		page_buff=Bef[0];
		OLED_SetPos(page_buff,x);
		WriteDat(Bef[2]-0x01);
		page_buff--;
		for(i=0;i<page_sub-1;i++)
		{
			OLED_SetPos(page_buff,x);
			WriteDat(0xff);
			page_buff--;
		}
		OLED_SetPos(page_buff,x);
		WriteDat(0xff<<Cur[1]);
	}
	else if(page_sub==0)
	{
		if(Cur[1]==Bef[1])
		{
			OLED_SetPos(Cur[0],x);
			WriteDat(Cur[2]);
		}
		else if(Cur[1]>Bef[1])
		{
			OLED_SetPos(Cur[0],x);
			WriteDat((Cur[2]-Bef[2])|Cur[2]);
		}
		else if(Cur[1]<Bef[1])
		{
			OLED_SetPos(Cur[0],x);
			WriteDat(Bef[2]-Cur[2]);
		}
	}
	else if(page_sub<0)
	{
		page_buff=Cur[0];
		OLED_SetPos(page_buff,x);
		WriteDat((Cur[2]<<1)-0x01);
		page_buff--;
		for(i=0;i<0-page_sub-1;i++)
		{
			OLED_SetPos(page_buff,x);
			WriteDat(0xff);
			page_buff--;
		}
		OLED_SetPos(page_buff,x);
		WriteDat(0xff<<(Bef[1]+1));
	}
	Before_State_Update(y);
	//����һ�У�ÿһҳ�����������
	for(i=0;i<8;i++)
	{
		OLED_SetPos(i, x+1) ;
		for(j=0;j<1;j++)
			WriteDat(0x00);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED������
*	����˵��: ���I2C�����豸��ʵ���Ƕ�OLED_CheckDevice()�ķ�װ
*	��    �Σ�
*	�� �� ֵ: ����ֵ 0 ��ʾû�м�⵽OLED������1��ʾ��⵽OLED
*********************************************************************************************************
*/

 uint8_t OLED_Test(void) 
{
	
  if (OLED_CheckDevice(OLED_ADDRESS) == 1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}	

/*д ��������*/
void Write_DataBuffer(void)//����ǽ�DataBuffer���������ֵ��ȫ��д������ȥ
{	
	uint8_t i,j;
	for(i=0;i<8;i++)
	{
		OLED_SetPos(i,0); //������ʼ������
		for(j=0;j<128;j++)
		{
			WriteDat(DataBuffer[i][j]);//д����
		}
	}
}

