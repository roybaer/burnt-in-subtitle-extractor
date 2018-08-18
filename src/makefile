
all : locsubtitles getsubtitles remsubtitles

locsubtitles : locsubtitles.cpp common/y4m.cpp common/subtitle.cpp common/rectangle.cpp common/color.cpp common/ycbcr_img.cpp common/pgm.cpp
	g++ -O2 -lboost_program_options -o locsubtitles locsubtitles.cpp common/y4m.cpp common/subtitle.cpp common/rectangle.cpp common/color.cpp common/ycbcr_img.cpp common/pgm.cpp

getsubtitles : getsubtitles.cpp common/y4m.cpp common/subtitle.cpp common/rectangle.cpp common/color.cpp common/ycbcr_img.cpp common/pgm.cpp
	g++ -O2 -lboost_program_options -o getsubtitles getsubtitles.cpp common/y4m.cpp common/subtitle.cpp common/rectangle.cpp common/color.cpp common/ycbcr_img.cpp common/pgm.cpp

remsubtitles : remsubtitles.cpp common/y4m.cpp common/subtitle.cpp common/filler.cpp common/color.cpp common/ycbcr_img.cpp common/pgm.cpp
	g++ -O2 -lboost_program_options -o remsubtitles remsubtitles.cpp common/y4m.cpp common/subtitle.cpp common/filler.cpp common/color.cpp common/ycbcr_img.cpp common/pgm.cpp
