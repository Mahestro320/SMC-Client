#pragma once

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

struct ConfigValues {
    std::string server_address{};
    uint16_t server_port{};

    bool client_connect_at_launch{};

    bool shell_verbose{};
    bool shell_print_addr_prefix{};

    std::string cmd_download_output{};
    uint64_t cmd_download_buffer_size{};
    bool cmd_download_del_file_on_stop{};
};

class Config final {
  private:
    boost::property_tree::ptree property_tree{};
    ConfigValues values{};

    bool loadIniFile();
    bool tryToLoadValuesFromTree();
    void loadValuesFromTree();

    template<typename T> T getValueFromTree(const std::string& key);

    Config() = default;

  public:
    static Config& getInstance();

    bool load();
    bool updateFile() const;

    const ConfigValues& getValues() const;
    const boost::property_tree::ptree& getPropertyTree() const;
    std::string getValue(const std::string& key) const;
    std::string getResolvedValue(const std::string& key) const;
    bool setValue(const std::string& key, const std::string& value);
};
