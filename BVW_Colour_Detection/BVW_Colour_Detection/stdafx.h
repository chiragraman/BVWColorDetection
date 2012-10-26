/* Copyright (C) 2011 Chirag Raman

This file is part of BVW_Colour_Detection.

BVW_Colour_Detection is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

BVW_Colour_Detection is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BVW_Colour_Detection.  If not, see <http://www.gnu.org/licenses/>.
*/

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently


#pragma once
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#include "targetver.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "cv.hpp"
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <tchar.h>


// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")



// TODO: reference additional headers your program requires here
