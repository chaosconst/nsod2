#include "NsodCommon.h"
#include <sstream>
using namespace std;

double * nsod_common_get_double_array_from_string(string str, int size) {
	double * res = (double*) malloc(sizeof(double)*size);
	istringstream iss(str);
	int i=0;
	string sub;
	while (getline(iss,sub,',')){
		res[i++]=atof(sub.c_str());
	};
	return res;
}

int * nsod_common_get_int_array_from_string(string str, int size) {
	int * res = (int*) malloc(sizeof(double)*size);
	istringstream iss(str);
	int i=0;
	string sub;
	while (getline(iss,sub,',')){
		res[i++]=atoi(sub.c_str());
	};
	return res;
}
