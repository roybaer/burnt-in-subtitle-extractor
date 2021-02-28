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

#include "color.hpp"

color::color()
{

}

color::color(uint8_t y, uint8_t cb, uint8_t cr)
{
  this->y = y;
  this->cb = cb;
  this->cr = cr;
}

bool color::similar_to(color c, color d)
{
  return ((y >= c.y - d.y) && (y <= c.y + d.y) && (cb >= c.cb - d.cb) && (cb <= c.cb + d.cb) && (cr >= c.cr - d.cr) && (cr <= c.cr + d.cr));
}

std::istream& operator>>(std::istream& istr, color& c)
{
  char ch;
  int y, cb, cr;
  istr >> y >> ch >> cb >> ch >> cr;
  c.y = y;
  c.cb = cb;
  c.cr = cr;
  return istr;
}
