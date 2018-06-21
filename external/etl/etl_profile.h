#ifndef __ETL_PROFILE_H__
#define __ETL_PROFILE_H__

#define ETL_THROW_EXCEPTIONS
#define ETL_VERBOSE_ERRORS
#define ETL_CHECK_PUSH_POP
#include <stdint.h>
#ifdef GCC_COMPILER
#include "../../external/etl/profiles/gcc_generic.h"
#else
#include "../../external/etl/profiles/ticc.h"
#endif
#define ETL_NO_CHECKS
#endif
