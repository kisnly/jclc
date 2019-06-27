#include "main.h"
typedef enum {CMD_READY, CMD_RX} Cmd_State_t;
void SW_Que_Init(PtrQue_TypeDef * swq)
{
	SW_Handle_t * sh;
	int i;
	for (i = 0; i < SW_NUM; i++) //��ʼ�����������У�����ɴ�����������ֳ�ʼ��
	{
		sh = (SW_Handle_t *)malloc(sizeof(SW_Handle_t));
		sh->action = ACT_STOP;
		sh->command_new = FALSE;
		sh->result = RES_OK;
		sh->status = ACT_STOP;
		sh->cmdbuf = malloc(SW_CMD_LEN);
		sh->cmdsize = SW_CMD_LEN;
		PtrQue_In(swq, (void *)sh);
	}

}
BOOL_t SW_Cmd_Exec(PtrQue_TypeDef * swq)
{
	int i;
	SW_Handle_t * sw;
	for (i = 0; i < __PTRQUE_COUNT(swq); i++)
	{
		if (PtrQue_Query(swq, (void **)&sw))
		{
			if (sw->command_new)
				break;
		}
	}
	if (i >= __PTRQUE_COUNT(swq))
		return FALSE;
	SW_Control(sw);
	return TRUE;
}
/**
  * @brief  Analysis command received from UART and refresh switch handle que when received new char c.
  * @param  swq: pointer of switch handle que
  *         c:   new char received from UART
  * @note   
  * @retval None
  */
void SW_Cmd_Analysis(PtrQue_TypeDef * swq, char c)
{
	static char buf[5];
	static uint8_t i = 0;
	static Cmd_State_t s = CMD_READY;
	uint8_t cmd_rx;
	cmd_rx = 0;
	switch (s)
	{
		case CMD_READY:
			if (c == '@')
			{
				i = 0;
				s = CMD_RX;
			}
			break;
		case CMD_RX:
			if (c == '@')
			{
				i = 0;
				s = CMD_READY;
			}
			else if (c == '#')
			{
				if (i == 3) cmd_rx = 1;//received a command.
				i = 0;
				s = CMD_READY;
			}
			else if (i > 2)//overflow
			{
				i = 0;
				s = CMD_READY;
			}
			else
			{
				buf[i] = c;
				i++;
			}
			break;
		default:
			s = CMD_READY;
	}
	if (cmd_rx)
	{
		buf[3] = '\0';
		printf("rx cmd: %s\r\n", buf);
	}
}

void SW_Control(SW_Handle_t * sw)
{
	sw->command_new = FALSE;
	if (sw->status == sw->action)
	{
		sw->result = RES_KEEP;
		return;
	}
	switch (sw->action)
	{
		case ACT_OFF:
			SW_OFF(sw);
			break;
		case ACT_ON:
			SW_ON(sw);
			break;
		case ACT_STOP:
			SW_STOP(sw);
			break;
	}
}
CommandResType_t SW_OFF(SW_Handle_t * sw)
{
	CommandResType_t res;
	switch (sw->type)
	{
		case SW_FAN:
			res = RES_FAILED;
			break;
		case SW_SUNSHADE:
		case SW_TOPFILM:
		case SW_SIDEFILM:
			K_ON(sw->k1_port, sw->k1_pin);
			K_OFF(sw->k2_port, sw->k2_pin);
			sw->status = ACT_OFF;
			res = RES_OK;
			break;
		case SW_LIGHT:
			res = RES_FAILED;
			break;
	}
	return res;
}
CommandResType_t SW_ON(SW_Handle_t * sw)
{
	switch (sw->type)
	{
		case SW_FAN:
		case SW_SUNSHADE:
		case SW_TOPFILM:
		case SW_SIDEFILM:
			K_OFF(sw->k1_port, sw->k1_pin);
			K_ON(sw->k2_port, sw->k2_pin);
			break;
		case SW_LIGHT:
			K_ON(sw->k1_port, sw->k1_pin);
			K_OFF(sw->k2_port, sw->k2_pin);
			break;
	}
	sw->status = ACT_ON;
	return RES_OK;
}
CommandResType_t SW_STOP(SW_Handle_t * sw)
{
	K_OFF(sw->k1_port, sw->k1_pin);
	K_OFF(sw->k2_port, sw->k2_pin);
	sw->status = ACT_OFF;
	return RES_OK;
}
