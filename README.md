由于CH32V203系列MCU功耗较低，习惯采用，所以我先尝试，待移植到其他STM32兼容型号  
ESP32-C3 GPIO太少，CPU任务可能干扰传感器数据读取，所以我想单独采用一个MCU  
目标是驱动[该PCB](https://github.com/XuHg-zjcn/bicycle_board)上连接的所有传感器、电源控制等外设  

目前能用定时器捕获霍尔传感器脉冲，硬件I2C驱动QMC6308磁力计

# 版权和许可证
Copyright (C) 2025  徐瑞骏  
本软件采用GNU通用公共许可证第三版或更新授权(COPYING.GPL3文件是该许可证的副本)  

本软件不提供任何保证  
