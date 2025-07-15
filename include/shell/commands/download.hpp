#pragma once

#include <atomic>
#include <filesystem>
#include <fstream>
#include "../command.hpp"
#include "network/request/handlers/io_get_file_content.hpp"

class DownloadCommand final : public Command {
  private:
    std::filesystem::path input_path_raw{}, input_path_complete{}, input_path_real{};
    std::filesystem::path output_path{};
    std::ofstream* output_file{};
    uint64_t buffer_count{}, buffer_size{};
    IOGetFileContentRH handler{};

    std::atomic<uint64_t> curr_buffer_idx{};
    std::atomic<bool> transmition_stopped_flag{};
    std::atomic<bool> info_thread_stopped_flag{};

    void getBufferSize();
    bool getRawInputPath();
    bool getCompleteInputPath();
    bool getRealInputPath();
    bool getOutputPath();
    bool createOutputFile();
    bool beginHandler();

    void createInfoThread();
    void printInfoMessage();

    bool startTransmition();
    bool processNextBuffer();
    void stopInfoThread();
    void deleteFileInstance();

    std::filesystem::path getNewOutputFileNameInDir(const std::filesystem::path& path) const;

  public:
    DownloadCommand() = default;

    exit_code_t run() override;
};
