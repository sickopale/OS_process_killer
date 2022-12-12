#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>

void ShowActiveProcesses(std::vector<std::string>& vec){
    std::cout << "\nPROCESSES STATISTIC: \n";
    bool flag = false;
    for(auto& el: vec){
        char buf[512];
        std::string command = "pidof -s " + el;
        FILE *cmd_pipe = popen(command.c_str(), "r");

        fgets(buf, 512, cmd_pipe);
        pid_t pid = strtoul(buf, NULL, 10);
        if(pid != 0 ) {
            flag = true;
            std::cout << el << " is running" << '\n';
        }
        pclose(cmd_pipe);
    }
    if(!flag){
        std::cout << "No processes from PROC_TO_KILL are running\n";
    }
}

int main() {
    setenv("PROC_TO_KILL", "telegram-desktop,firefox", 0);
    std::string proc_to_kill = getenv("PROC_TO_KILL");

    std::vector<std::string> processes;
    size_t pos = 0;
    std::string token;
    while ((pos = proc_to_kill.find(',')) != std::string::npos) {
        processes.push_back(proc_to_kill.substr(0, pos));
        proc_to_kill.erase(0, pos + 1);
    }
    processes.push_back(proc_to_kill.substr(0, pos));

    ShowActiveProcesses(processes);

    if (system(R"(../Killer/cmake-build-debug/Killer)")){
        std::cout << "The meow process is not created.\n";
        return 0;
    }
    ShowActiveProcesses(processes);
    clearenv();//delete
    return 0;
}