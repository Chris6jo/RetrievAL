#ifndef _UNICODE
#define _UNICODE 1
#endif
#ifndef UNICODE
#define UNICODE 1
#endif
#define _ultot               _ultow
#define _ui64tot             _ui64tow
#define _ui32tont(n)         _ui32to##n##w
#define _ui64tont(n)         _ui64to##n##w
#define internal_ui64tont(n) internal_ui64to##n##w
#include "ui64tot.h"
