#pragma once

#include "system/beg.hpp"

#include <filesystem>
#include <fstream>
#include <vector>
#include "../command.hpp"
#include "network/request/handlers/io_set_file_content.hpp"
#include "shell/commands/download_upload/download_upload.hpp"

class UploadCommand final : public Command, public DownloadUpload {
  private:
    std::filesystem::path input_path{};
    std::filesystem::path path_parent{};
    std::vector<std::filesystem::path> input_files{};
    uint64_t buffer_size{};
    uint64_t file_size{};

    bool init();
    bool loadFiles();
    void getInputPath();
    bool checkInputPath() const;

    bool startTransmition() override;
    void loadBufferSize();
    bool uploadFile(const std::filesystem::path& path);

    IOSetFileContentRH getNewIOSetFileContentRH(const std::filesystem::path& path);
    std::filesystem::path getTargetPathFromSource(const std::filesystem::path& path) const;
    bool getInputFile(const std::filesystem::path& path, std::ifstream*& stream);

  public:
    UploadCommand() = default;

    exit_code_t run() override;
};
