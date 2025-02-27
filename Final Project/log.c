#include "log.h"
#define _CRT_SECURE_NO_WARNINGS

// Helper function to get the current timestamp
char* getTimestamp() {
    static char buffer[20];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
        t->tm_hour, t->tm_min, t->tm_sec);
    return buffer;
}

// Log a user action to the log file
void logAction(const char* username, const char* action) {
    FILE* fp = fopen("system.log", "a");
    if (!fp) {
        printf("Error: Could not open log file for writing.\n");
        return;
    }

    fprintf(fp, "[%s] USER: %s ACTION: %s\n", getTimestamp(), username, action);
    fclose(fp);
}

// Log system startup
void logSystemStart() {
    FILE* fp = fopen("system.log", "a");
    if (!fp) {
        printf("Error: Could not open log file for writing.\n");
        return;
    }

    fprintf(fp, "[%s] SYSTEM: System started.\n", getTimestamp());
    fclose(fp);
}

// Log system shutdown
void logSystemShutdown() {
    FILE* fp = fopen("system.log", "a");
    if (!fp) {
        printf("Error: Could not open log file for writing.\n");
        return;
    }

    fprintf(fp, "[%s] SYSTEM: System shut down.\n", getTimestamp());
    fclose(fp);
}

