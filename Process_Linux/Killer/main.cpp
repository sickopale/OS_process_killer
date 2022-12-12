#include <iostream>
#include <string>
#include <csignal>
#include <sys/types.h>
#include <cstdlib>

bool KillProcessById(pid_t pid){
    if(pid == 0){
        std::cout << "couldn't find process";
        return false;
    }
    kill(pid, SIGTERM) == 0;
    return true;
}

bool KillProcessByName(std::string processName) {
    bool success = false;
    char buf[512];
    std::string command = "pidof -s " + processName;
    FILE *cmd_pipe = popen(command.c_str(), "r");

    fgets(buf, 512, cmd_pipe);
    pid_t pid = strtoul(buf, NULL, 10);
    if(pid!=0) {
        command = "killall "+processName;
        if (!system(command.c_str())){
            success = true;
        }
    }
    pclose(cmd_pipe);
    return success;
}

int main() {
    std::cout << "==================================================================\n";
    std::cout << "Welcome to Killer.exe!!\n";
    std::cout << "Would you...\n";
    int answ;
    std::cout << "1 -- kill process by name\n";
    std::cout << "2 -- kill process by id\n";
    std::cout << "3 -- kill all processes in PROC_TO_KILL\n";
    std::cout << "4 -- exit\n";
    while(true){
        std::cout << "\nChoose option from the menu: ";
        std::cin >> answ;
        switch (answ) {
            case 1:{
                std::cout << "\nEnter process name: ";
                std::string procName;
                std::cin >> procName;
                if(KillProcessByName(procName)){
                    std::cout << "Success!\n";
                } else{
                    std::cout << ":(\n";
                }
                break;
            }
            case 2:{
                std::cout << "\nEnter process id: ";
                int procId;
                std::cin >> procId;

                if(KillProcessById(procId)){
                    std::cout << "Success!\n";
                } else{
                    std::cout << ":(\n";
                }
                break;
            }
            case 3:{
                std::string proc_to_kill = getenv("PROC_TO_KILL");

                size_t pos = 0;
                std::string token;
                while ((pos = proc_to_kill.find(',')) != std::string::npos) {
                    KillProcessByName(proc_to_kill.substr(0, pos));
                    proc_to_kill.erase(0, pos + 1);
                }
                KillProcessByName(proc_to_kill.substr(0, pos));
                break;
            }
            case 4:{
                std::cout << "bye! :)\n";
                std::cout << "==================================================================\n";
                return 0;
            }
        }

    }
}
