#include "./command.h"
#include "./utils.hpp"
#include "./virfile.hpp"


class Application {
public:
    static void run(VFileSystem *system)
    {
        VFolder *workfolder = (VFolder*) system->subfile("root");
        VFile *workFile = nullptr;
        while (true) {
            std::string instr;
            std::cout << "[root@Admin " << workfolder->name() << "]# ";
            std::cin >> instr;
            if (instr == "help") {
                Command::help();
            } else if(instr == "ls") {
                Command::ls(workfolder);
            } else if (instr == "pwd") {
                Command::pwd(workfolder);
            } else if(instr == "cd") {
                std::cin >> instr;
                workfolder = Command::cd(workfolder, instr);
            } else if (instr == "mkdir") {
                std::cin >> instr;
                Command::mkdir(workfolder, new VFolder(instr));
            } else if (instr == "rmdir") {
                std::cin >> instr;
                Command::rmdir(workfolder, instr);
            } else if (instr == "create") {
                std::cin >> instr;
                Command::create(workfolder, new VFile(instr));
            } else if (instr == "rm") {
                std::cin >> instr;
                Command::rm(workfolder, instr);
            } else if (instr == "open") {
                std::cin >> instr;
                workFile = Command::open(workfolder, instr);
            } else if (instr == "chmod") {
                std::cin >> instr;
                std::string sinstr;
                std::cin >> sinstr;
                if(workfolder->contains(instr)) {
                    VIR* vir = workfolder->subfile(instr);
                    if(Command::chmod(vir, sinstr)) {
                        std::cout << "INFO: OK." << std::endl;
                    } else {
                        Command::help();
                    }
                } else {
                    std::cout << "INFO: Not Find File." << std::endl;
                }
            } else if (instr == "cat") {
                if(workFile != nullptr) {
                    Command::cat(workFile);
                } else {
                    std::cout << "INFO: Not Open File." << std::endl;
                }
            } else if (instr == "write") {
                if(workFile != nullptr) {
                    std::cin >> instr;
                    bool cover = true;
                    if(instr == "-a") {
                        cover = false;
                        std::cin >> instr;
                    }
                    if(instr.size() + system->size() >= system->maxsize()) {
                        std::cout << "WRAN: Out of memory." << std::endl;
                        continue;
                    }
                    Command::write(workFile, instr, cover);
                } else {
                    std::cout << "INFO: Not Open File." << std::endl;
                }
            } else if(instr == "mv") {
                std::cin >> instr;
                std::string sinstr;
                std::cin >> sinstr;
                if(workfolder->contains(instr)) {
                    VIR *vir = workfolder->subfile(instr);
                    Command::mv(system, vir, sinstr);
                } else {
                    std::cout << "INFO: Not Find File." << std::endl;
                }
            } else if(instr == "rename") {
                std::cin >> instr;
                std::string sinstr;
                std::cin >> sinstr;
                if(workfolder->contains(instr)) {
                    VIR* vir = workfolder->subfile(instr);
                    Command::rename(vir, sinstr);
                } else {
                    std::cout << "INFO: Not Find File." << std::endl;
                }
            } else if(instr == "vi") {
                workFile = nullptr;
            } else if (instr == "df") {
                Command::df(system);
            } else if (instr == "exit") {
                std::cout << "INFO: Are you sure you want to exit the system? (Y/N)" << std::endl;
                std::cout << "[root@Admin " << workfolder->name() << "]# ";
                std::cin >> instr;
                if (instr == "y" || instr == "Y") {
                    Command::exit();
                }
            } else {
                Command::help();
            }
        }
    };
};

int main(int argc, char *argv[]) {
    Application::run(new VFileSystem(4 * 1024));
}