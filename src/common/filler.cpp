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

#include "filler.hpp"

filler::filler(int w, int h, uint8_t* img, uint8_t* mask)
{
  int i;
  width[0] = w;
  height[0] = h;
  data[0] = img;
  alpha[0] = mask;
  //data_blurred[0] = new uint8_t[width[0] * height[0]];
  //alpha_blurred[0] = new uint8_t[width[0] * height[0]];
  for (i = 1; i < 32; i++)
  {
    width[i] = (width[i - 1] + 1) / 2;
    height[i] = (height[i - 1] + 1) / 2;
    data[i] = new uint8_t[width[i] * height[i]];
    alpha[i] = new uint8_t[width[i] * height[i]];
    //data_blurred[i] = new uint8_t[width[i] * height[i]];
    //alpha_blurred[i] = new uint8_t[width[i] * height[i]];
  }
}

filler::~filler()
{
  //delete [] data_blurred[0];
  //delete [] alpha_blurred[0];
  int i;
  for (i = 1; i < 32; i++)
  {
    delete [] data[i];
    delete [] alpha[i];
    //delete [] data_blurred[i];
    //delete [] alpha_blurred[i];
  }
}

void filler::set_data0(uint8_t* data0)
{
  data[0] = data0;
}


int min(int a, int b)
{
  return (a < b) ? a : b;
}

int bounds(int a, int b, int c)
{
  if (a < b)
    return b;
  if (a > c)
    return c;
  return a;
}

int index(int x, int y, int width, int height)
{
  return width * bounds(y, 0, height - 1) + bounds(x, 0, width - 1);
}

/*void filler::blur(int n)
{
  int x, y;
  for (y = 0; y < height[n]; y++)
  {
    for (x = 0; x < width[n]; x++)
    {
      int alphasumme = 
        alpha[n][index(x - 1, y - 1, width[n], height[n])] + 
        alpha[n][index(x,     y - 1, width[n], height[n])] + 
        alpha[n][index(x + 1, y - 1, width[n], height[n])] +
        alpha[n][index(x - 1, y,     width[n], height[n])] + 
        alpha[n][index(x,     y,     width[n], height[n])] + 
        alpha[n][index(x + 1, y,     width[n], height[n])] +
        alpha[n][index(x - 1, y + 1, width[n], height[n])] + 
        alpha[n][index(x,     y + 1, width[n], height[n])] + 
        alpha[n][index(x + 1, y + 1, width[n], height[n])];
      if (alphasumme == 0)
      {
        alpha_blurred[n][index(x, y, width[n], height[n])] = 0;
        data_blurred[n][index(x, y, width[n], height[n])] = 0;
      }
      else
      {
        int summe =
          data[n][index(x - 1, y - 1, width[n], height[n])] * alpha[n][index(x - 1, y - 1, width[n], height[n])] + 
          data[n][index(x,     y - 1, width[n], height[n])] * alpha[n][index(x,     y - 1, width[n], height[n])] + 
          data[n][index(x + 1, y - 1, width[n], height[n])] * alpha[n][index(x + 1, y - 1, width[n], height[n])] + 
          data[n][index(x - 1, y,     width[n], height[n])] * alpha[n][index(x - 1, y,     width[n], height[n])] + 
          data[n][index(x,     y,     width[n], height[n])] * alpha[n][index(x,     y,     width[n], height[n])] + 
          data[n][index(x + 1, y,     width[n], height[n])] * alpha[n][index(x + 1, y,     width[n], height[n])] + 
          data[n][index(x - 1, y + 1, width[n], height[n])] * alpha[n][index(x - 1, y + 1, width[n], height[n])] + 
          data[n][index(x,     y + 1, width[n], height[n])] * alpha[n][index(x,     y + 1, width[n], height[n])] + 
          data[n][index(x + 1, y + 1, width[n], height[n])] * alpha[n][index(x + 1, y + 1, width[n], height[n])];
        alpha_blurred[n][index(x, y, width[n], height[n])] = min(255, alphasumme / 9);
        data_blurred[n][index(x, y, width[n], height[n])] = summe / alphasumme;
      }
    }
  }
}*/


void filler::shrink(int n)
{
  int x, y;
  for (y = 0; y < height[n + 1]; y++)
  {
    for (x = 0; x < width[n + 1]; x++)
    {
      int summe =
        data[n][index(x * 2, y * 2, width[n], height[n])] * alpha[n][index(x * 2, y * 2, width[n], height[n])] + 
        data[n][index(x * 2 + 1, y * 2, width[n], height[n])] * alpha[n][index(x * 2 + 1, y * 2, width[n], height[n])] + 
        data[n][index(x * 2, y * 2 + 1, width[n], height[n])] * alpha[n][index(x * 2, y * 2 + 1, width[n], height[n])] + 
        data[n][index(x * 2 + 1, y * 2 + 1, width[n], height[n])] * alpha[n][index(x * 2 + 1, y * 2 + 1, width[n], height[n])];
      int alphasumme =
        alpha[n][index(x * 2, y * 2, width[n], height[n])] + 
        alpha[n][index(x * 2 + 1, y * 2, width[n], height[n])] + 
        alpha[n][index(x * 2, y * 2 + 1, width[n], height[n])] + 
        alpha[n][index(x * 2 + 1, y * 2 + 1, width[n], height[n])];
      alpha[n + 1][y * width[n + 1] + x] = min(255, alphasumme / 2); /* Alpha value * 2 */
      data[n + 1][y * width[n + 1] + x] = (alphasumme == 0) ? 0 : (summe / alphasumme);
    }
  }
}

uint8_t alphablend(uint8_t a, uint8_t b, uint8_t alpha)
{
  return (a * alpha + b * (255 - alpha)) / 255;
}

uint8_t subpix(uint8_t* data, int x, int y, int width, int height)
{
  if (y % 2 == 0)
  {
    if (x % 2 == 0)
      return (
        (int)data[index(x / 2, y / 2, width, height)] * 9 + 
        (int)data[index(x / 2 - 1, y / 2, width, height)] * 3 + 
        (int)data[index(x / 2, y / 2 - 1, width, height)] * 3 + 
        (int)data[index(x / 2 - 1, y / 2 - 1, width, height)] * 1
      ) / 16;
    else
      return (
        (int)data[index(x / 2, y / 2, width, height)] * 9 + 
        (int)data[index(x / 2 + 1, y / 2, width, height)] * 3 + 
        (int)data[index(x / 2, y / 2 - 1, width, height)] * 3 + 
        (int)data[index(x / 2 + 1, y / 2 - 1, width, height)] * 1
      ) / 16;
  }
  else
  {
    if (x % 2 == 0)
      return (
        (int)data[index(x / 2, y / 2, width, height)] * 9 + 
        (int)data[index(x / 2 - 1, y / 2, width, height)] * 3 + 
        (int)data[index(x / 2, y / 2 + 1, width, height)] * 3 + 
        (int)data[index(x / 2 - 1, y / 2 + 1, width, height)] * 1
      ) / 16;
    else
      return (
        (int)data[index(x / 2, y / 2, width, height)] * 9 + 
        (int)data[index(x / 2 + 1, y / 2, width, height)] * 3 + 
        (int)data[index(x / 2, y / 2 + 1, width, height)] * 3 + 
        (int)data[index(x / 2 + 1, y / 2 + 1, width, height)] * 1
      ) / 16;
  }
}

void filler::restore(int n)
{
  int x, y;
  for (y = 0; y < height[n]; y++)
  {
    for (x = 0; x < width[n]; x++)
    {
      data[n][y * width[n] + x] = alphablend(data[n][y * width[n] + x], subpix(data[n + 1], x, y, width[n + 1], height[n + 1]), alpha[n][y * width[n] + x]);
      if (n)
        alpha[n][y * width[n] + x] = alphablend(alpha[n][y * width[n] + x], 255, subpix(alpha[n + 1], x, y, width[n + 1], height[n + 1]));
    }
  }
}

void filler::fill()
{
  int i;
  for (i = 0; i < 31; i++)
  {
    //blur(i);
    shrink(i);
  }
  alpha[31][0] = 255;
  for (i = 30; i >= 0; i--)
  {
    restore(i);
  }
}
