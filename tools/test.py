#!/usr/bin/env python3
#########################################################################
#  串口读取QMC6308数据并显示
#  Copyright (C) 2025  Xu Ruijun
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#########################################################################
import glob
import serial
import struct

devs = glob.glob('/dev/ttyUSB*')
if len(devs) == 0:
    raise FileExistsError("can't find device")
dev = devs[0]
ser = serial.Serial(dev, 115200)

while True:
    data = ser.read(10)
    if len(data) == 10 and data[:4] == b'MAGd':
        x, y, z = struct.unpack('hhh', data[4:])
        print(x, y, z)
