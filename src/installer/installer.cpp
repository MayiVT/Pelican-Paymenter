#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdlib>
#include <git2.h>
#include <ncurses.h>

namespace fs = std::filesystem;

struct Data {
    std::string language;
    std::string os;
    std::string version;
    std::string path;
};

void showMenu(const std::string &title, const std::vector<std::string> &options, std::string &selectedOption) {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE); // Enable special keys like arrow keys

    int highlight = 0;
    int choice;
    int numOptions = options.size();

    while (true) {
        clear();
        mvprintw(0, 0, "%s", title.c_str());

        for (int i = 0; i < numOptions; ++i) {
            if (i == highlight) {
                attron(A_REVERSE);
                mvprintw(i + 1, 0, "%s", options[i].c_str());
                attroff(A_REVERSE);
            } else {
                mvprintw(i + 1, 0, "%s", options[i].c_str());
            }
        }

        choice = getch();

        switch (choice) {
            case KEY_UP:
                highlight = (highlight - 1 + numOptions) % numOptions;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % numOptions;
                break;
            case 10:
                selectedOption = options[highlight];
                endwin();
                return;
            default:
                break;
        }
    }
}

void cloneRepository(const std::string &repoUrl, const std::string &clonePath) {
    git_libgit2_init();

    git_repository *repo = nullptr;
    int error = git_clone(&repo, repoUrl.c_str(), clonePath.c_str(), nullptr);

    if (error != 0) {
        const git_error *err = git_error_last();
        std::cerr << "Error cloning repository: " << (err ? err->message : "Unknown error") << std::endl;
        git_libgit2_shutdown();
        exit(1);
    }

    git_repository_free(repo);
    git_libgit2_shutdown();
}

std::string getTranslation(const std::string &language, const std::string &key) {
    if (language == "Spanish") {
        if (key == "choose_language") return "Por favor, elige un idioma:";
        if (key == "choose_version") return "Por favor, elige una versión:";
        if (key == "choose_os") return "Por favor, elige un sistema operativo:";
        if (key == "enter_path") return "Por favor, introduce la ruta donde está instalado Paymenter [/var/www/paymenter]:";
        if (key == "path_error") return "La ruta no existe. Inténtalo de nuevo:";
        if (key == "cloning") return "Clonando en ";
        if (key == "destination_exists") return "El directorio de destino ya existe. Limpiándolo...";
        if (key == "move_success") return "Archivos movidos exitosamente a ";
        if (key == "source_error") return "Error: El directorio fuente no existe: ";
    }

    // Default to English
    if (key == "choose_language") return "Please choose a language:";
    if (key == "choose_version") return "Please choose a version:";
    if (key == "choose_os") return "Please choose an OS:";
    if (key == "enter_path") return "Please enter the path where Paymenter is installed [/var/www/paymenter]:";
    if (key == "path_error") return "Path does not exist. Please try again:";
    if (key == "cloning") return "Cloning into ";
    if (key == "destination_exists") return "Destination directory already exists. Clearing it...";
    if (key == "move_success") return "Successfully moved files to ";
    if (key == "source_error") return "Error: Source directory does not exist: ";

    return key;
}

int main() {
    Data data;

    std::vector<std::string> languages = {"English", "Spanish"};
    std::vector<std::string> versions = {"v1", "v0 (do not use yet)"};
    std::vector<std::string> osOptions = {
        "Ubuntu 20.04", "Ubuntu 22.04", "Ubuntu 24.04", 
        "CentOS 7", "CentOS 8", "Debian 10", "Debian 11"
    };

    showMenu(getTranslation("", "choose_language"), languages, data.language);
    showMenu(getTranslation(data.language, "choose_version"), versions, data.version);
    showMenu(getTranslation(data.language, "choose_os"), osOptions, data.os);

    std::cout << getTranslation(data.language, "enter_path") << " ";
    std::getline(std::cin, data.path);
    if (data.path.empty()) {
        data.path = "/var/www/paymenter";
    }

    while (!fs::exists(data.path)) {
        std::cout << getTranslation(data.language, "path_error") << " ";
        std::getline(std::cin, data.path);
    }

    std::string tempPath = "/tmp/Pelican-Paymenter";

    std::cout << getTranslation(data.language, "cloning") << tempPath << "..." << std::endl;

    try {
        cloneRepository("https://github.com/MayiVT/Pelican-Paymenter", tempPath);

        fs::path source = tempPath + "/src/" + data.version + "/Pelican";
        fs::path destination = data.path + "/extensions/Servers/Pelican";

        if (fs::exists(destination)) {
            std::cout << getTranslation(data.language, "destination_exists") << std::endl;
            fs::remove_all(destination);
        }

        fs::create_directories(destination.parent_path());

        if (fs::exists(source)) {
            fs::rename(source, destination);
            std::cout << getTranslation(data.language, "move_success") << destination << std::endl;
        } else {
            std::cerr << getTranslation(data.language, "source_error") << source << std::endl;
        }

        fs::remove_all(tempPath);
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
