#ifndef _FM_SDK_UTILITY_H_
#define _FM_SDK_UTILITY_H_

#include "SystemExport.h"

namespace fm {
namespace util {

LIB_SDK std::string GenerateUUID();

LIB_SDK std::string GetHostName(bool with_domain);

LIB_SDK std::string GetUserName();

LIB_SDK int GetPID();

LIB_SDK int LockPIDFile(const std::string& path);

LIB_SDK bool ParseConfigString(const std::string& str, std::map<std::string, std::string>& config);

struct IsBlankSpace { bool operator()(const char& c) const { return c == ' '; } };

struct IsComma { bool operator()(const char& c) const { return c == ','; } };

struct IsSemicolon { bool operator()(const char& c) const { return c == ';'; } };

struct IsEqualSign { bool operator()(const char& c) const { return c == '='; } };

}
}

#endif
