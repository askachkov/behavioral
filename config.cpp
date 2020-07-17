#include "config.h"

FileReader::~FileReader()
{

}

std::map<std::string, std::string> FileReader::getData()
{
    std::map<std::string, std::string> ma;
    open();
    std::pair<std::string,std::string> res;
    do {
        res = read();
        if ( res.first.empty() ){
            close();
            return ma;
        } else {
            ma.insert(res);
        }
    } while ( true );
    return ma;
}

void FileReader::open()
{
    m_File.open("../config.cfg");
}

std::pair<std::string, std::string> FileReader::read()
{
    std::pair<std::string, std::string> res;
    m_File >> res.first >> res.second;
    return res;
}

void FileReader::close()
{
    m_File.close();
}

Config::Config()
{
}

void Config::init(ReaderPtr r)
{
    for ( auto p : r->getData() ){
        Config::instance().set(p.first, p.second);
    }
}

std::pair<std::string, std::string> JsonFileReader::read()
{
    return FileReader::read();
}

Config & Config::instance()
{
    static Config inst;
    return inst;
}

void Config::set(const String & key, const String & value)
{
    m_Library[key] = value;
}

Config::String Config::get(const String & key)const
{
    auto it = m_Library.find(key);
    if ( it != m_Library.end() ){
        return it->second;
    }
    return "";
}