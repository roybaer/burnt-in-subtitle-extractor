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

#include "ycbcr_img.hpp"

ycbcr_img::ycbcr_img(int width, int height)
{
  this->width = width;
  this->height = height;
  this->width_chroma = this->width / 2;
  this->height_chroma = this->height / 2;
  this->y = new uint8_t[this->width * this->height];
  this->cb = new uint8_t[this->width_chroma * this->height_chroma];
  this->cr = new uint8_t[this->width_chroma * this->height_chroma];
}

ycbcr_img::~ycbcr_img()
{
  delete [] y;
  delete [] cb;
  delete [] cr;
}

std::istream& operator>>(std::istream& istr, ycbcr_img& img)
{
  istr.read((char*)img.y, img.width * img.height);
  istr.read((char*)img.cb, img.width_chroma * img.height_chroma);
  istr.read((char*)img.cr, img.width_chroma * img.height_chroma);
  return istr;
}

std::ostream& operator<<(std::ostream& ostr, ycbcr_img& img)
{
  ostr.write((char*)img.y, img.width * img.height);
  ostr.write((char*)img.cb, img.width_chroma * img.height_chroma);
  ostr.write((char*)img.cr, img.width_chroma * img.height_chroma);
  return ostr;
}

color ycbcr_img::pixel(int xpos, int ypos)
{
  return color(y[ypos * width + xpos], cb[(ypos / 2) * width_chroma + (xpos / 2)], cr[(ypos / 2) * width_chroma + (xpos / 2)]);
}
