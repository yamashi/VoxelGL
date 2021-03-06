#include "Log.h"

_INITIALIZE_EASYLOGGINGPP

void InitializeLog()
{
	el::Configurations defaultConf;
	defaultConf.setToDefault();
	// To set GLOBAL configurations you may use
	defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime level=%level %msg");
	defaultConf.setGlobally(el::ConfigurationType::ToStandardOutput, "true");

	el::Loggers::reconfigureLogger("default", defaultConf);
	el::Loggers::reconfigureAllLoggers(defaultConf);
}

void ShutdownLog()
{

}

void Crash(const char* acpMessage)
{
	LOG(ERROR) << "error=\"" << acpMessage << "\"";
	exit(-1);
}