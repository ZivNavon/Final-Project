// Ziv Navon | TZ: 207423955     
// Ofek Iram | TZ: 319089884

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <time.h>
#include <string.h>

// Function Prototypes
void logAction(const char* username, const char* action);
void logSystemStart();
void logSystemShutdown();

#endif // LOG_H