#ifndef _FM_SDK_FILE_SYSTEM_H_
#define _FM_SDK_FILE_SYSTEM_H_

#include <string>
#include <vector>
#include <boost/serialization/string.hpp>
#include <boost/serialization/split_member.hpp>
#include "SystemExport.h"

namespace fm {

/**
 * @brief 实现文件系统逻辑域的概念及操作。
 *
 * FileSysDomain 类提供了在文件系统逻辑域及本地文件系统之间路径转换的功能。
 */
class LIB_SDK FileSysDomain
{
public:
	static void LoadLocalSetting(const std::string& path = "");

	static void LoadStringSetting(const std::string& setting);

	static void SetDomainPath(const std::string& path, const std::string& domain);

	static std::string GetLocalSetting();

	static std::string GetDomainPath(const std::string& domain);

	static std::string AbridgePath(const std::string& file_path);

	static std::string ExtendPath(const std::string& file_path);

	static void Clear();
};

/**
 * @brief 实现文件路径的常用操作。
 *
 * Path 类提供了多种对文件路径的操作功能，可方便地改变路径字符串，查询路径
 * 中的不同组成部分。
 */
class LIB_SDK Path
{
public:
	/**
	 * @brief 默认构造函数。
	 */
	Path();

	/**
	 * @brief 构造函数。
	 *
	 * @param p 路径字符串。
	 */
	Path(const std::string& p);

	/**
	 * @brief 构造函数。
	 *
	 * @param p 路径字符串。
	 */
	Path(const char* p);

	/**
	 * @brief 拷贝构造函数。
	 *
	 * @param p 路径对象。
	 */
	Path(const Path& p);

	/**
	 * @brief =运算符重载，复制另一个路径对象的内容。
	 *
	 * @param p 路径对象。
	 * @return 修改后的当前路径对象引用。
	 */
	Path& operator=(const Path& p);

	/**
	 * @brief =运算符重载，复制路径字符串的内容。
	 *
	 * @param p 路径字符串。
	 * @return 修改后的当前路径对象引用。
	 */
	Path& operator=(const std::string& p);

	/**
	 * @brief =运算符重载，复制路径字符串的内容。
	 *
	 * @param p 路径字符串。
	 * @return 修改后的当前路径对象引用。
	 */
	Path& operator=(const char* p);

	/**
	 * @brief /=运算符重载，将指定路径连接到当前路径之后。
	 *
	 * @param p 路径字符串。
	 * @return 修改后的当前路径对象引用。
	 */
	Path& operator/=(const std::string& p);

	/**
	 * @brief /=运算符重载，将指定路径连接到当前路径之后。
	 *
	 * @param p 路径字符串。
	 * @return 修改后的当前路径对象引用。
	 */
	Path& operator/=(const char* p);

	/**
	 * @brief /运算符重载，将指定路径连接到当前路径之后。
	 *
	 * @param p 路径字符串。
	 * @return 修改后的新路径对象。
	 */
	Path operator/(const std::string& p);

	/**
	 * @brief /运算符重载，将指定路径连接到当前路径之后。
	 *
	 * @param p 路径字符串。
	 * @return 修改后的新路径对象。
	 */
	Path operator/(const char* p);

	/**
	 * @brief 类型转换重载，获得路径完整字符串。
	 *
	 * @return 完整的路径字符串。
	 */
	operator std::string() const;

	/**
	 * @brief 清空路径内容。
	 *
	 * @return 修改后的当前路径对象引用。
	 */
	Path& Clear();

	/**
	 * @brief 将指定路径连接到当前路径之后。
	 *
	 * @param p 路径字符串。
	 * @return 修改后的当前路径对象引用。
	 */
	Path& Append(const std::string& p);

	/**
	 * @brief 将路径中的文件名移除。
	 *
	 * @return 修改后的当前路径对象引用。
	 */
	Path& RemoveFileName();

	/**
	 * @brief 替换路径中文件名的扩展名部分。
	 *
	 * @param extension 新的扩展名。
	 * @return 修改后的当前路径对象引用。
	 */
	Path& ReplaceExtension(const std::string& extension = "");
	
	/**
	 * @brief 获取路径的完整字符串。
	 *
	 * @return 路径的完整字符串。
	 */
	std::string String() const;

	/**
	 * @brief 获取路径的完整C字符串。
	 *
	 * @return 路径的完整C字符串。
	 */
	const char* c_str() const;

	/**
	 * @brief 获取上级路径。
	 *
	 * @return 上级路径字符串。
	 */
	std::string ParentPath() const;

	/**
	 * @brief 将当前路径转换为文件域路径。
	 *
	 * @return 文件域路径字符串。
	 * @note 若当前路径没有位于任何文件域下则直接返回当前路径。
	 */
	std::string DomainPath() const;

	/**
	 * @brief 获取路径中的文件名及扩展名部分。
	 *
	 * @return 文件名及扩展名字符串。
	 */
	std::string FileName() const;

	/**
	 * @brief 获取路径中的文件名部分。
	 *
	 * @return 文件名字符串。
	 */
	std::string Stem() const;

	/**
	 * @brief 获取路径中的扩展名部分（包含.)。
	 *
	 * @return 扩展名字符串。
	 */
	std::string Extension() const;

	/**
	 * @brief 判断是否存在上级路径。
	 */
	bool HasParentPath() const;

	/**
	 * @brief 判断是否包含文件名，即不是只有根目录。
	 */
    bool HasFileName() const;

	/**
	 * @brief 判断是否包含除扩展名以外的文件名。
	 */
    bool HasStem() const;

	/**
	 * @brief 判断是否包含扩展名。
	 */
	bool HasExtension() const;
 
	/**
	 * @brief 判断路径是否完整。
	 * 
	 * @note 不同操作系统可能判定的标准不一样，如在POSIX
	 *       上，Path("/foo").IsAbsolute()返回真，而在
	 *       windows上，Path("/foo").IsAbsolute()返回假。
	 */
	bool IsAbsolute() const;

	/**
	 * @brief 判断路径是否为空。
	 */
	bool IsEmpty() const;

	/**
	 * @brief 判断是否位于某个文件系统域下。
	 */
	bool IsUnderDomain() const;

	/**
	 * @brief 获取路径下所有的文件名（非递归)。
	 *
	 * @param names 文件名集合。
	 * @return 如果路径不存在，返回 false，否则返回 true。
	 */
	bool GetSubFileNames(std::vector<std::string>& names) const;

	/**
	 * @brief 获取路径下所有的目录名（非递归)。
	 *
	 * @param names 目录名集合。
	 * @return 如果路径不存在，返回 false，否则返回 true。
	 */
	bool GetSubDirectoryNames(std::vector<std::string>& names) const;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void save(Archive& ar, const unsigned int) const
	{
		std::string path = DomainPath();
		ar & path;
	}
	template<class Archive>
	void load(Archive& ar, const unsigned int)
	{
		std::string path;
		ar & path;
		*this = path;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()

	std::string path;
};
LIB_SDK std::ostream& operator<<(std::ostream& os, const Path& path);
LIB_SDK std::istream& operator>>(std::istream& is, Path& path);

/**
 * @brief 获得应用程序所在的路径。
 *
 * @return 应用程序所在路径。
 */
std::string LIB_SDK GetModulePath();

/**
 * @brief 获得应用程序的当前路径。
 * 
 * @return 应用程序的当前路径。
*/
std::string LIB_SDK GetCurrentPath();

/**
 * @brief 获得系统临时路径。
 *
 * @return 系统临时路径。
 */
std::string LIB_SDK GetTempPath();

/**
 * @brief 生成一个具有全局唯一性的文件路径。
 * 
 * @return 文件路径。
 */
std::string LIB_SDK GetUniquePath();

/**
 * @brief 将参数路径和当前路径组合成一个新的绝对路径。
 *
 * @param path 相对路径或绝对路径。
 * @return 绝对路径。
 */
std::string LIB_SDK Absolute(const std::string& path);

/**
 * @brief 获得文件大小。
 *
 * @param path 文件的路径。
 * @return 文件大小（字节为单位），-1表示获取失败。
 */
long long LIB_SDK FileSize(const std::string& path);

/**
 * @brief 创建指定文件的拷贝。
 *
 * @param src 源文件的路径。
 * @param dest 目的文件路径。
 */
bool LIB_SDK MakeFileCopy(const std::string& src, const std::string& dest);

/**
 * @brief 创建指定目录及其内容的拷贝。
 *  
 * @param src 源目录路径。
 * @param dest 目的目录路径。
 */
bool LIB_SDK MakeDirectoryCopy(const std::string& src, const std::string& dest);

typedef enum {
	FileAccessPrivate,
	FileAccessProtected,
	FileAccessPublic
} FileAccessMode;

bool LIB_SDK ChangePathMode(const std::string& path, FileAccessMode mode, bool executable);

/**
 * @brief 创建新目录。
 * 
 * @param path 新目录的路径。
 */
bool LIB_SDK CreatePath(const std::string& path, FileAccessMode mode = FileAccessProtected);

/**
 * @brief 创建新目录及所有不存在的中间目录。
 * 
 * @param path 新目录的路径。
 */
bool LIB_SDK CreateAllPaths(const std::string& path, FileAccessMode mode = FileAccessProtected);

/**
 * @brief 判断文件或目录是否存在。
 *
 * @param path 待判断的路径。
 */
bool LIB_SDK Exists(const std::string& path);

/**
 * @brief 判断指定路径是否为目录。
 * 
 * @param path 待判断的路径。
 */
bool LIB_SDK IsDirectory(const std::string& path);

/**
 * @brief 判断指定路径是否为普通文件。
 *
 * @param path 待判断的路径。
 */
bool LIB_SDK IsRegularFile(const std::string& path);

/**
 * @brief 删除指定的路径。如指定的是目录并且不为空则无法删除。
 * 
 * @param path 待删除的路径。
 */
bool LIB_SDK RemoveFile(const std::string& path);

/**
 * @brief 删除指定的路径。如指定的是目录则删除其下所有内容。
 *
 * @param path 待删除的路径。
 */
bool LIB_SDK RemovePath(const std::string& path);

/**
 * @brief 重命名路径。
 * 
 * @param old_path 原来的路径。
 * @param new_path 新的路径。
 */
bool LIB_SDK RenamePath(const std::string& old_path, const std::string& new_path);

/**
 * @brief 重置文件的大小。
 *
 * @param path 待操作的文件路径。
 * @param size 重置后的文件大小。
 */
bool LIB_SDK ResizeFile(const std::string& path, long long size);

/**
 * @brief 创建链接文件。
 *
 * @param link_file 链接文件路径。
 * @param target_file 指向的文件路径。
 */
bool LIB_SDK CreateLinkFile(const std::string& link_file, const std::string& target_file);

/**
 * @brief 获取链接文件指向的目标文件路径。
 *
 * @param link_file 链接文件路径。
 * @return 目标文件的路径对象。
 */
Path LIB_SDK GetTargetFilePath(const Path& link_file);

}

#endif
