#if defined(WIN32) || defined(_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#else
    #include <dlfcn.h>
#endif
#include "DynamicLibrary.h"

namespace fm {

DynamicLibrary::DynamicLibrary(const std::string& path) : handle(NULL)
{
	assert( !path.empty() );
#if defined(WIN32) || defined(_WINDOWS)
	handle = (void*)LoadLibraryA(path.c_str());
	if (handle == NULL) {
		LPVOID msg_buf = NULL;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msg_buf, 0, NULL);
		if (msg_buf)
			load_error = (LPCSTR)msg_buf;
		LocalFree(msg_buf);
	}
#else
	handle = dlopen(path.c_str(), RTLD_LAZY);
	if (handle == NULL && dlerror())
		load_error = dlerror();
#endif
    if( handle ) {
		lib_path = path;
		load_error.clear();
	}
}

DynamicLibrary::~DynamicLibrary()
{
	if( handle != NULL ) {
#if defined(WIN32) || defined(_WINDOWS)
		FreeLibrary((HMODULE)handle);
#else
		dlclose(handle);
#endif
	}
}

bool DynamicLibrary::IsLoaded() const
{
	return handle != NULL;
}

const std::string& DynamicLibrary::GetLoadError() const
{
	return load_error;
}

void* DynamicLibrary::GetProcAddress(const char* name) const
{
	assert(name != NULL && handle != NULL);

	if( handle != NULL ) {
#if defined(WIN32) || defined(_WINDOWS)
		return (void*)::GetProcAddress((HMODULE)handle, name);
#else
		return (void*)dlsym(handle, name);
#endif
	}
	return NULL;
}

const std::string& DynamicLibrary::GetPath() const
{
	return lib_path;
}

}
