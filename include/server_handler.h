#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#ifdef _WIN32
#include <Windows.h>

DWORD WINAPI handle_client(LPVOID arg);
#else
void *handle_client(void *arg);
#endif

#endif