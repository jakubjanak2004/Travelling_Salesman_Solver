#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <__filesystem/directory_entry.h>

#include "../graph/ts_instance.hpp"

class file_manager {
public:
    static const std::string INSTANCES_PATH;

    static const std::string RESULTS_PATH;

    static std::unique_ptr<ts_instance> read_dot_file(const std::string &file_name);

    static std::vector<std::filesystem::directory_entry> get_dot_instances(const std::string &directory_path);

    static void folder_exists_check();

    static void file_naming_conflicts_check(const std::string &file_name, std::string &output_file);

    static void save_solution(const std::string& file_name, const std::string& file_content);
};


#endif //FILEMANAGER_H
