// cstddef standard header
#pragma once
#include "yvals.hpp"

#ifdef _STD_USING
 #undef _STD_USING
  #include "stddef.h"
 #define _STD_USING

#else /* _STD_USING */
 #include "stddef.h"

 #if _GLOBAL_USING
_OLDSTD_BEGIN
using ::ptrdiff_t; using ::size_t;
_OLDSTD_END
 #endif /* _GLOBAL_USING */

#endif /* _STD_USING */

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V3.13:0009 */
