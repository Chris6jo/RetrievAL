#ifdef _UNICODE
#undef _UNICODE
#endif
#ifdef UNICODE
#undef UNICODE
#endif
#define _ui64tot     _ui64toa
#define _ui64totn(n) _ui64toa ## n
#define _ultotn(n)   _ultoa ## n
#include "ui64tot.c"
