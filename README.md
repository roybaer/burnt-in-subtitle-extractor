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
