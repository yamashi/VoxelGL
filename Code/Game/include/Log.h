#pragma once

#include "easylogging++.h"

void InitializeLog();
void ShutdownLog();

void Crash(const char* acpMessage);