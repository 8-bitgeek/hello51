#include <8052.h>
#define _nop_() __asm nop __endasm
typedef signed char int8;        // 8 位有符号整型数
typedef signed int int16;        // 16 位有符号整型数
typedef signed long int32;       // 32 位有符号整型数
typedef unsigned char uint8;     // 8 位无符号整型数
typedef unsigned int uint16;     // 16 位无符号整型数
typedef unsigned long uint32;    // 32 位无符号整型数
#define SYS_MCLK (11059200 / 12) // 系统主时钟频率，即振荡器频率÷12
#include "kingst.h"
/***************************Ultrasonic.h 文件程序源代码***************************/
#define Trig P2_7 // 超声波控制端引脚
#define Echo P2_6 // 超声波接收端引脚
void InitUltrasonic();
unsigned int GetDistance();
/***************************Ultrasonic.c 文件程序源代码***************************/
/*软件延时，延时时间（t*10)us*/
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
/* 获取测量数据，返回值：以毫米为单位的距离值 */
unsigned int GetDistance() {
    float time;
    float result;
    TH0 = 0; // 定时器 0 清零，准备本次测距计数
    TL0 = 0;
    Trig = 1;      // Trig 输出高电平
    DelayX10us(2); // 延时 20us
    Trig = 0;      // Trig 输出低电平，至此产生一个 20us 的脉冲信号以启动超声波模块
    while (!Echo)
        ;    // 等待回波信号：当 Echo 为 0 时等待，当 Echo 为 1 时继续往下执行
    TR0 = 1; // 启动 T0 开始计时
    while (Echo)
        ;         // 等待回波信号结束
    TR0 = 0;      // 关闭定时器 0 停止计时
    if (TF0 == 1) // TF0 为 1 时说明定时器溢出了
    {
        TF0 = 0;      // 清除标志位
        time = 65535; // 溢出时，直接设置为最大计数值
    } else
        time = TH0 * 256 + TL0; // 未溢出时，读取其实际计数值
    // 将计数周期转换为实际时间（us）；超声波测得的是来回距离，除 2 得到单程距离的时间
    time = time * (12 / 11.0592) / 2;
    // time(s)*速度 = 距离，340 为声波在空气中的传播速度(m/s)
    result = (time / 1000000) * 340;
    if (result > 4.0) // 判断是否超出模块的测量范围
        result = 4.0; // 超出上限 4m 则设置为 4m
    else if (result < 0.02)
        result = 0.02; // 低于下限 0.02m 则设置为 0.02m
    // 以无符号整形数返回测量结果，乘 1000 使其单位变为毫米
    return (unsigned int)(result * 1000);
}
/*初始化超声波模块*/
void InitUltrasonic() {
    TMOD &= 0xf0;
    TMOD |= 0x01; // 设置 T0 为方式 1
    TH0 = 0;
    TL0 = 0;
    ET0 = 0;  // 关闭 T0 中断
    Trig = 0; // 拉低模块触发引脚
}
/******************************main.c 文件程序源代码*****************************/
uint16 flag = 0;     // 定义定时标志
uint8 LedBuff[7] = { // 数码管和 8 个独立 LED 的显示缓冲区，初值 0xFF 确保启动时都不亮
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const int8 __code LedChar[] = { // 数码管显示字符转换表
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};
const uint8 __code ledtab[8] = { // led 显示转换表
    0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00};
void ShowDistance();
/* 主函数 */
void main() {
    EA = 1;    // 使能总中断
    ENLED = 0; // 使能 U3，选择控制数码管
    ADDR3 = 1; // 因为需要动态改变 ADDR0-2 的值，所以不需要再初始化了
    TMOD &= 0x0f;
    TMOD |= 0x10; // 设置 T1 为模式 1
    TH1 = 0xFC;   // 为 T1 赋初值 0xFC67，定时 1ms
    TL1 = 0x67;
    ET1 = 1;          // 使能 T1 中断
    TR1 = 1;          // 启动 T1
    InitUltrasonic(); // 初始化超声波模块
    while (1) {
        if (flag == 1) // 每间隔 200ms 启动一次测距
        {
            flag = 0;
            ShowDistance();
        }
    }
}
/* 执行测距并显示结果 */
void ShowDistance() {
    int8 i;
    uint8 buff[5];
    uint16 distance; // 暂存距离值
    distance = GetDistance();
    buff[0] = distance % 10;
    buff[1] = distance / 10 % 10;
    buff[2] = distance / 100 % 10;
    buff[3] = distance / 1000 % 10;
    buff[4] = distance / 10000 % 10;
    for (i = 4; i >= 1; i--) // 整数部分高位的 0 转换为空白符
    {
        if (buff[i] == 0)
            LedBuff[i] = 0xff;
        else
            break;
    }
    for (; i >= 0; i--) // 有效数字转化为显示字符
        LedBuff[i] = LedChar[buff[i]];
    // 在从低起第 2 位添加小数点，表示显示数值的单位为厘米
    LedBuff[1] &= 0x7f;
    // 将 4m 的距离距离平均分为 8 个区间，用 LED 灯直观反应出来
    LedBuff[6] = ledtab[(distance - 1) / 500];
}
/* 定时器 1 中断服务函数 */
void InterruptTimer1() __interrupt(3) {
    static uint8 i = 0;    // LED 位选索引
    static uint16 cnt = 0; // 定义计数变量
    TH1 = 0xFC;            // 重新加载初值
    TL1 = 0x67;
    P0 = 0xFF;                     // 关闭所有段选位，显示消隐
    P1 = (P1 & 0xF8) | i;          // 位选索引值赋值到 P1 口低 3 位
    P0 = LedBuff[i];               // 缓冲区中索引位置的数据送到 P0 口
    if (i < (sizeof(LedBuff) - 1)) // 索引递增循环，遍历整个缓冲区
        i++;
    else
        i = 0;
    cnt++;
    if (cnt >= 200) // 定时 200ms
    {
        cnt = 0;  // 清零计数值
        flag = 1; // 设置定时标志为 1
    }
}