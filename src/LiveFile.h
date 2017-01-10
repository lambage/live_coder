#pragma once
#include <filesystem>

struct LiveFile
{
    explicit LiveFile(std::experimental::filesystem::path file);
    
    LiveFile& operator=(LiveFile&& other) noexcept;
    LiveFile(LiveFile&& other) noexcept;

    ~LiveFile() = default;

    bool IsExpired() const;
    void Reset();

    std::experimental::filesystem::path GetPath() const;

private:
    std::experimental::filesystem::path m_path;
    std::experimental::filesystem::file_time_type m_modified;
};

