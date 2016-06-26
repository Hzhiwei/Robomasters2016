#ifndef __CONFIG_H
#define __CONFIG_H

/***************************************   ↓ 操作手参数  ↓ **********************************************/
#define VISIONDEBUG             1           //视觉debug模式
#define INFANTRY                2           //1萨摩       2阿拉斯加       3哈士奇

#if INFANTRY == 1               //萨摩参数↓

#define MAXWORKINGSPEED         850         //正常前进后退速度（最大900）
#define MAXSHIFTSPEED           1200        //正常前进后退速度
#define SNEAKSPEED              100         //潜行速度
#define QESPINPARAM             0.9F        //QE自旋系数（越大自旋越快，过大会发生底盘响应跟不上）
#define FRICMOTORWORKINGSPEED   230         //摩擦轮工作速度(230)
#define POKESTEPSPEED           -200        //拨弹电机步进速度（符号控制方向，不要动）
#define POKESTEPMINTIMECRACK    215         //步进最小时间间隔（ms）
#define MOUSESPINPARAM          16.0F       //鼠标自旋系数（越小自旋越快，过小会发生底盘响应跟不上）
#define MOUSEINTLIMIT           0.9F        //鼠标旋转速度上限，当鼠标旋转时底盘速度跟不上则减小此值
#define POKESTRUCKDEALLINES     200         //拨弹电机卡弹后反转处理线数

#elif INFANTRY == 2               //阿拉斯加参数↓

#define MAXWORKINGSPEED         850         //正常前进后退速度（最大900）
#define MAXSHIFTSPEED           1200        //正常前进后退速度
#define SNEAKSPEED              100         //潜行速度
#define QESPINPARAM             0.9F        //QE自旋系数（越大自旋越快，过大会发生底盘响应跟不上）
#define FRICMOTORWORKINGSPEED   230         //摩擦轮工作速度(230)
#define POKESTEPSPEED           -200        //拨弹电机步进速度（符号控制方向，不要动）
#define POKESTEPMINTIMECRACK    215         //步进最小时间间隔（ms）
#define MOUSESPINPARAM          16.0F       //鼠标自旋系数（越小自旋越快，过小会发生底盘响应跟不上）
#define MOUSEINTLIMIT           0.9F        //鼠标旋转速度上限，当鼠标旋转时底盘速度跟不上则减小此值
#define POKESTRUCKDEALLINES     200         //拨弹电机卡弹后反转处理线数

#elif INFANTRY == 3               //哈士奇参数↓

#define MAXWORKINGSPEED         850         //正常前进后退速度（最大900）
#define MAXSHIFTSPEED           1200        //正常前进后退速度
#define SNEAKSPEED              100         //潜行速度
#define QESPINPARAM             0.9F        //QE自旋系数（越大自旋越快，过大会发生底盘响应跟不上）
#define FRICMOTORWORKINGSPEED   230         //摩擦轮工作速度(230)
#define POKESTEPSPEED           -200        //拨弹电机步进速度（符号控制方向，不要动）
#define POKESTEPMINTIMECRACK    215         //步进最小时间间隔（ms）
#define MOUSESPINPARAM          24.0F       //鼠标自旋系数（越小自旋越快，过小会发生底盘响应跟不上）
#define MOUSEINTLIMIT           0.9F        //鼠标旋转速度上限，当鼠标旋转时底盘速度跟不上则减小此值
#define POKESTRUCKDEALLINES     200         //拨弹电机卡弹后反转处理线数

#endif
/***************************************   ↑ 操作手参数  ↑ **********************************************/



#define CANPORT             1           //选择can端口
//#define MOTORTYPE           1           //电机类型（不管）

#define DEBUGEMODE			1           //debuge模式
#define BELLBARKPeriod      100         //蜂鸣器序列时间长度（ms）
#define MONITORPERIOD       100         //监视器周期（ms）

#define BEBUG_MODE          1           //debug模式

#define ArtilleryShotCrack  2300        //炮发射间隔时间
#define SrtilleryPushTick   500         //炮气缸推出时间

#define GUNSpeed            23.0F       //枪子弹速度(m/s)
#define ARTILLERYSpeed      14.0F       //炮速度(m/s)

#define AFG                 9.87F       //acceleration of gravity重力加速度

#define ProgonsisCompensate 370         //预判时间补偿

#endif
