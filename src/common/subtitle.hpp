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

#ifndef SUBTITLE_HPP
#define SUBTITLE_HPP

#include <string>
#include <iostream>

class subtitle
{
public:
  int start;
  int end;
  std::string text;
  subtitle();
  subtitle(int start, int end, std::string text);
  friend std::istream& operator>>(std::istream& istr, subtitle& s);
  friend std::ostream& operator<<(std::ostream& ostr, subtitle s);
};

#endif
