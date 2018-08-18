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

#include <fstream>
#include <string.h>

#include "pgm.hpp"

using namespace std;

void read_pgm(const char* name, uint8_t*& img, int& width, int& height)
{
  ifstream f;
  f.open(name);
  char sig[10];
  int maxval;
  f.getline(sig, 9);
  if (strcmp(sig, "P5") != 0)
  {
    width = 0;
    height = 0;
    return;
  }
  f >> width >> height >> maxval;
  f.ignore(1);
  img = new uint8_t[width * height];
  f.read((char*)img, width * height);
  f.close();
}

void write_pgm(uint8_t* img, int width, int height, const char* name)
{
  ofstream f;
  f.open(name);
  f << "P5\n" << width << " " << height << "\n255\n";
  f.write((char*)img, width * height);
  f.close();
}
