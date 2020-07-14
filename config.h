#pragma once

#include <map>
#include <string>
#include <fstream>
#include <memory>

class FileReader
{
    std::fstream m_File;
public:
    virtual ~FileReader();
    virtual void open();
    virtual std::pair<std::string, std::string> read();
    virtual void close();
};

class JsonFileReader: public FileReader
{
public:
    virtual std::pair<std::string, std::string> read();
};

class Config
{
    Config();
    Config(const Config&) = delete;
    void operator=(const Config&) = delete;
public:
    typedef std::string String;
    typedef std::map<String, String> Library;
    typedef std::shared_ptr<FileReader> ReaderPtr;

    static void init(ReaderPtr r);
    static Config & instance();
    void set(const String & key, const String & value);
    String get(const String & key)const;

private:
    Library m_Library;
};