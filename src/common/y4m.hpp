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

#ifndef Y4M_HPP
#define Y4M_HPP

#include <iostream>
#include <string>
#include <vector>
#include <inttypes.h>

class ratio
{
public:
  uint16_t num;
  uint16_t den;
  /*ratio(uint16_t num, uint16_t den);*/
  double to_double();
  friend std::istream& operator>>(std::istream& istr, ratio& r);
  friend std::ostream& operator<<(std::ostream& istr, ratio& r);
};

class y4m_stream_header
{
public:
  uint16_t w;
  uint16_t h;
  std::string c;
  char i;
  ratio f;
  ratio a;
  std::vector<std::string> x;
public:
  y4m_stream_header();
  friend std::istream& operator>>(std::istream& istr, y4m_stream_header& h);
  friend std::ostream& operator<<(std::ostream& ostr, y4m_stream_header& h);

};

#endif
