#include "io/console.hpp"
#include "shell/config.hpp"

using boost::property_tree::ptree;

Config& Config::getInstance() {
    static Config instance{};
    return instance;
}

bool Config::load() {
    return loadIniFile() && getValuesFromTree();
}

bool Config::loadIniFile() {
    console::out::inf("loading config file");
    try {
        boost::property_tree::ini_parser::read_ini(INI_FILE_PATH.string(), property_tree);
    } catch (const boost::property_tree::ini_parser_error& e) {
        console::out::err("error while parsing file: " + std::string{e.what()});
        return false;
    }
    return true;
}

bool Config::getValuesFromTree() {
    try {
        values.server_address = property_tree.get<std::string>("server.address");
        values.server_port = property_tree.get<uint16_t>("server.port");
        values.connect_at_launch = property_tree.get<bool>("server.connect_at_launch");
        values.enable_verbose_output = property_tree.get<bool>("stdout.verbose");
    } catch (const boost::property_tree::ptree_error& e) {
        console::out::err("error while getting config values from tree: " + std::string{e.what()});
        return false;
    }
    return true;
}

bool Config::updateFile() const {
    try {
        boost::property_tree::write_ini(INI_FILE_PATH.string(), property_tree);
    } catch (const boost::property_tree::ini_parser_error& e) {
        console::out::err("error while updating file: " + std::string{e.what()});
        return false;
    }
    return true;
}

const ConfigValues& Config::getValues() const {
    return values;
}

std::string Config::getValue(const std::string& key) const {
    auto opt_val{property_tree.get_optional<std::string>(key)};
    return opt_val ? opt_val.get() : "unknown";
}

bool Config::setValue(const std::string& key, const std::string& value) {
    property_tree.put(key, value);
    return getValuesFromTree();
}

const boost::property_tree::ptree& Config::getPropertyTree() const {
    return property_tree;
}
