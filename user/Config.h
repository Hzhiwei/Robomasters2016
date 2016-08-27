

#ifndef __CONFIG_H
#define __CONFIG_H


#define DEBUGECONTROLRC             0           //debug模式遥控控制
#define USEESP8266orOLEDorOLED      0           //1 使用ESP8266返回裁判系统数据           0 使用oled显示装态

/***************************************   ↓ 操作手参数  ↓ **********************************************/
#define INFANTRY                    1           //1萨摩   2阿拉斯加   3哈士奇    4 金毛   5 狗蛋       6 小英雄       7 基地

#if INFANTRY == 1               //萨摩参数↓

    #define FRICMOTORWORKINGSPEED           275         //摩擦轮工作速度(170)
    #define POKESTRUCKDEALLINES             400         //拨弹电机卡弹后反转处理线数
    #define CHASSISMAXPOWERRATE             1.0F        //底盘限制极限功率（80W）比例(例如此值为0.9，则实际限制功率为0.9*80=72W）
    #define POKESTEPSPEED                   -250        //拨弹电机步进速度（符号控制方向，不要动）
    #define POKESTEPMINTIMECRACK            265         //步进最小时间间隔（ms）
    #define MAXWORKINGSPEED                 850         //正常运动速度（最大900）
    #define DEPOTABSPITCH                   -8.0F       //进补给站云台pitch绝对角度值（0为水平，-20 ~ 20）  
    #define SNEAKSPEED                      150         //潜行速度
    #define SWINGMODELIMIT                  30          //摇摆模式单边限幅(慎重修改，不得高于40，否则此模式可能失效)
    #define SWINGMODEOMEGA                  90          //摇摆模式角速度
    #define CIRCLEMODEOMEGA                 300         //圈圈模式角速度
    #define CIRCLEMODEDELAY                 10000       //圈圈模式受到伤害旋转时间  
    #define QESPINSPEED                     2.5F        //QE自旋速度
    #define MOUSESPINSPEED                  50.0F       //鼠标自旋速度   
    #define AUTOSHOTDISTANCE                3.2F        //自动射击距离限幅
    #define AUTOSHOTANGLE                   30          //自动射击角度限幅 
    #define BIGSAMPLEOFFSETXPARAM           0.005       //大符X轴鼠标偏移调节倍率
    #define BIGSAMPLEOFFSETYPARAM           0.005       //大符X轴鼠标偏移调节倍率
    #define ENCODERTOABSANGLEOFFSETPARAM    1         //由编码器加底盘陀螺仪计算云台绝对角度时会有蜜汁偏差，此参数用于矫正偏差
    
#elif INFANTRY == 2               //阿拉斯加参数↓

    #define FRICMOTORWORKINGSPEED           155         //摩擦轮工作速度(170)
    #define CHASSISMAXPOWERRATE             0.82F       //底盘限制极限功率（80W）比例(例如此值为0.9，则实际限制功率为0.9*80=72W）
    #define POKESTEPSPEED                   -250        //拨弹电机步进速度（符号控制方向，不要动）
    #define POKESTRUCKDEALLINES             400         //拨弹电机卡弹后反转处理线数
    #define POKESTEPMINTIMECRACK            265         //步进最小时间间隔（ms）
    #define MAXWORKINGSPEED                 850         //正常运动速度（最大900）
    #define DEPOTABSPITCH                   -8.0F       //进补给站云台pitch绝对角度值（0为水平，-20 ~ 20）  
    #define SNEAKSPEED                      50         //潜行速度
    #define SWINGMODELIMIT                  30          //摇摆模式单边限幅(慎重修改，不得高于40，否则此模式可能失效)
    #define SWINGMODEOMEGA                  90          //摇摆模式角速度
    #define CIRCLEMODEOMEGA                 300         //圈圈模式角速度
    #define CIRCLEMODEDELAY                 10000       //圈圈模式受到伤害旋转时间 
    #define QESPINSPEED                     1.0F        //QE自旋速度   
    #define MOUSESPINSPEED                  50.0F       //鼠标自旋速度 
    #define AUTOSHOTDISTANCE                3.2F        //自动射击距离限幅
    #define AUTOSHOTANGLE                   30          //自动射击角度限幅  
    #define BIGSAMPLEOFFSETXPARAM           0.005       //大符X轴鼠标偏移调节倍率
    #define BIGSAMPLEOFFSETYPARAM           0.005       //大符X轴鼠标偏移调节倍率
    #define ENCODERTOABSANGLEOFFSETPARAM    1         //由编码器加底盘陀螺仪计算云台绝对角度时会有蜜汁偏差，此参数用于矫正偏差 

#elif INFANTRY == 3               //哈士奇参数↓

    #define FRICMOTORWORKINGSPEED           264         //摩擦轮工作速度(170)
    #define POKESTRUCKDEALLINES             400         //拨弹电机卡弹后反转处理线数
    #define CHASSISMAXPOWERRATE             0.82F       //底盘限制极限功率（80W）比例(例如此值为0.9，则实际限制功率为0.9*80=72W）
    #define POKESTEPSPEED                   -250        //拨弹电机步进速度（符号控制方向，不要动）
    #define POKESTEPMINTIMECRACK            265         //步进最小时间间隔（ms）
    #define MAXWORKINGSPEED                 850         //正常运动速度（最大900）
    #define DEPOTABSPITCH                   -8.0F       //进补给站云台pitch绝对角度值（0为水平，-20 ~ 20）  
    #define SNEAKSPEED                      150         //潜行速度
    #define SWINGMODELIMIT                  30          //摇摆模式单边限幅(慎重修改，不得高于40，否则此模式可能失效)
    #define SWINGMODEOMEGA                  90          //摇摆模式角速度
    #define CIRCLEMODEOMEGA                 300         //圈圈模式角速度
    #define CIRCLEMODEDELAY                 10000       //圈圈模式受到伤害旋转时间  
    #define QESPINSPEED                     1.0F        //QE自旋速度   
    #define MOUSESPINSPEED                  50.0F       //鼠标自旋速度 
    #define AUTOSHOTDISTANCE                3.2F        //自动射击距离限幅
    #define AUTOSHOTANGLE                   30          //自动射击角度限幅  
    #define BIGSAMPLEOFFSETXPARAM           0.005       //大符X轴鼠标偏移调节倍率
    #define BIGSAMPLEOFFSETYPARAM           0.005       //大符X轴鼠标偏移调节倍率
    #define ENCODERTOABSANGLEOFFSETPARAM    1         //由编码器加底盘陀螺仪计算云台绝对角度时会有蜜汁偏差，此参数用于矫正偏差

#elif INFANTRY == 4               //金毛

    #define FRICMOTORWORKINGSPEED           250         //摩擦轮工作速度(230)
    #define POKESTRUCKDEALLINES             400          //拨弹电机卡弹后反转处理线数
    #define CHASSISMAXPOWERRATE             0.82F       //底盘限制极限功率（80W）比例(例如此值为0.9，则实际限制功率为0.9*80=72W）
    #define POKESTEPSPEED                   -250        //拨弹电机步进速度（符号控制方向，不要动）
    #define POKESTEPMINTIMECRACK            265         //步进最小时间间隔（ms）
    #define MAXWORKINGSPEED                 850         //正常运动速度（最大900）
    #define DEPOTABSPITCH                   -8.0F       //进补给站云台pitch绝对角度值（0为水平，-20 ~ 20）  
    #define SNEAKSPEED                      150         //潜行速度
    #define SWINGMODELIMIT                  30          //摇摆模式单边限幅(慎重修改，不得高于40，否则此模式可能失效)
    #define SWINGMODEOMEGA                  90          //摇摆模式角速度
    #define CIRCLEMODEOMEGA                 300         //圈圈模式角速度
    #define CIRCLEMODEDELAY                 10000       //圈圈模式受到伤害旋转时间  
    #define QESPINSPEED                     1.0F        //QE自旋速度  
    #define MOUSESPINSPEED                  50.0F       //鼠标自旋速度 
    #define AUTOSHOTDISTANCE                3.2F        //自动射击距离限幅
    #define AUTOSHOTANGLE                   30          //自动射击角度限幅   
    #define BIGSAMPLEOFFSETXPARAM           0.005       //大符X轴鼠标偏移调节倍率
    #define BIGSAMPLEOFFSETYPARAM           0.005       //大符X轴鼠标偏移调节倍率  
    #define ENCODERTOABSANGLEOFFSETPARAM    1         //由编码器加底盘陀螺仪计算云台绝对角度时会有蜜汁偏差，此参数用于矫正偏差


#elif INFANTRY == 5               //狗蛋

    #define FRICMOTORWORKINGSPEED           264         //摩擦轮工作速度(230)
    #define POKESTRUCKDEALLINES             400          //拨弹电机卡弹后反转处理线数
    #define CHASSISMAXPOWERRATE             0.82F       //底盘限制极限功率（80W）比例(例如此值为0.9，则实际限制功率为0.9*80=72W）
    #define POKESTEPSPEED                   -250        //拨弹电机步进速度（符号控制方向，不要动）
    #define POKESTEPMINTIMECRACK            265         //步进最小时间间隔（ms）
    #define MAXWORKINGSPEED                 850         //正常运动速度（最大900）
    #define DEPOTABSPITCH                   -8.0F       //进补给站云台pitch绝对角度值（0为水平，-20 ~ 20）  
    #define SNEAKSPEED                      150         //潜行速度
    #define SWINGMODELIMIT                  30          //摇摆模式单边限幅(慎重修改，不得高于40，否则此模式可能失效)
    #define SWINGMODEOMEGA                  90          //摇摆模式角速度
    #define CIRCLEMODEOMEGA                 500         //圈圈模式角速度
    #define CIRCLEMODEDELAY                 10000       //圈圈模式受到伤害旋转时间  
    #define QESPINSPEED                     1.0F        //QE自旋速度  
    #define MOUSESPINSPEED                  50.0F       //鼠标自旋速度 
    #define AUTOSHOTDISTANCE                3.2F        //自动射击距离限幅
    #define AUTOSHOTANGLE                   30          //自动射击角度限幅  
    #define BIGSAMPLEOFFSETXPARAM           0.005       //大符X轴鼠标偏移调节倍率
    #define BIGSAMPLEOFFSETYPARAM           0.005       //大符X轴鼠标偏移调节倍率 
    #define ENCODERTOABSANGLEOFFSETPARAM    0.7375         //由编码器加底盘陀螺仪计算云台绝对角度时会有蜜汁偏差，此参数用于矫正偏差   

#elif INFANTRY == 6               //比特参数↓

    #define FRICMOTORWORKINGSPEED           260         //摩擦轮工作速度(230)
    #define POKESTRUCKDEALLINES             400         //拨弹电机卡弹后反转处理线数
    #define CHASSISMAXPOWERRATE             0.82F       //底盘限制极限功率（80W）比例(例如此值为0.9，则实际限制功率为0.9*80=72W）
    #define POKESTEPSPEED                   -250        //拨弹电机步进速度（符号控制方向，不要动）
    #define POKESTEPMINTIMECRACK            265         //步进最小时间间隔（ms）
    #define MAXWORKINGSPEED                 850         //正常运动速度（最大900）
    #define DEPOTABSPITCH                   -8.0F       //进补给站云台pitch绝对角度值（0为水平，-20 ~ 20）  
    #define SNEAKSPEED                      150         //潜行速度
    #define SWINGMODELIMIT                  30          //摇摆模式单边限幅(慎重修改，不得高于40，否则此模式可能失效)
    #define SWINGMODEOMEGA                  90          //摇摆模式角速度
    #define CIRCLEMODEOMEGA                 300         //圈圈模式角速度
    #define CIRCLEMODEDELAY                 10000       //圈圈模式受到伤害旋转时间   
    #define ARTILLERYFRICSPEED              5000        //炮摩擦轮速度    
    #define ARTILLERYSHOTCRACK              220         //炮发射时间间隔
    #define QESPINSPEED                     1.0F        //QE自旋速度  
    #define MOUSESPINSPEED                  50.0F       //鼠标自旋速度    

#elif INFANTRY == 7               //基地参数↓

    #define CHASSISMAXPOWERRATE             0.9F        //底盘限制极限功率（80W）比例(例如此值为0.9，则实际限制功率为0.9*80=72W）
    #define FRICMOTORWORKINGSPEED           275         //摩擦轮工作速度(230)
    #define POKESTRUCKDEALLINES             400         //拨弹电机卡弹后反转处理线数
    #define POKESTEPMINTIMECRACK            265         //步进最小时间间隔（ms）
    #define POKESTEPSPEED                   -250        //拨弹电机步进速度（符号控制方向，不要动）
    #define MOUSESPINSPEED                  50.0F       //鼠标自旋速度   
    #define AUTOSHOTDISTANCE                4.0F        //自动射击距离限幅
    #define AUTOSHOTANGLE                   30          //自动射击角度限幅
    #define SHOOTUSEOFFTICK                 1600        //连续发射失败时间长于此数据认为子弹用光
    #define SHOOTUSEOFFNUM                  150         //已发射子弹多于此数量可进行use up判断

#endif
/***************************************   ↑ 操作手参数  ↑ **********************************************/

#if INFANTRY == 1       //萨摩
    
    #define PITCHMOTORCURRENTPLUSDIR        1           //pitch轴云台电机电流方向为正时转动方向     1 抬头         0 低头
    #define YAWMOTORCURRENTPLUSDIR          1           //yaw轴云台电机电流方向为正时转动方向       1 逆时针       0 顺时针
    #define PITCHMOTORENCODERPLUSEDIR       1           //pitch轴云台电机编码器增加时转动方向       1 抬头         0 低头
    #define YAWMOTORENCODERPLUSEDIR         1           //yaw轴云台电机编码器增加时转动方向         1 逆时针       0 顺时针
    #define CANPORT                         1           //选择can端口
    #define MOTORTYPE                       1           //电机种类                                 1 3510        0 35
    #define FRICTYPE                        0           //摩擦轮种类                               1 3510        0 普通无刷
    #define AUTOSUPPORT                     1           //自动支持（大符/自动射击）                 1 支持        0 不支持  
    #define AUTOBIGSAMPLESUPPORT            0           //自动大符支持                             1 支持        0 不支持
    #define AUTOSHOOTSUPPORT                0           //自动射击支持                             1 支持        0 不支持    
    
#elif INFANTRY == 2     //阿拉斯加
    
    #define PITCHMOTORCURRENTPLUSDIR        1           //pitch轴云台电机电流方向为正时转动方向     1 抬头         0 低头
    #define YAWMOTORCURRENTPLUSDIR          1           //yaw轴云台电机电流方向为正时转动方向       1 逆时针       0 顺时针
    #define PITCHMOTORENCODERPLUSEDIR       1           //pitch轴云台电机编码器增加时转动方向       1 抬头         0 低头
    #define YAWMOTORENCODERPLUSEDIR         1           //yaw轴云台电机编码器增加时转动方向         1 逆时针       0 顺时针
    #define CANPORT                         1           //选择can端口
    #define MOTORTYPE                       1           //电机种类                                 1 3510        0 35
    #define FRICTYPE                        0           //摩擦轮种类                               1 3510        0 普通无刷
    #define AUTOSUPPORT                     1           //自动支持（大符/自动射击）                 1 支持        0 不支持  
    #define AUTOBIGSAMPLESUPPORT            1           //自动大符支持                             1 支持        0 不支持
    #define AUTOSHOOTSUPPORT                0           //自动射击支持                             1 支持        0 不支持  

#elif INFANTRY == 3     //哈士奇
    
    #define PITCHMOTORCURRENTPLUSDIR        0           //pitch轴云台电机电流方向为正时转动方向     1 抬头         0 低头
    #define YAWMOTORCURRENTPLUSDIR          1           //yaw轴云台电机电流方向为正时转动方向       1 逆时针       0 顺时针
    #define PITCHMOTORENCODERPLUSEDIR       1           //pitch轴云台电机编码器增加时转动方向       1 抬头         0 低头
    #define YAWMOTORENCODERPLUSEDIR         1           //yaw轴云台电机编码器增加时转动方向         1 逆时针       0 顺时针
    #define CANPORT                         1           //选择can端口
    #define MOTORTYPE                       1           //电机种类                                 1 3510        0 35
    #define FRICTYPE                        0           //摩擦轮种类                               1 3510        0 普通无刷
    #define AUTOSUPPORT                     1           //自动支持（大符/自动射击）                 1 支持        0 不支持 
    #define AUTOBIGSAMPLESUPPORT            1           //自动大符支持                             1 支持        0 不支持
    #define AUTOSHOOTSUPPORT                0           //自动射击支持                             1 支持        0 不支持   

#elif INFANTRY == 4     //金毛
    
    #define PITCHMOTORCURRENTPLUSDIR        0           //pitch轴云台电机电流方向为正时转动方向     1 抬头         0 低头
    #define YAWMOTORCURRENTPLUSDIR          0           //yaw轴云台电机电流方向为正时转动方向       1 逆时针       0 顺时针
    #define PITCHMOTORENCODERPLUSEDIR       1           //pitch轴云台电机编码器增加时转动方向       1 抬头         0 低头
    #define YAWMOTORENCODERPLUSEDIR         1           //yaw轴云台电机编码器增加时转动方向         1 逆时针       0 顺时针
    #define CANPORT                         1           //选择can端口
    #define MOTORTYPE                       0          //电机种类                                 1 3510        0 35
    #define FRICTYPE                        0           //摩擦轮种类                               1 3510        0 普通无刷
    #define AUTOSUPPORT                     0           //自动支持（大符/自动射击）                 1 支持        0 不支持  
    #define AUTOBIGSAMPLESUPPORT            0           //自动大符支持                             1 支持        0 不支持
    #define AUTOSHOOTSUPPORT                0           //自动射击支持                             1 支持        0 不支持  

#elif INFANTRY == 5     //狗蛋
    
    #define PITCHMOTORCURRENTPLUSDIR        0           //pitch轴云台电机电流方向为正时转动方向     1 抬头         0 低头
    #define YAWMOTORCURRENTPLUSDIR          1           //yaw轴云台电机电流方向为正时转动方向       1 逆时针       0 顺时针
    #define PITCHMOTORENCODERPLUSEDIR       1           //pitch轴云台电机编码器增加时转动方向       1 抬头         0 低头
    #define YAWMOTORENCODERPLUSEDIR         1           //yaw轴云台电机编码器增加时转动方向         1 逆时针       0 顺时针
    #define CANPORT                         1           //选择can端口
    #define MOTORTYPE                       1           //电机种类                                 1 3510        0 35
    #define FRICTYPE                        0           //摩擦轮种类                               1 3510        0 普通无刷
    #define AUTOSUPPORT                     1           //自动支持（大符/自动射击）                 1 支持        0 不支持 
    #define AUTOBIGSAMPLESUPPORT            1           //自动大符支持                             1 支持        0 不支持
    #define AUTOSHOOTSUPPORT                0           //自动射击支持                             1 支持        0 不支持   


#elif INFANTRY == 6     //小英雄
    
    #define PITCHMOTORCURRENTPLUSDIR        0           //pitch轴云台电机电流方向为正时转动方向     1 抬头         0 低头
    #define YAWMOTORCURRENTPLUSDIR          1           //yaw轴云台电机电流方向为正时转动方向       1 逆时针       0 顺时针
    #define PITCHMOTORENCODERPLUSEDIR       0           //pitch轴云台电机编码器增加时转动方向       1 抬头         0 低头
    #define YAWMOTORENCODERPLUSEDIR         1           //yaw轴云台电机编码器增加时转动方向         1 逆时针       0 顺时针
    #define CANPORT                         1           //选择can端口
    #define MOTORTYPE                       0           //电机种类                                 1 3510        0 35
    #define FRICTYPE                        1           //摩擦轮种类                               1 3510        0 普通无刷
    #define AUTOSUPPORT                     0           //自动支持（大符/自动射击）                 1 支持        0 不支持 
    #define AUTOBIGSAMPLESUPPORT            0           //自动大符支持                             1 支持        0 不支持
    #define AUTOSHOOTSUPPORT                0           //自动射击支持                             1 支持        0 不支持   


#elif INFANTRY == 7     //基地
    
    #define PITCHMOTORCURRENTPLUSDIR        1           //pitch轴云台电机电流方向为正时转动方向     1 抬头         0 低头
    #define YAWMOTORCURRENTPLUSDIR          1           //yaw轴云台电机电流方向为正时转动方向       1 逆时针       0 顺时针
    #define PITCHMOTORENCODERPLUSEDIR       1           //pitch轴云台电机编码器增加时转动方向       1 抬头         0 低头
    #define YAWMOTORENCODERPLUSEDIR         1           //yaw轴云台电机编码器增加时转动方向         1 逆时针       0 顺时针
    #define CANPORT                         1           //选择can端口
    #define MOTORTYPE                       0           //电机种类                                 1 3510        0 35
    #define FRICTYPE                        0           //摩擦轮种类                               1 3510        0 普通无刷 

#endif

#define BELLBARKPeriod      100         //蜂鸣器序列时间长度（ms）
#define MONITORPERIOD       100         //监视器周期（ms）
#define GUNSpeed            23.5F       //枪子弹速度(m/s)
#define ACCofGravity        9.87F       //acceleration of gravity重力加速度

#endif


