//
//  main.cpp
//  MyShell
//
//  Created by Mohan Thapa on 9/7/23.
//
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

// Function to split a string into a vector of tokens
vector<string> split(const string& input, char delimiter) {
    vector<string> tokens;
    size_t start = 0, end = 0;
    while ((end = input.find(delimiter, start)) != string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(input.substr(start));
    return tokens;
}


// Built-in function to exit the shell
void exitShell() {
    exit(0);
}

// Built-in function to display help
void displayHelp() {
    cout << "Basic Shell Example" << endl;
    cout << "Supported Built-in Commands:" << endl;
    cout << "  cd <directory>: Change the current directory" << endl;
    cout << "  exit: Exit the shell" << endl;
    cout << "  help: Display this help message" << endl;
}

// Built-in function to display history
void displayHistory(const vector<int> & history){
    cout << "Child Process IDs History: "<< endl;
    
    for (int i =max(0,int(history.size()) -5); i< history.size(); ++i){
        cout << history[i] << endl;
    }
}



// main function
int main() {
    string input;
    vector<int> history;
    while (true) {
        cout << "mt779s$ ";
        getline(cin, input);

        vector<string> args = split(input, ' ');

        if (args.empty()) {
            continue;
        }

        
        if (args[0] == "exit") {
            exitShell();
        } else if (args[0] == "help") {
            displayHelp();
        }else if (args[0] == "history"){
            displayHistory(history);
        } else {
            // Fork a new process to execute external programs
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                exit(-1);
            } else if (pid == 0) {
                // Child process
                execlp(args[0].c_str(), args[0].c_str(), NULL);
                cout<<"ERROR -- \""+input+"\""+" No Such Command. "<<endl;
                exit(1);

                
            } else {
                // Parent process
                history.push_back(pid);
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }

    return 0;
}
