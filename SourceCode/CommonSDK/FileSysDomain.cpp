#include <map>
#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include "FileSystem.h"
#include "Logging.h"

namespace fm {

static std::map<std::string, std::string> fs_domains;

void FileSysDomain::LoadLocalSetting(const std::string& path)
{
	std::string conf_path = path;
	if (conf_path.empty())
		conf_path = "FileSysDomain.conf";

	// Load file system domains from conf_path. 
	boost::system::error_code err;
	if ( !boost::filesystem::exists(conf_path, err) ) {
		if( getenv("FILESYS_DOMAIN") == NULL )
			return;
		FileSysDomain::LoadStringSetting(getenv("FILESYS_DOMAIN"));
		return;
	}

	std::ifstream conf_file(conf_path.c_str());
	if (!conf_file.is_open())
		throw std::runtime_error("FileSysDomain configuration file can not be opened.");
	
	std::string line;
	while(!conf_file.eof()) {
		getline(conf_file, line);
		std::size_t pos = line.find('=');
		if (pos == std::string::npos)
			continue;
		std::string domain = boost::trim_copy(line.substr(0, pos));
		std::string path   = boost::trim_copy(line.substr(pos+1));
		SetDomainPath(path, domain);
	}
	conf_file.close();
}

void FileSysDomain::LoadStringSetting(const std::string& setting)
{
	typedef boost::tokenizer< boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(";");
	tokenizer tokens(setting, sep);
	for(tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
		std::string domain_conf = *it;
		std::size_t pos = domain_conf.find('=');
		if (pos == std::string::npos)
			continue;
		std::string domain = boost::trim_copy(domain_conf.substr(0, pos));
		std::string path   = boost::trim_copy(domain_conf.substr(pos+1));
		FileSysDomain::SetDomainPath(path, domain);
	}
}

void FileSysDomain::SetDomainPath(const std::string& path, const std::string& domain)
{
	boost::system::error_code er;
	if ( !boost::filesystem::exists(path, er) )
		LOG_WARNING("FileSystem domain \""<<domain<<"\" is specified nonexistent path: "<<path);
	fs_domains[domain] = path;
}

std::string FileSysDomain::GetDomainPath(const std::string& domain)
{
	if (fs_domains.find(domain) != fs_domains.end())
		return fs_domains[domain];
	return "";
}

std::string FileSysDomain::AbridgePath(const std::string& file_path)
{
	std::map<std::string, std::string>::iterator it = fs_domains.begin();
	while (it != fs_domains.end()) {
#if defined(WIN32) || defined(_WINDOWS)
		if (boost::istarts_with(file_path, it->second)) {
#else
		if (boost::starts_with(file_path, it->second)) {
#endif
			return boost::str(boost::format("$[%s]%s") % it->first % file_path.substr(it->second.length()));
		}
		it++;
	}
	return file_path;
}

std::string FileSysDomain::ExtendPath(const std::string& file_path)
{
	if (!boost::starts_with(file_path, "$["))
		return file_path;
	std::string::size_type pos = file_path.find(']');
	std::string fs_path = FileSysDomain::GetDomainPath(file_path.substr(2, pos-2));
	if (fs_path.empty())
		throw std::runtime_error("File system domain not defined.");
	return fs_path+file_path.substr(pos+1);
}

}
