#ifndef __CODEC_UTIL_H__
#define __CODEC_UTIL_H__

#include <config.h>
#include <stdio.h>

#define NONE_LOG               0
#define ERR_LOG                1
#define WARN_LOG               3
#define INFO_LOG               4   
#define ALL_LOG                5

#define LOG_OPTION             ALL_LOG

// 按a字节向外对齐
#define ALIGN(x, a)	        (((x) + (a) - 1) & ~((a) - 1))
// 按a字节向内对齐
#define ALIGN_INNER(x, a)	((x) & ~((a) - 1))

#define CODEC_LOG(level, fmt, arg...) \
    printf("[%s @ %s] %s (%s:%d) "fmt"", PACKAGE_STRING, __FILE__, level, __FUNCTION__, __LINE__, ##arg)

#define B_RED(str)      "\033[1;31m" str "\033[0m"
#define B_GREEN(str)    "\033[1;32m" str "\033[0m"
#define B_YELLOW(str)   "\033[1;33m" str "\033[0m"
#define B_BLUE(str)     "\033[1;34m" str "\033[0m"
#define B_MAGENTA(str)  "\033[1;35m" str "\033[0m"
#define B_CYAN(str)     "\033[1;36m" str "\033[0m"
#define B_WHITE(str)    "\033[1;37m" str "\033[0m"

#define RED(str)        "\033[31m" str "\033[0m"
#define GREEN(str)      "\033[32m" str "\033[0m"
#define YELLOW(str)     "\033[33m" str "\033[0m"
#define BLUE(str)       "\033[34m" str "\033[0m"
#define MAGENTA(str)    "\033[35m" str "\033[0m"
#define CYAN(str)       "\033[36m" str "\033[0m"
#define WHITE(str)      "\033[37m" str "\033[0m"

#if LOG_OPTION == NONE_LOG
    #define debug(fmt, arg...)  
    #define info(fmt, arg...) 
    #define warn(fmt, arg...)   
    #define error(fmt, arg...)
#elif  LOG_OPTION == ERR_LOG
    #define debug(fmt, arg...)  
    #define info(fmt, arg...)
    #define warn(fmt, arg...)    
    #define error(fmt, arg...)  CODEC_LOG("ERROR", B_RED(fmt), ##arg)
#elif LOG_OPTION == WARN_LOG
    #define debug(fmt, arg...)  
    #define info(fmt, arg...)   
    #define warn(fmt, arg...)   CODEC_LOG("WARN",  B_YELLOW(fmt), ##arg)
    #define error(fmt, arg...)  CODEC_LOG("ERROR", B_RED(fmt), ##arg)
#elif  LOG_OPTION == INFO_LOG
    #define debug(fmt, arg...) 
    #define info(fmt, arg...)   CODEC_LOG("INFO",  B_GREEN(fmt), ##arg)
    #define warn(fmt, arg...)   CODEC_LOG("WARN",  B_YELLOW(fmt), ##arg)
    #define error(fmt, arg...)  CODEC_LOG("ERROR", B_RED(fmt), ##arg)
#elif  LOG_OPTION == ALL_LOG
    #define debug(fmt, arg...)  CODEC_LOG("DEBUG", B_CYAN(fmt), ##arg)
    #define info(fmt, arg...)   CODEC_LOG("INFO",  B_GREEN(fmt), ##arg)
    #define warn(fmt, arg...)   CODEC_LOG("WARN",  B_YELLOW(fmt), ##arg)
    #define error(fmt, arg...)  CODEC_LOG("ERROR", B_RED(fmt), ##arg)
#else 
#error LOG_OPTION NO SUPPORT!
#endif


#endif  //!__CODEC_UTIL_H__