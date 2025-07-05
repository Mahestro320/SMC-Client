#pragma once

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "io/directories.hpp"

struct ConfigValues {
    std::string server_address{};
    uint16_t server_port{};
    bool connect_at_launch{};
    bool enable_verbose_output{};
};

class Config final {
  private:
    static inline const std::filesystem::path INI_FILE_PATH{dirs::DATA / "config.ini"};

    boost::property_tree::ptree property_tree{};
    ConfigValues values{};

    bool loadIniFile();
    bool getValuesFromTree();

  public:
    Config() = default;

    static Config& getInstance();

    bool load();
    bool updateFile() const;

    const ConfigValues& getValues() const;
    const boost::property_tree::ptree& getPropertyTree() const;
    std::string getValue(const std::string& key) const;
    bool setValue(const std::string& key, const std::string& value);
};
