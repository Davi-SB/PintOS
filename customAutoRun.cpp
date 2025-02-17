#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream> 
#include <sstream> 
#include <cstdlib> 

namespace fs = std::filesystem;

std::set<std::string> getFilesToKeep(const std::vector<std::string>& filenames) {
    std::set<std::string> filesToKeep;
    for (const auto& name : filenames) {
        filesToKeep.insert(fs::path(name).stem().string());
    }
    return filesToKeep;
}

void deleteUnlistedFiles(const fs::path& directory, const std::set<std::string>& filesToKeep) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_regular_file(entry.path())) {
            std::string filenameWithoutExtension = entry.path().stem().string();
            if (filesToKeep.find(filenameWithoutExtension) == filesToKeep.end()) {
                fs::remove(entry.path());
                std::cout << "Deleted: " << entry.path() << std::endl;
            }
        }
    }
}

std::string getResultFromFile(const fs::path& resultFile) {
    std::ifstream file(resultFile);
    if (file.is_open()) {
        std::string firstWord;
        file >> firstWord;
        file.close();
        return firstWord;
    } else {
        std::cerr << "Erro ao abrir o arquivo: " << resultFile << std::endl;
        return "Erro";
    }
}

int main() {
    std::vector<std::string> commands = {
        "make clean",
        "make",
        "clear",
        "make ngui TEST=alarm-single",
        "make ngui TEST=alarm-multiple",
        "make ngui TEST=alarm-simultaneous",
        "make ngui TEST=alarm-zero",
        "make ngui TEST=alarm-negative",
        "make ngui TEST=mlfqs-load-1",
        "make ngui TEST=mlfqs-load-60",
        "make ngui TEST=mlfqs-load-avg",
        "make ngui TEST=mlfqs-recent-1",
        "make ngui TEST=mlfqs-fair-2",
        "make ngui TEST=mlfqs-fair-20",
        "make ngui TEST=mlfqs-nice-2",
        "make ngui TEST=mlfqs-nice-10",
        "make ngui TEST=mlfqs-block"
    };

    fs::path buildDirectory = "/home/davi/Área de Trabalho/PintOS/src/threads";

    if (fs::exists(buildDirectory) && fs::is_directory(buildDirectory)) {
        fs::current_path(buildDirectory);
    } else {
        std::cerr << "Erro: Diretório não encontrado!" << std::endl;
        return 1;
    }

    for (const auto& cmd : commands) {
        int result = system(cmd.c_str());
        if (result != 0) {
            std::cerr << "Erro ao executar comando: " << cmd << std::endl;
            return 1;
        }
    }

    fs::path directory = "/home/davi/Área de Trabalho/PintOS/src/threads/build/tests/threads";

    std::vector<std::string> filenames = { 
        "alarm-single", "alarm-multiple", "alarm-simultaneous", "alarm-zero", "alarm-negative", 
        "mlfqs-load-1", "mlfqs-load-60", "mlfqs-load-avg", "mlfqs-recent-1", "mlfqs-fair-2", 
        "mlfqs-fair-20", "mlfqs-nice-2", "mlfqs-nice-10", "mlfqs-block" 
    };

    std::set<std::string> filesToKeep = getFilesToKeep(filenames);

    deleteUnlistedFiles(directory, filesToKeep);

    std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
    for (const auto& filename : filenames) {
        fs::path resultFile = directory / (filename + ".result");
        std::string result = getResultFromFile(resultFile);
        if(filename == "mlfqs-load-1") std::cout << std::endl;
        std::cout << result << " - " << filename << std::endl;
    }

    return 0;
}
