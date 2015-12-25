#include <sys/stat.h>
#if defined(WIN32) || defined(_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#else
	#define BOOST_NO_CXX11_SCOPED_ENUMS
#endif
#include <boost/filesystem.hpp>
#include "FileSystem.h"
namespace fs = boost::filesystem;

namespace fm {

std::string GetModulePath()
{
	const int MAX_PATH_LENGTH = 512;
	char module[MAX_PATH_LENGTH+1] = {0};
#if defined(WIN32) || defined(_WINDOWS)
	if( ::GetModuleFileNameA(NULL, module, MAX_PATH_LENGTH) == 0 )
		return "";
#else
	int count = readlink( "/proc/self/exe", module, MAX_PATH_LENGTH );
	if( count < 0 || count >= MAX_PATH_LENGTH )
		return "";
#endif
	fs::path fullPath(module);
	return fullPath.remove_filename().string();
}

std::string LIB_SDK GetCurrentPath()
{
	return fs::current_path().string();
}

std::string LIB_SDK GetTempPath()
{
	return fs::temp_directory_path().string();
}

std::string LIB_SDK GetUniquePath()
{
	return fs::unique_path().string();
}

std::string LIB_SDK Absolute(const std::string& path)
{
	return fs::absolute(path).string();
}

long long LIB_SDK FileSize(const std::string& path)
{
	boost::system::error_code ec;
    long long size = fs::file_size(path, ec);
	return ec ? -1 : size;
}

bool LIB_SDK MakeFileCopy(const std::string& src, const std::string& dest)
{
	boost::system::error_code ec;
	fs::copy_file(src,dest,ec);
	return !ec;
}

bool LIB_SDK MakeDirectoryCopy(const std::string& src, const std::string& dest)
{
	boost::system::error_code ec;
	fs::copy_directory(src, dest, ec);
	return !ec;
}

bool LIB_SDK ChangePathMode(const std::string& path, FileAccessMode mode, bool executable)
{
#if defined(WIN32) || defined(_WINDOWS)
	return true;
#else
	int file_mode = S_IRUSR|S_IWUSR|(executable?S_IXUSR:0)|
			        S_IRGRP|(executable?S_IXGRP:0)|
			        S_IROTH|(executable?S_IXOTH:0);
	if (mode == FileAccessProtected || mode == FileAccessPublic)
		file_mode |= S_IWGRP;
	if (mode == FileAccessPublic)
		file_mode |= S_IWOTH;
	return chmod(path.c_str(), file_mode) != -1;
#endif
}

bool LIB_SDK CreatePath(const std::string& path, FileAccessMode mode)
{
	boost::system::error_code ec;
	if (fs::exists(path, ec)) {
		ChangePathMode(path, mode, true);
		return true;
	}
	return fs::create_directory(path, ec) && ChangePathMode(path, mode, true);
}

bool LIB_SDK CreateAllPaths(const std::string& path, FileAccessMode mode)
{
	boost::system::error_code ec;
	if (fs::exists(path, ec)) {
		ChangePathMode(path, mode, true);
		return true;
	}
	return fs::create_directories(path, ec) && ChangePathMode(path, mode, true);
}

bool LIB_SDK Exists(const std::string& path)
{
	boost::system::error_code ec;
	return fs::exists(path, ec);
}

bool LIB_SDK IsDirectory(const std::string& path)
{
	boost::system::error_code ec;
	return fs::is_directory(path, ec);
}

bool LIB_SDK IsRegularFile(const std::string& path)
{
	boost::system::error_code ec;
	return fs::is_regular_file(path, ec);
}

bool LIB_SDK RemoveFile(const std::string& path)
{
	boost::system::error_code ec;
	return fs::remove(path, ec);
}

bool LIB_SDK RemovePath(const std::string& path)
{
	boost::system::error_code ec;
	return fs::remove_all(path,ec) != 0;
}

bool LIB_SDK RenamePath(const std::string& old_path, const std::string& new_path)
{
	boost::system::error_code ec;
	fs::rename(old_path, new_path, ec);
	return !ec;
}

bool LIB_SDK ResizeFile(const std::string& path, long long size)
{
	boost::system::error_code ec;
	fs::resize_file(path,size,ec);
	return !ec;
}

#define LINK_FILE_HEADER "Titan.LinkFile"

bool LIB_SDK CreateLinkFile(const std::string& link_file, const std::string& target_file)
{
	std::ofstream ofs(link_file.c_str());
	if (!ofs.is_open())
		return false;
	ofs<<LINK_FILE_HEADER<<std::endl;
	ofs<<"TargetFile = "<<Path(target_file).DomainPath()<<std::endl;
	return true;
}

Path LIB_SDK GetTargetFilePath(const Path& link_file)
{
	std::ifstream ifs(link_file.c_str());
	if (!ifs.is_open())
		return link_file;
	char header[64] = {0};
	ifs.read(header, sizeof(LINK_FILE_HEADER)-1);
	if (strcmp(header, LINK_FILE_HEADER) != 0)
		return link_file;

	std::string line;
	// skip the end of the first line
	std::getline(ifs, line);
	// read the target file
	std::getline(ifs, line);
	std::string::size_type pos = line.find("=");
	if (pos == std::string::npos)
		return link_file;
	return line.substr(pos+2);
}

Path::Path()
{
}

Path::Path(const std::string& p)
{
	if (boost::starts_with(p, "$["))
		path = FileSysDomain::ExtendPath(p);
	else
		path = p;
#if defined(WIN32) || defined(_WINDOWS)
	boost::replace_all(path, "/", "\\");
#endif
}

Path::Path(const char* p)
{
	if (boost::starts_with(p, "$["))
		path = FileSysDomain::ExtendPath(p);
	else
		path = p;
#if defined(WIN32) || defined(_WINDOWS)
	boost::replace_all(path, "/", "\\");
#endif
}

Path::Path(const Path& p) : path(p.path) 
{
}

Path& Path::operator=(const Path& p)
{
	path = p.path;
	return *this;
}

Path& Path::operator=(const std::string& p)
{
	if (boost::starts_with(p, "$["))
		path = FileSysDomain::ExtendPath(p);
	else
		path = p;
#if defined(WIN32) || defined(_WINDOWS)
	boost::replace_all(path, "/", "\\");
#endif
	return *this;
}

Path& Path::operator=(const char* p)
{
	if (boost::starts_with(p, "$["))
		path = FileSysDomain::ExtendPath(p);
	else
		path = p;
#if defined(WIN32) || defined(_WINDOWS)
	boost::replace_all(path, "/", "\\");
#endif
	return *this;
}

Path& Path::operator/=(const std::string& p)
{
	path = (fs::path(path) / p).string();
	return *this;
}

Path& Path::operator/=(const char* p)
{
	path = (fs::path(path) / p).string();
	return *this;
}

Path Path::operator/(const std::string& p)
{
	fs::path temp(path);
	temp /= p;
	return Path(temp.string());
}

Path Path::operator/(const char* p)
{
	fs::path temp(path);
	temp /= p;
	return Path(temp.string());
}

Path::operator std::string() const
{
	return path;
}

Path& Path::Clear()
{
	path.clear();
	return *this;
}

Path& Path::Append(const std::string& p)
{
	fs::path temp(path);
	path = temp.append<std::string>(p, temp.codecvt()).string();
	return *this;
}

Path& Path::RemoveFileName()
{
	path = fs::path(path).remove_filename().string();
	return *this;
}

Path& Path::ReplaceExtension(const std::string& extension)
{
	const fs::path p(extension);
	path = fs::path(path).replace_extension(p).string();
	return *this;
}

std::string Path::String() const
{
	return path;
}

const char* Path::c_str() const
{
	return path.c_str();
}

std::string Path::ParentPath() const
{
	return fs::path(path).parent_path().string();
}

std::string Path::DomainPath() const
{
	std::string domain_path = FileSysDomain::AbridgePath(path);
#if defined(WIN32) || defined(_WINDOWS)
	if (domain_path != path)
		boost::replace_all(domain_path, "\\", "/");
#endif
	return domain_path;
}

std::string Path::FileName() const
{
	return fs::path(path).filename().string();
}

std::string Path::Stem() const
{
	return fs::path(path).stem().string();
}

std::string Path::Extension() const
{
	return fs::path(path).extension().string();
}

bool Path::HasParentPath() const
{
	return fs::path(path).has_parent_path();
}

bool Path::HasFileName() const
{
	return fs::path(path).has_filename();
}

bool Path::HasStem() const
{
	return fs::path(path).has_stem();
}
    
bool Path::HasExtension() const
{
	return fs::path(path).has_extension();
}
 
bool Path::IsAbsolute() const
{
	return fs::path(path).is_absolute();
}

bool Path::IsEmpty() const
{
	return path.empty();
}

bool Path::IsUnderDomain() const
{
	std::string domain_path = FileSysDomain::AbridgePath(path);
	return domain_path != path;
}

bool Path::GetSubFileNames(std::vector<std::string>& names) const
{
	fs::directory_iterator it(path);
	if( it == fs::directory_iterator() )
		return false;
	for( ; it != fs::directory_iterator(); ++it )
		if( fs::is_regular_file( it->path() ) )
			names.push_back(it->path().filename().string());
	return true;
}

bool Path::GetSubDirectoryNames(std::vector<std::string>& names) const
{
	fs::directory_iterator it(path);
	if( it == fs::directory_iterator() )
		return false;
	for( ; it != fs::directory_iterator(); ++it )
		if(fs::is_directory( it->path() ) )
			names.push_back(it->path().filename().string());
	return true;
}

std::ostream& operator<<(std::ostream& os, const Path& path)
{
	os<<path.DomainPath();
	return os;
}

std::istream& operator>>(std::istream& is, Path& path)
{
	std::string p;
	is>>p;
	path = p;
	return is;
}

}
