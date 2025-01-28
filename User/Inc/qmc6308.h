/*************************************************************************
 *  QMC6308驱动
 *  Copyright (C) 2025  Xu Ruijun
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *************************************************************************/
#ifndef QMC6308_H
#define QMC6308_H

#include <stdint.h>

typedef struct{
  int16_t x;
  int16_t y;
  int16_t z;
}QMC6308_Data;

void QMC6308_ReadData(QMC6308_Data *data);
void QMC6308_SelfTest();
int QMC6308_Init();

#endif
