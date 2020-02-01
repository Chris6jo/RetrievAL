#ifndef _FPCONST_H_
#define _FPCONST_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern const unsigned int _fltused;
extern const double fpconst_inf;
extern const double fpconst_minus_inf;
extern const double fpconst_nan;
extern const double fpconst_nan_ind;
extern const double fpconst_max;
extern const double fpconst_true_min;
extern const double fpconst_half;
extern const double fpconst_one;
extern const double fpconst_two;
extern const double fpconst_minus_one;
extern const double fpconst_fyl2xp1_limit;
extern const unsigned int fpconst_x0363;
extern const unsigned int fpconst_x0763;
extern const unsigned int fpconst_x0B63;
extern const unsigned int fpconst_x0F63;

#ifdef __cplusplus
}
#endif

#endif	// _FPCONST_H_
