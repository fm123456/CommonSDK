#ifndef _FM_SDK_DYNAMIC_LIBRARY_H_
#define _FM_SDK_DYNAMIC_LIBRARY_H_

#include <string>
#include "SystemExport.h"

namespace fm {

/**
 * @brief 运行时加载动态链接库。
 *
 * DynamicLibrary 类封装了动态链接库的概念，提供了跨平台的方法在运行时
 * 加载动态链接库，获取其中导出函数的功能。
 */
class LIB_SDK DynamicLibrary
{
public:
	/**
	 * @brief 构造函数，加载指定的动态库。
	 *
	 * @param path 动态库的路径。
	 */
	DynamicLibrary(const std::string& path);

	/**
	 * @brief 析构函数，卸载已经加载的动态库。
	 */
	~DynamicLibrary();

	/**
	 * @brief 判断动态库是否成功加载。
	 *
	 * @retval true 动态库加载成功
	 * @retval false 动态库加载失败
	 */
	bool IsLoaded() const;

	/**
	 * @brief 获取加载失败的错误信息。
	 *
	 * @return 动态库加载错误时系统提供的描述信息
	 */
	const std::string& GetLoadError() const;
	
	/**
	 * @brief 获取动态库中指定函数的地址。
	 *
	 * @param name 函数名称。
	 * @return 指定名称函数的指针。若找不到该名称的函数则返回NULL。
	 */
	void* GetProcAddress(const char* name) const;
	
	/**
	 * @brief 获取动态库所在的路径。
	 *
	 * @return 动态库所在的路径，可能是相对路径或绝对路径。
	 */
	const std::string& GetPath() const;

private:
	void* handle;
	
	std::string lib_path;

	std::string load_error;
};
typedef boost::shared_ptr<DynamicLibrary> DynamicLibraryPtr;

}

#endif
