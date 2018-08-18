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
#include <sstream>
#include <string>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "common/y4m.hpp"
#include "common/subtitle.hpp"
#include "common/rectangle.hpp"
#include "common/color.hpp"
#include "common/ycbcr_img.hpp"
#include "common/pgm.hpp"

using namespace std;

int bounds(int a, int b, int c)
{
  if (a < b)
    return b;
  if (a > c)
    return c;
  return a;
}

int main(int argc, char **argv)
{
  rectangle area(50, 350, 550, 100);

  color text_color(205, 128, 128);
  color text_color_diff(45, 20, 20);
  color border_color(50, 128, 128);
  color border_color_diff(50, 40, 40);

  string prefix;
  string subname;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "print help message")
    ("area,a", po::value<rectangle>(&area), "area containing subtitles (x,y,w,h)")
    ("text-color,t", po::value<color>(&text_color), "text color (y,cb,cr)")
    ("text-cdiff,T", po::value<color>(&text_color_diff), "text color max. difference (y,cb,cr)")
    ("border-color,b", po::value<color>(&border_color), "border color (y,cb,cr)")
    ("border-cdiff,B", po::value<color>(&border_color_diff), "border color max. difference (y,cb,cr)")
    ("prefix,p", po::value<string>(&prefix)->default_value("text_"), "name prefix for numbered .pgm files showing text")
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

  int dark = (text_color.y + border_color.y) / 2;
  int bright = text_color.y;

  y4m_stream_header stream_header;
  cin >> stream_header;

  ycbcr_img img(stream_header.w, stream_header.h);

  int img_no = 0;
  int mask_no = 0;

  int start = 0;
  int end = 0;

  uint8_t* mask = NULL;

  uint32_t* img_area_luma = NULL;

  int count = 0;

  ifstream f;
  f.open(subname.c_str());

  char buffer[10000];

  while (!cin.eof())
  {
    cin.getline(buffer, 9999);
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
      ss >> noskipws >> area.x >> c >> area.y >> c;
      string name;
      getline(ss, name);

      read_pgm(name.c_str(), mask, area.w, area.h);

      img_area_luma = new uint32_t[area.w * area.h];
      memset(img_area_luma, 0, area.w * area.h * sizeof(uint32_t));
      count = 0;
    }

    int x, y;
    if ((img_no >= start) && (img_no < end))
    {
      for (y = 0; y < area.h; y++)
      {
        for (x = 0; x < area.w; x++)
        {
          img_area_luma[y * area.w + x] += img.y[(y + area.y) * img.width + x + area.x];
        }
      }
      count++;
    }

    img_no++;

    if (img_no == end)
    {
      for (y = 0; y < area.h; y++)
      {
        for (x = 0; x < area.w; x++)
        {
          if (mask[y * area.w + x])
            mask[y * area.w + x] = bounds(((int)(img_area_luma[y * area.w + x] / count) - dark) * 255 / (bright - dark), 0, 255);
        }
      }

      ostringstream oss;
      oss << prefix << mask_no << ".pgm";
      write_pgm(mask, area.w, area.h, oss.str().c_str());
      oss.clear();
      oss << area.x << " " << area.y << " " << prefix << mask_no << ".pgm";
      cout << subtitle(start, end, oss.str());
      mask_no++;

      delete [] img_area_luma;
      img_area_luma = NULL;
      delete [] mask;
      mask = NULL;
    }

  }
}
