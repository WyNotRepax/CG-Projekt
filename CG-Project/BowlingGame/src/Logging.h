#pragma once

// Allows for Logging with Debug information in the Debug Configuration (defines _DEBUG)

#ifdef _DEBUG

#define LOG(format,...)(fprintf(stderr,"%s:%d: " format,__FILE__,__LINE__,__VA_ARGS__))

#define LOG_CALL(f,...) { f(__VA_ARGS__);GLenum Error = glGetError(); if(Error != GL_NO_ERROR){LOG("GL_ERROR:0x%x\n",Error); }}

#else

#define LOG(FORMAT,...)

#define LOG_CALL(f,...)f(__VA_ARGS__)

#endif
