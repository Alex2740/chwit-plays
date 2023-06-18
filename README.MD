# Projet Docker - Chwit Plays

## Dependencies

The program use OpenGL, OpenCV and FFmpeg.

## Installation

### Windows

To be able to run the program, you will need:

* OpenCV -> [tuto](https://www.tutorialspoint.com/how-to-install-opencv-for-cplusplus-in-windows)
* FFmpeg -> [tuto](https://phoenixnap.com/kb/ffmpeg-windows)

To finish setting up the VS project add dependencies in the project setting -> [tuto](https://www.youtube.com/watch?v=XpBGwZNyUh0&t=208s)

Don't forget to reboot your computer after changing the environmental variables !

cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .

./broadcaster | ffmpeg -f rawvideo -pix_fmt rgb24 -s:v 800x800 -r 25 -i pipe: -f mpegts 'udp://239.0.0.1:5000?ttl=2'
