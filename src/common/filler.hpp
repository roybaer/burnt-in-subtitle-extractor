/*
 * This file is part of the burnt-in subtitle extractor.
 * Copyright (C) 2018  Benedikt Freisen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdint.h>

class filler
{
  uint8_t* data[32];
  uint8_t* alpha[32];
  //uint8_t* data_blurred[32];
  //uint8_t* alpha_blurred[32];
  int steps;
  int width[32];
  int height[32];
public:
  filler(int w, int h, uint8_t* img, uint8_t* mask);
  ~filler();
  void set_data0(uint8_t* data0);
  void fill();
  void shrink(int n);
  /*void blur(int n);*/
  void restore(int n);
};
