#pragma once

#include <filesystem>
#include <optional>

namespace inotifypp
{
    namespace filesystem = std::filesystem;

    using error_code = std::error_code;

    template<typename T>
    using optional = std::optional<T>;

    inline constexpr auto nullopt() { return std::nullopt; }

}
