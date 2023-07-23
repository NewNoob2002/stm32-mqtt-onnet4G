#include "ML307A.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
#include "cJSON.h"
char *strx,*extstrx;
extern char  RxBuffer[200],RxCounter;
ML307 ML307_Status;
unsigned char socketnum=0;//��ǰ��socket����
//��Ҫ�û��ĵ� 
#define Pubtopic  "$sys/xnWEo430Ir/hum_temp/thing/property/post"  //��������
#define Subtopic  "$sys/xnWEo430Ir/hum_temp/#"  //��������
//��Ҫ�û��ĵ� 
#define ProductID  "xnWEo430Ir"  //��Ʒ����
#define DeviceID  "hum_temp"  //�豸����
#define Token    "version=2018-10-31&res=products%2FxnWEo430Ir%2Fdevices%2Fhum_temp&et=1702961458&method=md5&sign=YQgU7Fswqjh1NZFhe0bOKw%3D%3D"  //tokenֵ

void Clear_Buffer(void)//��ջ���
{
		u8 i;
		Uart1_SendStr(RxBuffer);
		for(i=0;i<200;i++)
		RxBuffer[i]=0;//����
		RxCounter=0;
		IWDG_Feed();//ι��
	
}
void ML307_Init(void)
{
		Uart1_SendStr("���β����Ƿ����ݵ�MQTT����������\r\n");
	  delay_ms(300);
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
    printf("ATE0&W\r\n"); //�رջ���
    delay_ms(300);
		printf("AT+MQTTDISC=0\r\n");
		delay_ms(300);
    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//��460������ʶ�𵽿���
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//����OK,˵�����Ǵ��ڵ�
    }
        printf("AT+CGDCONT=2,\"IP\",\"CMNET\"\r\n");//����APN����
        delay_ms(300);
        Clear_Buffer();	
				printf("AT+MIPCALL=1,1\r\n");//�������磬PDP
		    delay_ms(300);
        Clear_Buffer();	
        printf("AT+CGATT?\r\n");//��ѯ����״̬
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//��1
        Clear_Buffer();	
			while(strx==NULL)
			{
							Clear_Buffer();	
							printf("AT+CGATT?\r\n");//��ȡ����״̬
							delay_ms(300);
							strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//����1,����ע���ɹ�
			}
				 printf("AT+CSQ\r\n");//��ѯ�ź�
				 delay_ms(300);
				 Clear_Buffer();	
  
}


//��ʼ��MQTT
void ML307_MQTTInit(void)
{
	  printf("AT+MQTTCFG=\"clean\",0,1\r\n");//���õ�¼����
    delay_ms(300);
    Clear_Buffer();	
	    printf("AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,\"%s\",\"%s\",\"%s\"\r\n",DeviceID,ProductID,Token);//���ӵ�ONENETƽ̨
     delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+MQTTURC: \"conn\",0");//����¼�ɹ�
   while(strx==NULL)
    { 
       strx=strstr((const char*)RxBuffer,(const char*)"+MQTTURC: \"conn\",0");//��¼��ONENET�ɹ� ��ʱ�豸����ʾ����  
    }
//		   while(1)
//    Clear_Buffer();	
		printf("AT+MQTTSUB=0,\"%s\",0\r\n",Subtopic);
		delay_ms(300);
	  strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUB: 0");//���ĳɹ�
   while(strx==NULL)
    { 
       strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUB: 0");//���ĳɹ�
    }
    Clear_Buffer();
		
}
//�������ݵ�������
void ML307_MQTTPUBMessage(char*temp)
{
		char json[]="{\"id\":123,\"version\":1.0,\"params\":{\"temperatrue\":[{\"v\":%d}]}}";
		char t_json[200];
		int json_len;
		sprintf(t_json, json, 23);
		json_len = strlen(t_json)/sizeof(char);
    printf("AT+MQTTPUB=0,%s,0,0,0,%d\r\n",Pubtopic,json_len);//�������ݵ�ƽ̨
	  //printf("AT+MQTTPUB=0,\"world\",0,0,0,4,\"3242\"\r\n");
	  // printf("AT+MQTTPUB=\"pyr\",1,0,0,\"abcdef\"\r\n");
    delay_ms(300);
		printf(t_json);
	  delay_ms(300);
     strx=strstr((const char*)RxBuffer,(const char*)"+MQTTPUB: 0");//���ط����ɹ�
    while(strx==NULL)
    { 
       strx=strstr((const char*)RxBuffer,(const char*)"+MQTTPUB: 0");//�������ݳɹ�
    }
    Clear_Buffer();

}

void get_api(char *city_name,char *weather,char *temp,char *humi,char * windspeed)
{
	//���HTTP���ӻ���
	Clear_Buffer();
	printf("AT+MHTTPDEL=0\r\n");
	delay_ms(300);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");
	while(strx!=NULL);
	Clear_Buffer();
	//HTTP����
	printf("AT+MHTTPCREATE=\"api.seniverse.com:80\"\r\n");
		delay_ms(300);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");
	while(strx!=NULL);
	Clear_Buffer();
	printf("AT+MHTTPREQUEST=0,1,0,\"https://api.seniverse.com/v3/weather/now.json?key=Sd00_-LVgvZIliefr&location=daye&language=zh-Hans&unit=c\"\r\n");
	delay_ms(300);
	const char *json_data=(const char*)RxBuffer;
	Clear_Buffer();
	cJSON *root=cJSON_Parse(json_data);
	while (root == NULL) {
       Uart1_SendStr("Error parsing JSON data.\n");
    }
	cJSON *results = cJSON_GetObjectItem(root, "results");
    if (results != NULL && cJSON_IsArray(results)) {
			cJSON *result = cJSON_GetArrayItem(results, 0);
      cJSON *location = cJSON_GetObjectItem(result, "location");
      cJSON *now = cJSON_GetObjectItem(result, "now");
			
			cJSON *name = cJSON_GetObjectItem(location, "name");
			
			cJSON *text = cJSON_GetObjectItem(now, "text");
      cJSON *temperature = cJSON_GetObjectItem(now, "temperature");
      cJSON *humidity = cJSON_GetObjectItem(now, "humidity");
      cJSON *wind_speed = cJSON_GetObjectItem(now, "wind_speed");
			city_name=name->valuestring;
			weather=text->valuestring;
			temp=temperature->valuestring;
			humi=humidity->valuestring;
			windspeed=wind_speed->valuestring;
		}
		cJSON_Delete(root);
	
}
