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

#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <string>
#include <sstream>
#include <stdlib.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "common/y4m.hpp"
#include "common/subtitle.hpp"
#include "common/rectangle.hpp"
#include "common/color.hpp"
#include "common/ycbcr_img.hpp"
#include "common/pgm.hpp"

using namespace std;

void expand(uint8_t* img, int width, int height, int left, int right, int top, int bottom)
{
  int x, y;
  for (y = 0; y < height; y++)
  {
    int distance = 1000;
    for (x = left; x < width; x++)
    {
      if (img[y * width + x])
      {
        if (distance > right)
        {
          int x2;
          for (x2 = x - left; x2 < x; x2++)
            img[y * width + x2] = 255;
        }
        distance = 0;
      }
      else if (distance < right)
        img[y * width + x] = 255;
      distance++;
    }
  }
  for (x = 0; x < width; x++)
  {
    int distance = 1000;
    for (y = top; y < height; y++)
    {
      if (img[y * width + x])
      {
        if (distance > bottom)
        {
          int y2;
          for (y2 = y - top; y2 < y; y2++)
            img[y2 * width + x] = 255;
        }
        distance = 0;
      }
      else if (distance < bottom)
        img[y * width + x] = 255;
      distance++;
    }
  }
}

int count_nonzero(uint8_t* img, int size)
{
  int n = 0;
  int i;
  for (i = 0; i < size; i++)
  {
    if (img[i])
      n++;
  }
  return n;
}

void swap(uint8_t*& a, uint8_t*& b)
{
  uint8_t* temp = a;
  a = b;
  b = temp;
}


int main(int argc, char** argv)
{
  rectangle area(50, 350, 550, 100);
  rectangle radius1(4, 6, 4, 6);
  rectangle radius2(4, 4, 4, 4);

  color text_color(205, 128, 128);
  color text_color_diff(45, 20, 20);
  color border_color(50, 128, 128);
  color border_color_diff(50, 40, 40);

  string prefix;

  int threshold;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "print help message")
    ("area,a", po::value<rectangle>(&area), "area containing subtitles (x,y,w,h)")
    ("text-color,t", po::value<color>(&text_color), "text color (y,cb,cr)")
    ("text-cdiff,T", po::value<color>(&text_color_diff), "text color max. difference (y,cb,cr)")
    ("border-color,b", po::value<color>(&border_color), "border color (y,cb,cr)")
    ("border-cdiff,B", po::value<color>(&border_color_diff), "border color max. difference (y,cb,cr)")
    ("prefix,p", po::value<string>(&prefix)->default_value("mask_"), "name prefix for numbered .pgm masks")
    ("radius1,r", po::value<rectangle>(&radius1), "radius 1 (left,right,top,bottom)")
    ("radius2,R", po::value<rectangle>(&radius2), "radius 2 (left,right,top,bottom)")
    ("threshold,s", po::value<int>(&threshold)->default_value(5000), "threshold")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    clog << desc << "\n";
    exit(EXIT_FAILURE);
  }

  y4m_stream_header stream_header;
  cin >> stream_header;

  bool subtitles_visible = false;

  int img_no = 0;
  int mask_no = 0;
  int start = 0;
  int end = 0;

  ycbcr_img img(stream_header.w, stream_header.h);

  uint8_t* mask1 = new uint8_t[area.w * area.h];  // text
  uint8_t* mask2 = new uint8_t[area.w * area.h];  // border
  uint8_t* mask3 = new uint8_t[area.w * area.h];  // text & border (intersection)
  uint8_t* mask3_old = new uint8_t[area.w * area.h];

  char buffer[10000];

  while (!cin.eof())
  {
    cin.getline(buffer, 9999);
    cin >> img;
    memset(mask1, 0, area.w * area.h);
    memset(mask2, 0, area.w * area.h);
    int x, y, i;
    for (y = 0; y < area.h; y++)
    {
      for (x = 0; x < area.w; x++)
      {
        color pix(img.pixel(x + area.x, y + area.y));
        if (pix.similar_to(text_color, text_color_diff))
          mask1[y * area.w + x] = 255;
        else if (pix.similar_to(border_color, border_color_diff))
          mask2[y * area.w + x] = 255;
      }
    }
    expand(mask1, area.w, area.h, radius1.x, radius1.y, radius1.w, radius1.h);
    expand(mask2, area.w, area.h, radius2.x, radius2.y, radius2.w, radius2.h);
    for (i = 0; i < area.w * area.h; i++)
    {
      mask3[i] = mask1[i] & mask2[i];
    }
    if (count_nonzero(mask3, area.w * area.h) > threshold)
    {
      if (!subtitles_visible)
      {
        start = img_no;
        subtitles_visible = true;
      }
      else
      {
        for (i = 0; i < area.w * area.h; i++)
        {
          mask3[i] &= mask3_old[i];
        }
      }
    }
    else
    {
      if (subtitles_visible)
      {
        end = img_no;
        ostringstream oss;
        oss << prefix << mask_no << ".pgm";
        write_pgm(mask3_old, area.w, area.h, oss.str().c_str());
        ostringstream oss2;
        oss2 << area.x << " " << area.y << " " << prefix << mask_no << ".pgm";
        cout << subtitle(start, end, oss2.str());
        mask_no++;
        subtitles_visible = false;
      }
    }

    img_no++;
    swap(mask3, mask3_old);
  }
}
