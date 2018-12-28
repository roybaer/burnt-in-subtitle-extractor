# Burnt-in subtitle extractor

This project provides a set of basic extraction tools for burnt-in subtitles, i.e. subtitles that are part of the picture itself.

## Tools

The burnt-in subtitle extractor consists of four different tools:

- `locsubtitles` locates subtitles within the picture, creates mask files and writes their names to an initial textual subtitle file along with timing information
- `getsubtitles` applies those masks to the picture and writes the averaged out graphical subtitles to a new set of image files
- `remsubtitles` deletes the masked out areas from the input frames and blends in the surrounding colors
- `ocrsubtitles` runs the extracted graphical subtitles through an external OCR and outputs the final textual subtitle file

## Examples

The following examples will in part rely on default parameters.
You will typically have to provide parameters tuned to your input file.
Type e.g. `getsubtitles --help` for details.

```
> ffmpeg -i test.flv -f yuv4mpegpipe - | ./locsubtitles > test.sub
> ffmpeg -i test.flv -f yuv4mpegpipe - | ./remsubtitles -s test.sub | ffplay -
```

## How it works

This program relies on several patterns in how subtitles are typically added to videos:

- Subtitles typically have borders and a defined text and border color
- Subtitles are usually located in the same part of the picture and do not move
  - Analysis can be limited to a specified region
- This program further relies on the presence of a frame without subtitles between frames with different subtitles

The algorithm then does the following:

- For every frame
  - Create a text mask that includes all pixels that have the text color
  - Expand that text mask in all four directions by the stroke width of the text border
  - Create a border mask that includes all pixels that have the border color
  - Expand that border mask in all four directions by the stroke width of the text
  - Calculate the intersection of both masks
  - If the number of pixels in the resulting mask is above a specified threshold
    - (A subtitle has been found)
    - If the previous frame did not have subtitles
      - Mark the start position
      - Otherwise: Set the subtitle mask to the intersection of both frames' subtitle masks
    - Otherwise (no subtitle)
      - If the previous frame had subtitles
        - Write the previous (i.e. the non-empty) subtitle mask to an image file
        - Write start and end time information and the name of the mask file to the subtitle file
- When the end of the video has been reached, output any pending information and exit

The OCR component uses the Tesseract engine, which has to be installed on the system.
The target language is currently hard-coded to Dutch.  This can be changed in the wrapper script.

## Prerequisites

The program has tested with the following setup, only:

- GNU/Linux operating system
- GNU C++ compiler + Boost libraries
- Bash shell + Tesseract OCR
- Y4M video streams produced by ffmpeg

Other setups might work.
