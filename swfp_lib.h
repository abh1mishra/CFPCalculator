/* Do Not Modify This File */
#ifndef SWFP_LIB_H
#define SWFP_LIB_H

#include "fp.h"

swfp_t double_to_swfp(double val);
double swfp_to_double(swfp_t val);
swfp_t swfp_mul(swfp_t val1, swfp_t val2);
swfp_t swfp_add(swfp_t val1, swfp_t val2);

#endif
