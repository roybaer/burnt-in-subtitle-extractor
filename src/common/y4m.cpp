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

#include "y4m.hpp"

/*ratio::ratio(uint16_t num, uint16_t den)
{
  this->num = num;
  this->den = den;
}*/

std::istream& operator>>(std::istream& istr, ratio& r)
{
  istr >> r.num;
  char c;
  istr >> c;
  istr >> r.den;
  return istr;
}

std::ostream& operator<<(std::ostream& ostr, ratio& r)
{
  ostr << r.num << ':' << r.den;
  return ostr;
}

double ratio::to_double()
{
  if (this->den == 0)
    return 1;
  return ((double) this->num) / this->den;
}

y4m_stream_header::y4m_stream_header()
{
  w = 0;
  h = 0;
  c = "420jpeg";
  i = '?';
  /*f = ratio(0, 0);
  i = ratio(0, 0);*/
}

std::istream& operator>>(std::istream& istr, y4m_stream_header& h)
{
  std::ios_base::fmtflags flags = istr.flags(std::ios_base::dec);
  std::string s;
  char c;
  istr >> s;
  if (s != "YUV4MPEG2")
  {
    istr.setstate(std::istream::failbit);
    istr.flags(flags);
    return istr;
  }
  istr >> c;
  while (c != '\n')
  {
    if (c != ' ')
    {
      istr.setstate(std::istream::failbit);
      istr.flags(flags);
      return istr;
    }
    istr >> c;
    switch (c)
    {
      case 'W':
        istr >> h.w;
        break;
      case 'H':
        istr >> h.h;
        break;
      case 'C':
        istr >> h.c;
        break;
      case 'I':
        istr >> h.i;
        break;
      case 'F':
        istr >> h.f;
        break;
      case 'A':
        istr >> h.a;
        break;
      case 'X':
        istr >> s;
        h.x.push_back(std::string(s));
        break;
      default:
        istr.setstate(std::istream::failbit);
        istr.flags(flags);
        return istr;
        break;
    }
    istr >> c;
  }
  return istr;
}

std::ostream& operator<<(std::ostream& ostr, y4m_stream_header& h)
{
  ostr << "YUV4MPEG2" << " W" << h.w << " H" << h.h << " C" << h.c << " I" << h.i << " F" << h.f << " A" << h.a;
  int i;
  for (i = 0; i < h.x.size(); i++)
    ostr << " X" << h.x[i];
  ostr << '\n';
  return ostr;
}
