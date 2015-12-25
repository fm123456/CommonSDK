#ifndef _FM_SDK_XML_CONFIG_H_
#define _FM_SDK_XML_CONFIG_H_

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include "SystemExport.h"

namespace fm {

/**
 * @brief 访问基于XML文件或内存流的配置信息。
 *
 * XmlConfig 类用于读取、创建和修改基于XML文件或内存流存储的配置信息。
 * 该类提供了对指定配置项直接读写的能力，并且允许调用者以类似于路径的
 * 方式指定配置项的键值，例如“debug.level”键名称表示访问以下配置节点：
 *     <?xml version="1.0" encoding="GB2312"?>
 *     <debug>
 *         <level>2</level>
 *     ......
 */
class LIB_SDK XmlConfig
{
public:
	/**
	 * @brief 构造函数。
	 */
	XmlConfig();

	/**
	 * @brief 构造函数。
	 *
	 * @param xml  要加载的XML文件路径或XML内存流。
	 */
	XmlConfig(const std::string& xml);

	/**
	 * @brief 构造函数。
	 *
	 * @param path 要加载的XML文件路径或XML内存流。
	 */
	XmlConfig(const char* xml);

	/**
	 * @brief 析构函数。
	 */
	~XmlConfig();

	/**
	 * @brief 载入XML配置文件并替换原来的内容。
	 *
	 * @param path 要加载的XML文件路径。
	 */
	void LoadFile(const std::string& path);

	/**
	 * @brief 解析内存中的XML并替换原来的内容。
	 *
	 * @param xml 内存中的XML文档。
	 */
	void ParseXml(const std::string& xml);

	/**
	 * @brief 判断当前配置内容是否为空。
	 *
	 * @return 如果包含配置内容返回false，否则返回true。
	 */
	bool IsEmpty() const;

	/**
	 * @brief 以指定字符编码保存XML配置文件。
	 *
	 * @param encoding XML文件的字符编码类型，默认utf-8。
	 */
	void Save(const std::string& encoding = "utf-8");

	/**
	 * @brief 以指定字符编码将配置内容保存为一个新的XML文件。
	 *
	 * @param path 要保存的XML文件名。
	 * @param encoding XML文件的字符编码类型，默认utf-8。
	 */
	void SaveAs(const std::string& path, const std::string& encoding = "utf-8");

	/**
	 * @brief 检查指定的配置项是否存在。
	 *
	 * @param key 配置项的键名称，可用“.”分割表示不同级别。
	 * @return 配置项是否存在。
	 */
	bool Exists(const std::string& key) const;

	/**
	 * @brief 删除指定的配置项。
	 *
	 * @param key 配置项的键名称，可用“.”分割表示不同级别。
	 */
	void Delete(const std::string& key);

	/**
	 * @brief 获取指定配置项的值。
	 *
	 * @param key 配置项的键名称，可用“.”分割表示不同级别。
	 * @param value 获取配置项失败或该项不存在时返回的默认值。
	 * @return 配置项的值。
	 * @note 配置项值的类型由提供的默认值类型决定。若该配置项的值
	 *       无法转换为该类型，函数会抛出异常。
	 */
	template<typename T>
	T Get(const std::string& key, T value)
	{
		assert( !key.empty() );
		return config.get(key, value);
	}

	/**
	 * @brief 获取指定配置项的字符串值。
	 *
	 * @param key 配置项的键名称，可用“.”分割表示不同级别。
	 * @param value 获取配置项失败或该项不存在时返回的默认值。
	 * @return 配置项的字符串值。
	 * @note 是通用 Get 函数针对 const char* 默认值类型的重载函
	 *       数，使之可以返回 std::string 类型的配置项值。
	 */
	std::string Get(const std::string& key, const char* value)
	{
		assert (!key.empty());
		assert (value != NULL);
		return config.get(key, std::string(value));
	}

	/**
	 * @brief 获取指定配置项的多个值。
	 *
	 * @param [in] key 配置项的键名称，可用“.”分割表示不同级别。
	 * @param [out] values 配置项的值列表。
	 * @note 在XML配置文件中可以存在多个相同名称的配置节点，GetValues
	 *       函数以数组的形式返回所有可用的值（忽略不能转换为指定类型
	 *       的配置值）。
	 */
	template<typename T>
	void Get(const std::string& key, std::vector<T>& values)
	{
		assert (!key.empty());
		BOOST_FOREACH(boost::property_tree::ptree::value_type& v, config.get_child(key)) {
			try {
				values.push_back (boost::lexical_cast<T>(v.second.data()));
			}catch(std::exception&) { }
		}
	}
	
	/**
	 * @brief 设置指定配置项的值。
	 *
	 * @param key 配置项的键名称，可用“.”分割表示不同级别。
	 * @param value 配置项的值。
	 */
	template<typename T> 
	void Set(const std::string& key, const T& value)
	{
		assert (!key.empty());
		config.put(key, value);
	}

	/**
	 * @brief 设置指定配置项的多个值。
	 *
	 * @param key 配置项的键名称，可用“.”分割表示不同级别。
	 * @param begin 配置项值的起始迭代器。
	 * @param end 配置项值的结束迭代器。
	 */
	template<typename Iter>
	void Set(const std::string& key, Iter begin, Iter end)
	{
		assert (!key.empty());
		while (begin != end) {
			config.add(key, *begin);
			begin++;
		}
	}

	/**
	 * @brief 增加指定配置项的值。
	 *
	 * @param key 配置项的键名称，可用“.”分割表示不同级别。
	 * @param value 要增加的配置项的值。
	 * @note Add 同 Set 的不同之处在于它不会覆盖配置项原来的值，而是
	 *       将新的值追加到后面，形成一个多值配置项。
	 */
	template<typename T> 
	void Add(const std::string& key, const T& value)
	{
		assert (!key.empty());
		config.add(key, value);
	}
	
private:
	boost::property_tree::ptree config;

	std::string file_path;
};

}

#endif
