//
//  main.cpp
//  project1
//
//  Created by Mohan Thapa on 9/8/23.
//

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <map>
using namespace std;
void child(string const cmd)
{
    const string vl = "/bin/"+cmd;
    execlp(vl.c_str(), cmd.c_str(), NULL);
    cout<<"ERROR -- \""+cmd+"\""+" No Such Command. Type \"help\" to see the supported command"<<endl;
    exit(1);
}
void parent(int child_pid)
{
int status;
    waitpid(child_pid, &status, 0);
}
struct mycmd
{
string name;
    string dis;
};
struct user {
    string name = "Adarsha";
    string MNum = "M03433866";
};
map<string, string> allCmd = {{"help" , "List all the built in command supported"},
    {"exit" , "Exit the program"},
    {"history" , "Shows Recent Five Executed Command"},
};
int main(void)
{
    vector<int> hist(5);
    user* cur = new user();
    string dir = cur->MNum + "$ ";
    string cmd = "";
    int counter = 0;
    while (1)
    {
        if (counter == 5)
        {
counter = 0; }
        cout << dir;
        getline(cin,cmd);
        // Help command print out all the command supported by the console.
        if (cmd == "help"){
            cout<<cur->name<<endl;
            cout<<"CSC360/660 Operating Systems"<<endl;
            cout<<"Project #2: My Shell - Writing Your Own Shell"<<endl;
            cout<<"This shell supports the following commands:"<<endl;
            for (auto it = allCmd.begin();it != allCmd.end(); ++it)
            {
                cout<<"|"<<endl;
                cout<<":--> ";
                cout<<it->first<<endl;
} }
        else if (cmd == "exit"){  // Exit Function Terminate the Console.
            cout<<"EXIT!"<<endl;
            break;
}
        // If the command is history
else if (cmd == "history")
    {
        for_each(hist.begin(), hist.end(), [](const int &str){
            cout<<str<<endl;
}); }
    // fork child and execute program
    else {
        pid_t child_pid = fork();
        if (child_pid == 0) {
            // Child proces
            child(cmd);
        } else if (child_pid > 0) {
            // Parent process
            parent(child_pid);
            hist[counter] = child_pid;
            counter += 1;
            // Check status and handle any errors
        } else {
            // Handle fork error
            perror("Error");
            exit(1);
} }
} }
