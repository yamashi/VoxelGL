#pragma once

void InitializeLog();
void ShutdownLog();

void Crash(const char* acpMessage);

void LogError(const char* acpMessage);
void LogWarn(const char* acpMessage);