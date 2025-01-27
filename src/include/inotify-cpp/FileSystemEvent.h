#pragma once
#include <inotify-cpp/FileSystemAdapter.h>

#include <chrono>

namespace inotify {
class FileSystemEvent {
  public:
    FileSystemEvent(
        int wd,
        uint32_t mask,
        const inotifypp::filesystem::path& path,
        const std::chrono::steady_clock::time_point& eventTime);

  public:
    int wd;
    uint32_t mask;
    inotifypp::filesystem::path path;
    std::chrono::steady_clock::time_point eventTime;
};
}
