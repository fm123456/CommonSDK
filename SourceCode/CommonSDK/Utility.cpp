#if defined(WIN32) || defined(_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <uuid/uuid.h>
#endif
#include <boost/filesystem.hpp>
#include "Logging.h"
#include "Utility.h"

namespace fm {

#if defined(WIN32) || defined(_WINDOWS)
#include <winsock2.h>
std::string GetHostName(bool with_domain)
{
	char host_name[128] = {0};
	WSAData wsa;
	WSAStartup(MAKEWORD(1,1), &wsa);
	gethostname(host_name, 128);
	WSACleanup();
	if (strlen(host_name) == 0)
		return "Unknown Host";
	if (!with_domain) {
		char* domain_dot = strrchr(host_name, '.');
		if (domain_dot != NULL) {
			if (strcmp(domain_dot, ".local") == 0 )
				*domain_dot = 0;
		}
	}
	return host_name;
}
#else
std::string GetHostName(bool with_domain)
{
	char host_name[128] = {0};
	if (gethostname(host_name, 128) != 0)
		perror("gethostname()");
	if (strlen(host_name) == 0)
		return "Unknown Host";
	if (!with_domain) {
		char* domain_dot = strrchr(host_name, '.');
		if (domain_dot != NULL) {
			if (strcmp(domain_dot, ".local") == 0 )
				*domain_dot = 0;
		}
	}
	return host_name;
}
#endif

std::string GetUserName()
{
#if defined(WIN32) || defined(_WINDOWS)
  const char* user = getenv("USERNAME");
#else
  const char* user = getenv("USER");
#endif
  if (user != NULL)
    return user;
  else
    return "invalid-user";
}

int GetPID()
{
#if defined(WIN32) || defined(_WINDOWS)
	return ::GetCurrentProcessId();
#else
	return getpid();
#endif
}

#if defined(WIN32) || defined(_WINDOWS)
std::string GenerateUUID()
{
	return "";
}
#else
std::string GenerateUUID()
{
	uuid_t uuid;
	uuid_generate(uuid);

	unsigned char *p = uuid;
	char uuid_buf[128] = {0};
	for (size_t i = 0; i < sizeof(uuid_t); i++, p++)
	{
		char buf[16] = {0};
	   sprintf(buf, "%02x", *p);
	   strcat(uuid_buf, buf);
	}
	return uuid_buf;
}
#endif

Timer::Timer()
{
	value = Now();
}

void Timer::Reset()
{
	value = Now();
}

boost::int64_t Timer::Interval() const
{
	return Now() - value;
}

double Timer::Seconds() const
{
#if defined(WIN32) || defined(_WINDOWS)
    LARGE_INTEGER qpfreq;
	QueryPerformanceFrequency(&qpfreq);
	return (Now() - value)/(double)qpfreq.QuadPart;
#else
    return (Now() - value)*1E-9;
#endif
}

boost::int64_t Timer::Now()
{
	long long result;
#if defined(WIN32) || defined(_WINDOWS)
	LARGE_INTEGER qpcnt;
	QueryPerformanceCounter(&qpcnt);
	result = qpcnt.QuadPart;
#else
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	result = static_cast<long long>(1000000000UL)*
		static_cast<long long>(ts.tv_sec) + 
		static_cast<long long>(ts.tv_nsec);
#endif
	return result;
}


}
