#pragma once

#include "system/beg.hpp"

#include <filesystem>
#include <fstream>
#include <vector>
#include "../command.hpp"
#include "download_upload/download_upload.hpp"
#include "network/request/handlers/io_get_file_content.hpp"

class DownloadCommand final : public Command, public DownloadUpload {
  private:
    std::filesystem::path input_path_raw{}, input_path_complete{}, input_path_real{};
    std::vector<std::filesystem::path> input_files{};

    bool init();
    bool getInputPath();
    void getInputPathRaw();
    bool getInputPathComplete();
    bool getInputPathReal();
    bool getInputFiles();
    std::vector<std::filesystem::path> getDirectoryContent() const;

    bool startTransmition() override;
    bool downloadFile(const std::filesystem::path& path);
    IOGetFileContentRH getNewIOGetFileContentRH(const std::filesystem::path& path);
    bool createOutputFile(const std::filesystem::path& path, std::ofstream*& file) const;
    std::filesystem::path getOutputPathFromSource(const std::filesystem::path& source) const;

  public:
    DownloadCommand() = default;
    ~DownloadCommand();

    exit_code_t run() override;
};
