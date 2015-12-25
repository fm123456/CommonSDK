#ifndef _FM_SDK_EXPORT_H_
#define _FM_SDK_EXPORT_H_

#if defined(WIN32) || defined(_WINDOWS)
	#if defined(COMMONSDK_EXPORTS)
		#define LIB_SDK __declspec(dllexport)
		#pragma warning(disable: 4251 4275)
	#else
		#define LIB_SDK __declspec(dllimport)
		#pragma comment(lib,"CommonSDK.lib")
	#endif
	#pragma warning(disable: 4251 4275)
#else
	#define LIB_SDK
#endif

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <boost/version.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#endif
