#include <8052.h>
#define _nop_() __asm nop __endasm
typedef signed char int8;        // 8 λ�з���������
typedef signed int int16;        // 16 λ�з���������
typedef signed long int32;       // 32 λ�з���������
typedef unsigned char uint8;     // 8 λ�޷���������
typedef unsigned int uint16;     // 16 λ�޷���������
typedef unsigned long uint32;    // 32 λ�޷���������
#define SYS_MCLK (11059200 / 12) // ϵͳ��ʱ��Ƶ�ʣ�������Ƶ�ʡ�12
#include "kingst.h"
/***************************Ultrasonic.h �ļ�����Դ����***************************/
#define Trig P2_7 // ���������ƶ�����
#define Echo P2_6 // ���������ն�����
void InitUltrasonic();
unsigned int GetDistance();
/***************************Ultrasonic.c �ļ�����Դ����***************************/
/*�����ʱ����ʱʱ�䣨t*10)us*/
void DelayX10us(unsigned char t) {
    do {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    } while (--t);
}
/* ��ȡ�������ݣ�����ֵ���Ժ���Ϊ��λ�ľ���ֵ */
unsigned int GetDistance() {
    float time;
    float result;
    TH0 = 0; // ��ʱ�� 0 ���㣬׼�����β�����
    TL0 = 0;
    Trig = 1;      // Trig ����ߵ�ƽ
    DelayX10us(2); // ��ʱ 20us
    Trig = 0;      // Trig ����͵�ƽ�����˲���һ�� 20us �������ź�������������ģ��
    while (!Echo)
        ;    // �ȴ��ز��źţ��� Echo Ϊ 0 ʱ�ȴ����� Echo Ϊ 1 ʱ��������ִ��
    TR0 = 1; // ���� T0 ��ʼ��ʱ
    while (Echo)
        ;         // �ȴ��ز��źŽ���
    TR0 = 0;      // �رն�ʱ�� 0 ֹͣ��ʱ
    if (TF0 == 1) // TF0 Ϊ 1 ʱ˵����ʱ�������
    {
        TF0 = 0;      // �����־λ
        time = 65535; // ���ʱ��ֱ������Ϊ������ֵ
    } else
        time = TH0 * 256 + TL0; // δ���ʱ����ȡ��ʵ�ʼ���ֵ
    // ����������ת��Ϊʵ��ʱ�䣨us������������õ������ؾ��룬�� 2 �õ����̾����ʱ��
    time = time * (12 / 11.0592) / 2;
    // time(s)*�ٶ� = ���룬340 Ϊ�����ڿ����еĴ����ٶ�(m/s)
    result = (time / 1000000) * 340;
    if (result > 4.0) // �ж��Ƿ񳬳�ģ��Ĳ�����Χ
        result = 4.0; // �������� 4m ������Ϊ 4m
    else if (result < 0.02)
        result = 0.02; // �������� 0.02m ������Ϊ 0.02m
    // ���޷������������ز���������� 1000 ʹ�䵥λ��Ϊ����
    return (unsigned int)(result * 1000);
}
/*��ʼ��������ģ��*/
void InitUltrasonic() {
    TMOD &= 0xf0;
    TMOD |= 0x01; // ���� T0 Ϊ��ʽ 1
    TH0 = 0;
    TL0 = 0;
    ET0 = 0;  // �ر� T0 �ж�
    Trig = 0; // ����ģ�鴥������
}
/******************************main.c �ļ�����Դ����*****************************/
uint16 flag = 0;     // ���嶨ʱ��־
uint8 LedBuff[7] = { // ����ܺ� 8 ������ LED ����ʾ����������ֵ 0xFF ȷ������ʱ������
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const int8 __code LedChar[] = { // �������ʾ�ַ�ת����
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};
const uint8 __code ledtab[8] = { // led ��ʾת����
    0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00};
void ShowDistance();
/* ������ */
void main() {
    EA = 1;    // ʹ�����ж�
    ENLED = 0; // ʹ�� U3��ѡ����������
    ADDR3 = 1; // ��Ϊ��Ҫ��̬�ı� ADDR0-2 ��ֵ�����Բ���Ҫ�ٳ�ʼ����
    TMOD &= 0x0f;
    TMOD |= 0x10; // ���� T1 Ϊģʽ 1
    TH1 = 0xFC;   // Ϊ T1 ����ֵ 0xFC67����ʱ 1ms
    TL1 = 0x67;
    ET1 = 1;          // ʹ�� T1 �ж�
    TR1 = 1;          // ���� T1
    InitUltrasonic(); // ��ʼ��������ģ��
    while (1) {
        if (flag == 1) // ÿ��� 200ms ����һ�β��
        {
            flag = 0;
            ShowDistance();
        }
    }
}
/* ִ�в�ಢ��ʾ��� */
void ShowDistance() {
    int8 i;
    uint8 buff[5];
    uint16 distance; // �ݴ����ֵ
    distance = GetDistance();
    buff[0] = distance % 10;
    buff[1] = distance / 10 % 10;
    buff[2] = distance / 100 % 10;
    buff[3] = distance / 1000 % 10;
    buff[4] = distance / 10000 % 10;
    for (i = 4; i >= 1; i--) // �������ָ�λ�� 0 ת��Ϊ�հ׷�
    {
        if (buff[i] == 0)
            LedBuff[i] = 0xff;
        else
            break;
    }
    for (; i >= 0; i--) // ��Ч����ת��Ϊ��ʾ�ַ�
        LedBuff[i] = LedChar[buff[i]];
    // �ڴӵ���� 2 λ���С���㣬��ʾ��ʾ��ֵ�ĵ�λΪ����
    LedBuff[1] &= 0x7f;
    // �� 4m �ľ������ƽ����Ϊ 8 �����䣬�� LED ��ֱ�۷�Ӧ����
    LedBuff[6] = ledtab[(distance - 1) / 500];
}
/* ��ʱ�� 1 �жϷ����� */
void InterruptTimer1() __interrupt(3) {
    static uint8 i = 0;    // LED λѡ����
    static uint16 cnt = 0; // �����������
    TH1 = 0xFC;            // ���¼��س�ֵ
    TL1 = 0x67;
    P0 = 0xFF;                     // �ر����ж�ѡλ����ʾ����
    P1 = (P1 & 0xF8) | i;          // λѡ����ֵ��ֵ�� P1 �ڵ� 3 λ
    P0 = LedBuff[i];               // ������������λ�õ������͵� P0 ��
    if (i < (sizeof(LedBuff) - 1)) // ��������ѭ������������������
        i++;
    else
        i = 0;
    cnt++;
    if (cnt >= 200) // ��ʱ 200ms
    {
        cnt = 0;  // �������ֵ
        flag = 1; // ���ö�ʱ��־Ϊ 1
    }
}