#include <unistd.h>
#include <sys/time.h>
#include <cstdlib>
#include <string.h>
#include <fstream>
#include <thread>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
int64_t getCurrentTime()       //ʱ���ȡ
  {      struct timeval tv;  
         gettimeofday(&tv,NULL);   
         return tv.tv_sec * 1000 + tv.tv_usec / 1000; 
   }   
using namespace std;

float jd2trueflaot(const char* input)
{
char jd[5];  
char jdfe[7];
memcpy(jd,input,3);
jd[4]='.';
jd[5]='0';
memcpy(jdfe,input+3,7);
float jdf=atof(jd);
float jdfef=atof(jdfe);
float output=jdf+jdfef/60;
return output;

}
float wd2trueflaot(const char* inputs)
{
char wd[4];  
char wdfe[7];
memcpy(wd,inputs,2);
wd[2]='.';
wd[3]='0';
memcpy(wdfe,inputs+2,7);
float wdf=atof(wd);
float wdfef=atof(wdfe);
float output=wdf+wdfef/60;
return output;
}
int main()
{

speed_t baud_rate_i,baud_rate_o;
struct termios options, newstate;
int fd=open("/dev/ttyS3", O_RDWR|O_NOCTTY|O_NDELAY);	//�򿪴���   
if(fd==-1)         
printf("can not open the COM1!\n");  
else   
 printf("open COM1 ok!\n");
// if( fcntl(fd, F_SETFL, 0) <0 ) //��Ϊ����ģʽ
//printf("fcntl failed\n");
//else 
//printf("fcntl=%d\n", fcntl(fd, F_SETFL, 0));
tcgetattr(fd, &options);
    //���ò�����
cfsetispeed(&options, B38400);
cfsetospeed(&options, B38400);
    //��ȡ������
tcgetattr(fd, &newstate);
baud_rate_i=cfgetispeed(&newstate);
baud_rate_o=cfgetospeed(&newstate);
    //��������
options.c_cflag |= (CLOCAL | CREAD);
options.c_cflag &= ~PARENB;//��������żУ��λ��N
options.c_cflag &= ~CSTOPB; //����ֹͣλ1
options.c_cflag &= ~CSIZE;
options.c_cflag |= CS8; //��������λ
//options.c_cc[VTIME]=1;//����ģʽ������
//options.c_cc[VMIN]=1;
    //����������
tcsetattr(fd, TCSANOW, &options);
    //���������
//signal(SIGALRM, printMes);
//printf("input %d  ouput  uart %d\n" , baud_rate_i, baud_rate_o);

char buff[212];  
while(1){
memset(buff,0,212);
string gpstj;
read(fd,buff,212) ;
memset(buff,0,212);
if(read(fd,buff,212) > 0&&(buff[0]>0&&buff[1]>0))
{ 
 string gpstjbg="      ";
 string kon=" ";
 gpstj=buff;
 char utcstr[12],astr[12],nstr[12],hstr[12],hstrt[12];
 float asf,nsf,hsf;
  for(int j=0;j<6;j++)
  {
  gpstjbg[j]=gpstj[j];
  }
   if(gpstjbg=="$GNGGA")
   { 
    printf("recivelocate : \n");
    memcpy(utcstr,buff+7,10);
    memcpy(astr,buff+18,9);
    memcpy(nstr,buff+30,10);
    memcpy(hstr,buff+52,6);
    utcstr[10]=' ';
    int ji=0;
     for(int j=0;j<6;j++)
     {
       if(hstr[j]!=',')
        {hstrt[ji]=hstr[j];
        ji++;}
       if(hstr[j]==',')
       j=6;
     }
   nsf=jd2trueflaot(nstr);
   asf=wd2trueflaot(astr);
   //asf=atof(astr);
   hsf=atof(hstrt);
   printf("time:%s   weidu N: %f,   jindu E: %f, gaodu: %f  \n",utcstr,asf,nsf,hsf);
   }
  }
 }
}
