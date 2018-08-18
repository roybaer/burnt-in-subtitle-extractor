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
#include <inttypes.h>
#include <fstream>
#include <string>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "common/y4m.hpp"
#include "common/subtitle.hpp"
#include "common/filler.hpp"
#include "common/ycbcr_img.hpp"
#include "common/pgm.hpp"

using namespace std;


int main(int argc, char **argv)
{
  int area_x;
  int area_y;
  string subname;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "print help message")
    ("area-x,x", po::value<int>(&area_x)->default_value(50), " x-coordinate of area containing subtitles")
    ("area-y,y", po::value<int>(&area_y)->default_value(350), " y-coordinate of area containing subtitles")
    ("sub,s", po::value<string>(&subname)->default_value("test.sub"), "name of input .sub file")
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
  cout << stream_header;

  int mask_width;
  int mask_height;
  int mask_width_chroma;
  int mask_height_chroma;

  int img_no = 0;
  int mask_no = 0;

  int start = 0;
  int end = 0;

  ycbcr_img img(stream_header.w, stream_header.h);
  uint8_t* mask = NULL;

  filler* filler_luma = NULL;
  uint8_t* img_area_luma = NULL;
  filler* filler_chroma = NULL;
  uint8_t* img_area_chroma = NULL;
  uint8_t* mask_chroma = NULL;

  ifstream f;
  f.open(subname.c_str());

  char buffer[10000];

  while (!cin.eof())
  {
    cin.getline(buffer, 9999);
    cout << buffer << "\n";
    cin >> img;

    if (img_no == end)
    {
      subtitle sub;
      f >> sub;
      start = sub.start;
      end = sub.end;
      stringstream ss;
      char c;
      ss << sub.text;
      ss >> noskipws >> area_x >> c >> area_y >> c;
      string name;
      getline(ss, name);

      read_pgm(name.c_str(), mask, mask_width, mask_height);
      mask_no++;

      mask_width_chroma = (mask_width + 1) / 2;
      mask_height_chroma = (mask_height + 1) / 2;
      int i, x, y;
      for (i = 0; i < mask_width * mask_height; i++)
        mask[i] = 255 - mask[i];
      img_area_luma = new uint8_t[mask_width * mask_height];
      filler_luma = new filler(mask_width, mask_height, img_area_luma, mask);
      img_area_chroma = new uint8_t[mask_width_chroma * mask_height_chroma];
      mask_chroma = new uint8_t[mask_width_chroma * mask_height_chroma];
      for (y = 0; y < (mask_height + 0) / 2; y++)
      {
        for (x = 0; x < (mask_width + 0) / 2; x++)
        {
          mask_chroma[y * mask_width_chroma + x] =
            mask[(y * 2) * mask_width + x * 2] &
            mask[(y * 2) * mask_width + x * 2 + 1] &
            mask[(y * 2 + 1) * mask_width + x * 2] &
            mask[(y * 2 + 1) * mask_width + x * 2 + 1];
        }
      }
      filler_chroma = new filler(mask_width_chroma, mask_height_chroma, img_area_chroma, mask_chroma);
    }

    int x, y;
    if ((img_no >= start) && (img_no < end))
    {
      for (y = 0; y < mask_height; y++)
      {
        for (x = 0; x < mask_width; x++)
        {
          img_area_luma[y * mask_width + x] = img.y[(y + area_y) * img.width + x + area_x];
        }
      }
      filler_luma->fill();
      for (y = 0; y < mask_height; y++)
      {
        for (x = 0; x < mask_width; x++)
        {
          img.y[(y + area_y) * img.width + x + area_x] = img_area_luma[y * mask_width + x];
        }
      }

      for (y = 0; y < mask_height_chroma; y++)
      {
        for (x = 0; x < mask_width_chroma; x++)
        {
          img_area_chroma[y * mask_width_chroma + x] = img.cb[(y + area_y / 2) * img.width_chroma + x + area_x / 2];
        }
      }
      filler_chroma->fill();
      for (y = 0; y < mask_height_chroma; y++)
      {
        for (x = 0; x < mask_width_chroma; x++)
        {
          img.cb[(y + area_y / 2) * img.width_chroma + x + area_x / 2] = img_area_chroma[y * mask_width_chroma + x];
        }
      }

      for (y = 0; y < mask_height_chroma; y++)
      {
        for (x = 0; x < mask_width_chroma; x++)
        {
          img_area_chroma[y * mask_width_chroma + x] = img.cr[(y + area_y / 2) * img.width_chroma + x + area_x / 2];
        }
      }
      filler_chroma->fill();
      for (y = 0; y < mask_height_chroma; y++)
      {
        for (x = 0; x < mask_width_chroma; x++)
        {
          img.cr[(y + area_y / 2) * img.width_chroma + x + area_x / 2] = img_area_chroma[y * mask_width_chroma + x];
        }
      }

    }

    cout << img;

    img_no++;

    if (img_no == end)
    {
      delete [] img_area_luma;
      img_area_luma = NULL;
      delete [] img_area_chroma;
      img_area_chroma = NULL;
      delete [] mask_chroma;
      mask_chroma = NULL;
      delete filler_chroma;
      filler_chroma = NULL;
      delete filler_luma;
      filler_luma = NULL;
      delete [] mask;
      mask = NULL;
    }

  }
}
