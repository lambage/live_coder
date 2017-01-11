#pragma once

#include <cinder/Json.h>

namespace Settings
{
    using FileMap = std::map<const std::string, std::string>;

    struct DataImports
    {
        explicit DataImports(cinder::JsonTree sub_tree)
        {
            for (auto& child : sub_tree)
            {
                data_files.insert({ child.getKey(), child.getValue() });
            }
        }

        bool DataFileExists(std::string key) const { return data_files.count(key) > 0; }
        std::string GetDataFile(std::string key) const { return data_files.at(key); }
        
        FileMap& GetDataFiles() { return data_files; }

        bool operator==(const DataImports& rhs)
        {
            return data_files.size() == rhs.data_files.size()
            && std::equal(data_files.begin(), data_files.end(),
                rhs.data_files.begin());
        }

        bool operator!=(const DataImports& rhs)
        {
            return !(*this == rhs);
        }
    private:
        FileMap data_files;
    };
}
