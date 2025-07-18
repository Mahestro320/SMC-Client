#include "io/console.hpp"
#include "shell/config.hpp"
#include "util/env_var_str_resolver.hpp"

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
        values.server_address = getValueFromTree<std::string>("server.address");
        values.server_port = getValueFromTree<uint16_t>("server.port");
        values.client_connect_at_launch = getValueFromTree<bool>("client.connect_at_launch");
        values.shell_verbose = getValueFromTree<bool>("shell.verbose");
        values.shell_print_addr_prefix = getValueFromTree<bool>("shell.print_addr_prefix");
        values.cmd_download_output = getValueFromTree<std::string>("commands.download_output");
        values.cmd_download_buffer_size = getValueFromTree<uint64_t>("commands.download_buffer_size");
    } catch (const boost::property_tree::ptree_error& e) {
        console::out::err("error while getting config values from tree: " + std::string{e.what()});
        return false;
    }
    return true;
}

template<typename T> T Config::getValueFromTree(const std::string& key) {
    if constexpr (std::is_same_v<T, std::string>) {
        const std::string raw_val{property_tree.get<std::string>(key)};
        EnvVarStrResolver resolver{};
        resolver.setInput(raw_val);
        if (!resolver.resolve()) {
            throw std::runtime_error{"error while resolving key " + key};
        }
        return resolver.getOutput();
    } else {
        return property_tree.get<T>(key);
    }
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
    return opt_val && opt_val.get() != "" ? opt_val.get() : "unknown";
}

std::string Config::getResolvedValue(const std::string& key) const {
    auto opt_val{property_tree.get_optional<std::string>(key)};
    if (!opt_val || opt_val.get() == "") {
        return "unknown";
    }
    EnvVarStrResolver resolver{};
    resolver.setInput(opt_val.get());
    if (!resolver.resolve()) {
        return "unknown";
    }
    return resolver.getOutput();
}

bool Config::setValue(const std::string& key, const std::string& value) {
    property_tree.put(key, value);
    return getValuesFromTree();
}

const boost::property_tree::ptree& Config::getPropertyTree() const {
    return property_tree;
}
