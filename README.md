# 自行车xx32固件
由于CH32V203系列MCU功耗较低，习惯采用，所以我先尝试，待移植到其他STM32兼容型号  
ESP32-C3 GPIO太少，CPU任务可能干扰传感器数据读取，所以我想单独采用一个MCU  
目标是驱动[该PCB](https://github.com/XuHg-zjcn/bicycle_board)上连接的所有传感器、电源控制等外设  
