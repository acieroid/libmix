/**
 * @file ossapi.h
 * @brief Provides utilities to access OSS's API
 */
#ifndef MIX_OSSAPI_H
#define MIX_OSSAPI_H

#include <stdio.h>

#define OSS_CALL(FD, CALL, ARG)                         \
  if (ioctl((FD), (CALL), (ARG)) == -1) {               \
    fprintf(stderr, "Error when calling: %s", (#CALL)); \
  }

#endif /* MIX_OSSAPI_H */
