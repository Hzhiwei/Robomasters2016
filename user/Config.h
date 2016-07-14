#ifndef __CONFIG_H
#define __CONFIG_H

/***************************************   ↓ 操作手参数  ↓ **********************************************/
#define VISIONDEBUG             0           //视觉debug模式
#define INFANTRY                2           //1萨摩       2阿拉斯加       3哈士奇        4 边牧（又金毛）     5 狗蛋  6 英雄
//#define INFANTRYTYPE          1           //车类型  1 A车，且上电默认控制车(C)   2 AB车，上电默认非控制车(F)      3 独立车

#if INFANTRY == 1               //萨摩参数↓

#define MAXWORKINGSPEED         850         //正常前进后退速度（最大900）
#define MAXSHIFTSPEED           1200        //正常平移速度
#define SNEAKSPEED              100         //潜行速度
#define QESPINPARAM             0.9F        //QE自旋系数（越大自旋越快，过大会发生底盘响应跟不上）
#define FRICMOTORWORKINGSPEED   350         //摩擦轮工作速度(230)
#define POKESTEPSPEED           -250        //拨弹电机步进速度（符号控制方向，不要动）
#define POKESTEPMINTIMECRACK    220         //步进最小时间间隔（ms）
#define MOUSESPINPARAM          16.0F       //鼠标自旋系数（越小自旋越快，过小会发生底盘响应跟不上）
#define MOUSEPITCHPARAM         20.0F       //鼠标pitch控制系数（越小速度越快）
#define POKESTRUCKDEALLINES     300         //拨弹电机卡弹后反转处理线数
#define LOWSPEEDOVERRIDE        0.65F       //底盘低速为全速的多少倍（0-1）
#define DEPOTABSPITCH           -120        //进补给站云台pitch编码器值（0为水平，-200 ~ 200） 
#define CHASSISMAXPOWERRATE     0.82F       //底盘限制极限功率（80W）比例(例如此值为0.9，则实际限制功率为0.9*80=72W）
#define AUTOSHOTTICKCRACK       400         //全自动射击时间间隔

#elif INFANTRY == 2               //阿拉斯加参数↓

#define MAXWORKINGSPEED         850         //正常前进后退速度（最大900）
#define MAXSHIFTSPEED           1200        //正常平移速度
#define SNEAKSPEED              100         //潜行速度
#define QESPINPARAM             0.9F        //QE自旋系数（越大自旋越快，过大会发生底盘响应跟不上）
#define FRICMOTORWORKINGSPEED   260         //摩擦轮工作速度(230)
#define POKESTEPSPEED           -250        //拨弹电机步进速度（符号控制方向，不要动）
#define POKESTEPMINTIMECRACK    200         //步进最小时间间隔（ms）
#define MOUSESPINPARAM          16.0F       //鼠标自旋系数（越小自旋越快，过小会发生底盘响应跟不上）
#define MOUSEPITCHPARAM         20.0F       //鼠标pitch控制系数（越小速度越快）
#define POKESTRUCKDEALLINES     300         //拨弹电机卡弹后反转处理线数
#define LOWSPEEDOVERRIDE        0.65F       //底盘低速为全速的多少倍（0-1）
#define DEPOTABSPITCH           -120        //进补给站云台pitch编码器值（0为水平，-200 ~ 200）  
#define CHASSISMAXPOWERRATE     0.82F       //底盘限制极限功率（80W        ）比例(例如此值为0.9，则实际限制功率为0.9*80=72W）
#define AUTOSHOTTICKCRACK       400         //全自动射击时间间隔

#elif INFANTRY == 3               //哈士奇参数↓（射速已调节）

#define MAXWORKINGSPEED         850         //正常前进后退速度（最大900）
#define MAXSHIFTSPEED           1200        //正常平移速度
#define SNEAKSPEED              100         //潜行速度
#define QESPINPARAM             0.9F        //QE自旋系数（越大自旋越快，过大会发生底盘响应跟不上）
#define FRICMOTORWORKINGSPEED   285         //摩擦轮工作速度(230)
#define POKESTEPSPEED           -250        //拨弹电机步进速度（符号控制方向，不要动）
#define POKESTEPMINTIMECRACK    215         //步进最小时间间隔（ms）
#define MOUSESPINPARAM          20.0F       //鼠标自旋系数（越小自旋越快，过小会发生底盘响应跟不上）
#define MOUSEPITCHPARAM         15.0F       //鼠标pitch控制系数（越小速度越快）
#define POKESTRUCKDEALLINES     300         //拨弹电机卡弹后反转处理线数
#define LOWSPEEDOVERRIDE        0.65F       //底盘低速为全速的多少倍（0-1）
#define DEPOTABSPITCH           -120        //进补给站云台pitch编码器值（0为水平，-200 ~ 200） 
#define CHASSISMAXPOWERRATE     0.82F       //底盘限制极限功率（80W）比例 (例如此值为0.9，则实际限制功率为0.9*80=72W）
#define AUTOSHOTTICKCRACK       400         //全自动射击时间间隔

#elif INFANTRY == 4               //边牧（射速已调节）

#define MAXWORKINGSPEED         850         //正常前进后退速度（最大900）
#define MAXSHIFTSPEED           1200        //正常平移速度
#define SNEAKSPEED              100         //潜行速度
#define QESPINPARAM             0.9F        //QE自旋系数（越大自旋越快，过大会发生底盘响应跟不上）
#define FRICMOTORWORKINGSPEED   318         //摩擦轮工作速度(230)
#define POKESTEPSPEED           -250        //拨弹电机步进速度（符号控制方向，不要动）
#define POKESTEPMINTIMECRACK    215         //步进最小时间间隔（ms）
#define MOUSESPINPARAM          10.0F       //鼠标自旋系数（越小自旋越快，过小会发生底盘响应跟不上）
#define MOUSEPITCHPARAM         10.0F       //鼠标pitch控制系数（越小速度越快）
#define POKESTRUCKDEALLINES     300         //拨弹电机卡弹后反转处理线数
#define LOWSPEEDOVERRIDE        0.65F       //底盘低速为全速的多少倍（0-1）
#define DEPOTABSPITCH           -120        //进补给站云台pitch编码器值（0为水平，-200 ~ 200） 
#define CHASSISMAXPOWERRATE     0.82F       //底盘限制极限功率（80W）比例 (例如此值为0.9，则实际限制功率为0.9*80=72W）
#define AUTOSHOTTICKCRACK       400         //全自动射击时间间隔

#elif INFANTRY == 5               //狗蛋

#define MAXWORKINGSPEED         850         //正常前进后退速度（最大900）
#define MAXSHIFTSPEED           1200        //正常平移速度
#define SNEAKSPEED              100         //潜行速度
#define QESPINPARAM             0.9F        //QE自旋系数（越大自旋越快，过大会发生底盘响应跟不上）
#define FRICMOTORWORKINGSPEED   305         //摩擦轮工作速度(230)
#define POKESTEPSPEED           -250        //拨弹电机步进速度（符号控制方向，不要动）
#define POKESTEPMINTIMECRACK    215         //步进最小时间间隔（ms）
#define MOUSESPINPARAM          20.0F       //鼠标自旋系数（越小自旋越快，过小会发生底盘响应跟不上）
#define MOUSEPITCHPARAM         10.0F       //鼠标pitch控制系数（越小速度越快）
#define POKESTRUCKDEALLINES     300         //拨弹电机卡弹后反转处理线数
#define LOWSPEEDOVERRIDE        0.65F       //底盘低速为全速的多少倍（0-1）
#define DEPOTABSPITCH           -120        //进补给站云台pitch编码器值（0为水平，-200 ~ 200） 
#define CHASSISMAXPOWERRATE     0.82F       //底盘限制极限功率（80W）比例 (例如此值为0.9，则实际限制功率为0.9*80=72W）
#define AUTOSHOTTICKCRACK       400         //全自动射击时间间隔

#elif INFANTRY == 6               //英雄

#define MAXWORKINGSPEED         850         //正常前进后退速度（最大900）
#define MAXSHIFTSPEED           1200        //正常平移速度
#define SNEAKSPEED              100         //潜行速度
#define QESPINPARAM             0.9F        //QE自旋系数（越大自旋越快，过大会发生底盘响应跟不上）
#define FRICMOTORWORKINGSPEED   305         //摩擦轮工作速度(230)
#define POKESTEPSPEED           -250        //拨弹电机步进速度（符号控制方向，不要动）
#define POKESTEPMINTIMECRACK    215         //步进最小时间间隔（ms）
#define MOUSESPINPARAM          20.0F       //鼠标自旋系数（越小自旋越快，过小会发生底盘响应跟不上）
#define MOUSEPITCHPARAM         10.0F       //鼠标pitch控制系数（越小速度越快）
#define POKESTRUCKDEALLINES     300         //拨弹电机卡弹后反转处理线数
#define LOWSPEEDOVERRIDE        0.1F       //底盘低速为全速的多少倍（0-1）
#define DEPOTABSPITCH           -120        //进补给站云台pitch编码器值（0为水平，-200 ~ 200） 
#define CHASSISMAXPOWERRATE     0.82F       //底盘限制极限功率（160W）比例 (例如此值为0.9，则实际限制功率为0.9*160=144W）
#define AUTOSHOTTICKCRACK       400         //全自动射击时间间隔

#endif
/***************************************   ↑ 操作手参数  ↑ **********************************************/



#define CANPORT             1           //选择can端口
//#define MOTORTYPE         1           //电机类型（不管）

#define DEBUGEMODE			1           //debuge模式
#define BELLBARKPeriod      100         //蜂鸣器序列时间长度（ms）
#define MONITORPERIOD       100         //监视器周期（ms）

#define BEBUG_MODE          1           //debug模式

#define ArtilleryShotCrack  2300        //炮发射间隔时间
#define SrtilleryPushTick   500         //炮气缸推出时间

#define GUNSpeed            23.5F       //枪子弹速度(m/s)
#define ARTILLERYSpeed      14.0F       //炮速度(m/s)

#define AFG                 9.87F       //acceleration of gravity重力加速度

#define ProgonsisCompensate 370         //预判时间补偿

#endif
