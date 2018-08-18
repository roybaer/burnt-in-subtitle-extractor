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

#ifndef YCBCR_IMG_HPP
#define YCBCR_IMG_HPP

#include <inttypes.h>
#include <iostream>
#include "color.hpp"

class ycbcr_img
{
public:
  int width;
  int height;
  int width_chroma;
  int height_chroma;
  uint8_t* y;
  uint8_t* cb;
  uint8_t* cr;
  ycbcr_img(int width, int height);
  ~ycbcr_img();
  color pixel(int xpos, int ypos);
};

std::istream& operator>>(std::istream& istr, ycbcr_img& img);

std::ostream& operator<<(std::ostream& ostr, ycbcr_img& img);

#endif
