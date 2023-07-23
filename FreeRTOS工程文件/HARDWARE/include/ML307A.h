#ifndef __M5311_H
#define __M5311_H	
#include "usart.h"
#include <stm32f10x.h>
#include "delay.h"
#include "stdbool.h"
void Clear_Buffer(void);//��ջ���	
void ML307_Init(void);
void ML307_MQTTInit(void);
void ML307_MQTTPUBMessage(char*temp);
void get_api(char *city_name,char *weather,char *temp,char *humi,char * windspeed);
void Clear_Buffer(void);
typedef struct
{
    uint8_t CSQ;    
    uint8_t Socketnum;   //���
    uint8_t reclen;   //��ȡ�����ݵĳ���
    uint8_t res;      
    uint8_t recdatalen[10];
    uint8_t recdata[100];
    uint8_t netstatus;//����ָʾ��
} ML307;

#endif





