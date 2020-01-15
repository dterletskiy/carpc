#pragma once

#include "Colors.hpp"



//                           +-------------------------- Foreground Colore
//                           |  +----------------------- Class Abbriviation
//                           |  |  +-------------------- Function Name
//                           |  |  |  +----------------- Line Number
//                           |  |  |  |      +---------- Foreground Colore for user message
//                           |  |  |  |      | +-------- Background Color for user message
//                           |  |  |  |      | |
//                           |  |  |  |      | |
#define PREFIX_FORMAT_CODE "%s[%s:%s:%d] -> %s%s"


//                              +---------------------------- Foreground Colore for Delimiter
//                              | +-------------------------- Delimiter sign
//                              | |  +----------------------- Foreground Colore for PID
//                              | |  |  +-------------------- PID
//                              | |  |  |   +---------------- Foreground Colore for TID
//                              | |  |  |   |   +------------ TID
//                              | |  |  |   |   |   +-------- Foreground Colore for Delimiter
//                              | |  |  |   |   |   | +------ Delimiter sign
//                              | |  |  |   |   |   | |
//                              | |  |  |   |   |   | |
#define PREFIX_FORMAT_PID_TID "%s%s %s%#5x %s%#10lx %s%s"

//                                   +----------------------- Foreground Colore
//                                   |   +------------------- Microseconds
//                                   |   |
//                                   |   |
#define PREFIX_FORMAT_MICROSECONDS "%s%20lu"

//                                             +------------------------------------------------ Foreground Colore
//                                             |  +--------------------------------------------- Year
//                                             |  |    +---------------------------------------- Month
//                                             |  |    |    +----------------------------------- Day
//                                             |  |    |    |   +------------------------------- Foreground Colore
//                                             |  |    |    |   |  +---------------------------- Hours
//                                             |  |    |    |   |  |    +----------------------- Minutes
//                                             |  |    |    |   |  |    |    +------------------ Seconds
//                                             |  |    |    |   |  |    |    |    +------------- Milliseconds
//                                             |  |    |    |   |  |    |    |    |
//                                             |  |    |    |   |  |    |    |    |
#define PREFIX_FORMAT_DATE_TIME_MILLISECONDS "%s%04d-%02d-%02d %s%02d:%02d:%02d:%06d"

#define PREFIX_FORMAT_MICROSECONDS_PID_TID_CODE PREFIX_FORMAT_MICROSECONDS " " PREFIX_FORMAT_PID_TID "   " PREFIX_FORMAT_CODE
#define PREFIX_FORMAT_DATE_TIME_MILLISECONDS_PID_TID_CODE PREFIX_FORMAT_DATE_TIME_MILLISECONDS " " PREFIX_FORMAT_PID_TID "   " PREFIX_FORMAT_CODE

#define NEW_LINE RESET "\n"

#define DBG_NO( FG_MSG, BG_MSG, USER_FORMAT, ... )


