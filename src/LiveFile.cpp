#include "LiveFile.h"

LiveFile::LiveFile(std::experimental::filesystem::path path) 
: m_path(path)
{
    Reset();
}

LiveFile::LiveFile(LiveFile&& other) noexcept 
: m_path(std::move(other.m_path)), m_modified(std::move(other.m_modified)) 
{
}

LiveFile& LiveFile::operator=(LiveFile&& other) noexcept
{
    if (this != &other)
    {
        m_path = std::move(other.m_path);
        m_modified = std::move(other.m_modified);
    }
    return *this;
}

bool LiveFile::IsExpired() const
{
    return m_modified < std::experimental::filesystem::last_write_time(m_path);
}

void LiveFile::Reset()
{
    m_modified = std::experimental::filesystem::last_write_time(m_path);
}

std::experimental::filesystem::path LiveFile::GetPath() const
{
    return m_path;
}
