#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <stdio.h>

using namespace std;


vector<string> cut_up(char bash[])
{
    char seps[] = " ";
    char *token;

    vector<string> return_string;
    token = strtok( bash,seps);
    while(token != NULL)
    {
        return_string.push_back(token);
        token = strtok( NULL, seps );
    }

    return return_string;
}


string shell_start()
{
    cout << "Welcome to hunix, a virtual unix environment developed by Henry Walters\n";
    cout << "enter 'help' in the shell for basic assistance\n";
    cout << "enter 'features' for built in apps\n\n";
    cout << "Enter hunix\n1.) login\n2.) new user\n\n";
    cout << "input: ";
    int input;
    bool valid = false;
    string user = "";
    while (valid == false)
    {
        cin >> input;
        if (input == 1 || input == 2)
        {
            valid = true;
        }
        else
        {
            cout << "invalid input, reeneter\n";
        }
    }

    if (input == 1)
    {
        bool access_granted = false;
        while(access_granted == false)
        {
            string username, password;
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;

            cout << "\n";
            ifstream users;
            users.open("user_base.txt");

            vector<string> userData;
            string line;
            while (getline(users,line))
            {
                userData.push_back(line);
            }

            for (int i = 0; i < userData.size(); i++)
            {
                string x = userData[i];
                int len = x.size();
                char data[100];
                for (int i = 0; i < len; i++)
                {
                    data[i] = x[i];
                }
                vector<string> cut_data = cut_up(data);
                if (password == cut_data[1] && username == cut_data[0])
                {
                    cout << "Access Granted\n\n";
                    access_granted = true;
                    user = username + "@hunix";
                }

            }
            if (access_granted == false) { cout << "Access Denied\n\n";}
            users.close();
        }
    }
    else
    {
        bool valid = false;
        while (valid == false)
        {
            string username, password, password_check;
            cout << "Desired Username: ";
            cin >> username;
            cout << "password: ";
            cin >> password;
            cout << "confirm password: ";
            cin >> password_check;

            if (password != password_check)
            {
                cout << "Passwords Do not match, reenter\n";
            }
            else
            {
                ifstream users;
                users.open("user_base.txt");

                vector<string> userData;
                string line;
                while (getline(users,line))
                {
                    userData.push_back(line);
                }
                bool user_taken = false;
                for (int i = 0; i < userData.size(); i++)
                {
                    char x[100];
                    string y = userData[i];
                    for (int z = 0; z < y.size(); z++)
                    {
                        x[z] = y[z];
                    }
                    vector<string> data = cut_up(x);
                    if (username == data[0])
                    {
                        user_taken = true;
                    }
                }
                if (user_taken == true)
                {
                    cout << "username already exists, please reenenter \n";
                }
                else
                {
                    ofstream output;
                    output.open("user_base.txt", ios::app);
                    output << username << " " <<  password << "\n";
                    valid = true;
                    user = username + "@hunix";
                    output.close();
                }

            }
        }

    }
    return (user);
}

bool mkdir(string dir)
{
    ifstream input;
    ofstream output;
    input.open("dirs.txt");
    vector<string> dirs;
    string line;
    while (getline(input,line))
    {
        dirs.push_back(line);
    }
    bool exists = false;
    for (int i = 0; i < dirs.size(); i++)
    {
        if (dir == dirs[i])
        {
            exists = true;
        }
    }
    if (exists == true) { return false ;  }
    else
    {
        input.close();
        output.open("dirs.txt",ios::app);
        output << dir << "\n";
        output.close();
        return true;
    }
}

bool bash(string user)
{
    string dir = "";
    bool go = true;

    bool valid = false;
    string input = "";
    cout << user << ":~" + dir + "$ ";
    char x[100] = "";
    getline(cin,input);
    getline(cin,input);


    for (int i = 0; i < input.size(); i++)
    {
        x[i] = input[i];
    }
    vector<string> bash_cmd = cut_up(x);

    if (bash_cmd.size() == 1 && bash_cmd[0] == "logout")
    {
        go = false;
    }

    if (bash_cmd[0] == "mkdir" && bash_cmd.size() == 2)
    {
        bool success = mkdir(bash_cmd[1]);
        if (success == true)
        {
            valid = true;
        }
        else { valid = true; cout << "mkdir: cannot create directory: '" + bash_cmd[1] + "': file exists\n"; }
    }
    if (bash_cmd[0] == "mkdir" && bash_cmd.size() == 1)
    {
        cout << "mkdir: missing operand\n";
        valid = true;
    }

    if (valid == false)
    {
        cout << bash_cmd[0] << ":command not found\n";
    }
    else
    {
        cout << "\n";
    }

    bash_cmd.clear();
    return (go);
}


int main()
{
    bool running = true;
    while (running)
    {
        string user = shell_start();
        bool shell_running = true;
        while (shell_running == true)
        {
            shell_running = bash(user);
        }
    }
    return 0;
}
