/**
 * @file ossapi.h
 * @brief Provides utilities to access OSS's API
 */
#ifndef MIX_OSSAPI_H
#define MIX_OSSAPI_H

#include <stdio.h>
#include <sys/soundcard.h>

/**
 * The file descriptor used to communicate with OSS's API
 */
typedef int MixAPIFD;

/**
 * Call an OSS routine and check if there has been errors during the call
 */
#define OSS_CALL(FD, CALL, ARG)                         \
  if (ioctl((FD), (CALL), (ARG)) == -1) {               \
    perror((#CALL));                                    \
  }

#endif /* MIX_OSSAPI_H */
