/* yvals.h values header for Microsoft C/C++ */
#pragma once

		/* NAMESPACE */

 #if defined(__cplusplus)
  #define _OLDSTD_BEGIN	namespace oldstd {
  #define _OLDSTD_END		}
  #define _OLDSTD	::oldstd::

/*
We use the stdext (standard extension) namespace to contain extensions that are not part of the current standard
*/
  #define _OLDSTDEXT_BEGIN	    namespace oldstdext {
  #define _OLDSTDEXT_END		}
  #define _OLDSTDEXT	        ::oldstdext::

  #ifdef _STD_USING
   #define _C_OLDSTD_BEGIN	namespace oldstd {	/* only if *.c compiled as C++ */
   #define _C_OLDSTD_END	}
   #define _COLDSTD	::oldstd::

  #else /* _STD_USING */
/* #define _GLOBAL_USING	*.h in global namespace, c* imports to std */

   #define _C_OLDSTD_BEGIN
   #define _C_OLDSTD_END
   #define _COLDSTD	::
  #endif /* _STD_USING */

  #define _C_LIB_OLD_DECL		extern "C" {	/* C has extern "C" linkage */
  #define _END_C_LIB_OLD_DECL	}
  #define _EXTERN_C			extern "C" {
  #define _END_EXTERN_C		}

 #else /* __cplusplus */
  #define _OLDSTD_BEGIN
  #define _OLDSTD_END
  #define _OLDSTD

  #define _C_OLDSTD_BEGIN
  #define _C_OLDSTD_END
  #define _COLDSTD

  #define _C_LIB_OLD_DECL
  #define _END_C_LIB_OLD_DECL
  #define _EXTERN_C
  #define _END_EXTERN_C
 #endif /* __cplusplus */

 #define _Restrict	restrict

 #ifdef __cplusplus
_OLDSTD_BEGIN
typedef bool _Bool;
_OLDSTD_END
 #endif /* __cplusplus */


/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V3.13:0009 */
