#include "ML307A.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
#include "cJSON.h"
char *strx,*extstrx;
extern char  RxBuffer[200],RxCounter;
ML307 ML307_Status;
unsigned char socketnum=0;//当前的socket号码
//需要用户改的 
#define Pubtopic  "$sys/xnWEo430Ir/hum_temp/thing/property/post"  //发布主题
#define Subtopic  "$sys/xnWEo430Ir/hum_temp/#"  //订阅主题
//需要用户改的 
#define ProductID  "xnWEo430Ir"  //产品名称
#define DeviceID  "hum_temp"  //设备名称
#define Token    "version=2018-10-31&res=products%2FxnWEo430Ir%2Fdevices%2Fhum_temp&et=1702961458&method=md5&sign=YQgU7Fswqjh1NZFhe0bOKw%3D%3D"  //token值

void Clear_Buffer(void)//清空缓存
{
		u8 i;
		Uart1_SendStr(RxBuffer);
		for(i=0;i<200;i++)
		RxBuffer[i]=0;//缓存
		RxCounter=0;
		IWDG_Feed();//喂狗
	
}
void ML307_Init(void)
{
		Uart1_SendStr("本次测试是发数据到MQTT服务器功能\r\n");
	  delay_ms(300);
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    }
    printf("ATE0&W\r\n"); //关闭回显
    delay_ms(300);
		printf("AT+MQTTDISC=0\r\n");
		delay_ms(300);
    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//返460，表明识别到卡了
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//返回OK,说明卡是存在的
    }
        printf("AT+CGDCONT=2,\"IP\",\"CMNET\"\r\n");//设置APN参数
        delay_ms(300);
        Clear_Buffer();	
				printf("AT+MIPCALL=1,1\r\n");//激活网络，PDP
		    delay_ms(300);
        Clear_Buffer();	
        printf("AT+CGATT?\r\n");//查询激活状态
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//返1
        Clear_Buffer();	
			while(strx==NULL)
			{
							Clear_Buffer();	
							printf("AT+CGATT?\r\n");//获取激活状态
							delay_ms(300);
							strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//返回1,表明注网成功
			}
				 printf("AT+CSQ\r\n");//查询信号
				 delay_ms(300);
				 Clear_Buffer();	
  
}


//初始化MQTT
void ML307_MQTTInit(void)
{
	  printf("AT+MQTTCFG=\"clean\",0,1\r\n");//设置登录参数
    delay_ms(300);
    Clear_Buffer();	
	    printf("AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,\"%s\",\"%s\",\"%s\"\r\n",DeviceID,ProductID,Token);//连接到ONENET平台
     delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+MQTTURC: \"conn\",0");//返登录成功
   while(strx==NULL)
    { 
       strx=strstr((const char*)RxBuffer,(const char*)"+MQTTURC: \"conn\",0");//登录到ONENET成功 此时设备会显示在线  
    }
//		   while(1)
//    Clear_Buffer();	
		printf("AT+MQTTSUB=0,\"%s\",0\r\n",Subtopic);
		delay_ms(300);
	  strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUB: 0");//订阅成功
   while(strx==NULL)
    { 
       strx=strstr((const char*)RxBuffer,(const char*)"+MQTTSUB: 0");//订阅成功
    }
    Clear_Buffer();
		
}
//发送数据到服务器
void ML307_MQTTPUBMessage(char*temp)
{
		char json[]="{\"id\":123,\"version\":1.0,\"params\":{\"temperatrue\":[{\"v\":%d}]}}";
		char t_json[200];
		int json_len;
		sprintf(t_json, json, 23);
		json_len = strlen(t_json)/sizeof(char);
    printf("AT+MQTTPUB=0,%s,0,0,0,%d\r\n",Pubtopic,json_len);//发布数据到平台
	  //printf("AT+MQTTPUB=0,\"world\",0,0,0,4,\"3242\"\r\n");
	  // printf("AT+MQTTPUB=\"pyr\",1,0,0,\"abcdef\"\r\n");
    delay_ms(300);
		printf(t_json);
	  delay_ms(300);
     strx=strstr((const char*)RxBuffer,(const char*)"+MQTTPUB: 0");//返回发布成功
    while(strx==NULL)
    { 
       strx=strstr((const char*)RxBuffer,(const char*)"+MQTTPUB: 0");//发布数据成功
    }
    Clear_Buffer();

}

void get_api(char *city_name,char *weather,char *temp,char *humi,char * windspeed)
{
	//清除HTTP连接缓存
	Clear_Buffer();
	printf("AT+MHTTPDEL=0\r\n");
	delay_ms(300);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");
	while(strx!=NULL);
	Clear_Buffer();
	//HTTP连接
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
