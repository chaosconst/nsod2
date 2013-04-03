#ifndef _H_NSOD_COMMON
#define _H_NSOD_COMMON

#include<cstdio>
#include<iostream>
#include<fstream>
#include<string>
double * nsod_common_get_double_array_from_string(std::string str, int size);
int * nsod_common_get_int_array_from_string(std::string str, int size);

#endif
