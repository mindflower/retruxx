// cstdlib standard header
#pragma once
#include "yvals.hpp"

#ifdef _STD_USING
 #undef _STD_USING
#include <stdlib.h>
 #define _STD_USING

#else /* _STD_USING */
 #include <stdlib.h>

 #if _GLOBAL_USING
_OLDSTD_BEGIN
using ::size_t; using ::div_t; using ::ldiv_t;

using ::abort; using ::abs; using ::atexit;
using ::atof; using ::atoi; using ::atol;
using ::bsearch; using ::calloc; using ::div;
using ::exit; using ::free; using ::getenv;
using ::labs; using ::ldiv; using ::malloc;
using ::mblen; using ::mbstowcs; using ::mbtowc;
using ::qsort; using ::rand; using ::realloc;
using ::srand; using ::strtod; using ::strtol;
using ::strtoul; using ::system;
using ::wcstombs; using ::wctomb;
_OLDSTD_END
 #endif /* _GLOBAL_USING */

#endif /* _STD_USING */

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V3.13:0009 */
