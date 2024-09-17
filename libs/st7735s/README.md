# Library for the ST7735S display driver

## Demonstration

[![demonstration video][display_image]][demo_video]

Click on the picture to go to the video.

## Architecture

![library architecture][architecture_image]

This library is an abstraction. To use it, you need to implement functions that will handle communication with the display driver. The header file contains a signature and description of these functions. The implementation itself is very easy. The source code includes an implementation example for Wiring Pi based on the 4-line SPI. After implementing these functions, you need to link them to the library code into your program. This architecture allows this library to be used with a wide range of devices.

If you need to use this library with a parallel interface, you have to rewrite the functions for sending data and drawing. The code is made so simply that this re-implementation will be easy.

The library code has been created in such a way that it can be easily changed to support similar display drivers. Therefore, optimal size variables were used. If you need to minimize the use of operating memory, you can: reduce the size of the variables that store the dimensions of the display and get rid of the pointers. The code will become less flexible but more suited for a specific device.

By using the pointer, you can easily and quickly switch to another display module and draw on it.

The library code is made in the ANSI C standard (X3.159-1989).

## Documentation

* The documentation was written in the header file: [st7735s.h][st7735s_h]
* Implementation details can be read in the file: [st7735s.c][st7735s_c]
* Usage guide is based on the Wiring Pi library: [main_example_wpi.c][main_example_wpi_c]
* Datasheet: [st7735s-datasheet-v1.4.pdf][datasheet_file]

## License

This software is licensed under the MIT License. See the [license file][license_file] for details.

[display_image]:  documentation/notes/display.png
[demo_video]: https://www.youtube.com/watch?v=2PtDBSYxbV0
[architecture_image]: documentation/notes/architecture/architecture.png
[datasheet_file]: documentation/datasheet/st7735s-datasheet-v1.4.pdf
[st7735s_h]: source/st7735s.h
[st7735s_c]: source/st7735s.c
[main_example_wpi_c]: source/main_example_wpi.c
[license_file]: license.txt
