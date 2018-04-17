#ifndef DTRACE_IMPORT_HPP
#define DTRACE_IMPORT_HPP

#include <dtrace.h>
#include <stdio.h>
#include "NetUuidData.hpp"

bool setupDTrace(unsigned int rate, NetUuidData *);
bool installProbes(void);
void sleepDTrace(void);
bool stopTracing(void);
bool processDTraceBuffers(void);
void closeDTrace(void);

extern int applicationInterrupted;

#endif /*DTRACE_IMPORT_HPP*/
