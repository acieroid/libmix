/**
 * @file ossapi.h
 * @brief Provides utilities to access OSS's API
 */
#ifndef MIX_OSSAPI_H
#define MIX_OSSAPI_H

#include <stdio.h>

/**
 * The file descriptor used to communicate with OSS's API
 */
typedef MixAPIFD int;

#define OSS_CALL(FD, CALL, ARG)                         \
  if (ioctl((FD), (CALL), (ARG)) == -1) {               \
    perror((#CALL));                                    \
  }

#endif /* MIX_OSSAPI_H */
