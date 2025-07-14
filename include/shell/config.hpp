#pragma once

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "io/directories.hpp"

struct ConfigValues {
    std::string server_address{};
    uint16_t server_port{};
    bool client_connect_at_launch{};
    bool shell_verbose{};
    bool shell_print_addr_prefix{};
    std::string cmd_download_output{};
    uint64_t cmd_download_buffer_size{};
};

class Config final {
  private:
    static inline const std::filesystem::path INI_FILE_PATH{dirs::DATA / "config.ini"};

    boost::property_tree::ptree property_tree{};
    ConfigValues values{};

    bool loadIniFile();
    bool getValuesFromTree();

    template<typename T> T getValueFromTree(const std::string& key);

  public:
    Config() = default;

    static Config& getInstance();

    bool load();
    bool updateFile() const;

    const ConfigValues& getValues() const;
    const boost::property_tree::ptree& getPropertyTree() const;
    std::string getValue(const std::string& key) const;
    std::string getResolvedValue(const std::string& key) const;
    bool setValue(const std::string& key, const std::string& value);
};
