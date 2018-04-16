#ifndef DTRACE_IMPORT_HPP
#define DTRACE_IMPORT_HPP

#include <dtrace.h>
#include <stdio.h>

bool setupDTrace(unsigned int);
bool installProbes(void);
void sleepDTrace(void);
bool stopTracing(void);
bool processDTraceBuffers(void);
void closeDTrace(void);

extern int applicationInterrupted;

#endif /*DTRACE_IMPORT_HPP*/
