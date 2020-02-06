/*
** EPITECH PROJECT, 2019
** JukeBox
** File description:
** Main.cpp
*/

#include <iostream>
#include <SFML/Audio.hpp>
#include "JukeBox.hpp"
#include "SendCommand.hpp"

using namespace std;

void help(const string &exe)
{
    cout << "Use : " << exe << " --run MusicDirectoryPath" << endl
         << "Or : " << exe << " Command [Port] [Address]" << endl
         << "Exemple : " << exe << " \"volume 30\" 8080" << endl << endl
         << "Command :" << endl
         << "    exit : stop JukeBox" << endl
         << "    pause : pauses the current music" << endl
         << "    play : play the current music" << endl
         << "    togglePlay : toggle play the current music" << endl
         << "    next : skip to the next music" << endl
         << "    playMusic MusicPath : play a specific music" << endl
         << "    volume : change volume (ex : \"50\" set to 50, \"+20\" up of 20, \"-5\" less of 5)" << endl;
}

int main(int argc, char **argv)
{
    if (argc == 2 && (string(argv[1]) == "-h" || string(argv[1]) == "--help"))
        help(argv[0]);
    else if (argc == 3 && (string(argv[1]) == "-r" || string(argv[1]) == "--run")) {
        if (fork() == 0)
            JukeBox jukeBox(argv[2]);
    }
    else if (argc == 2)
        SendCommand(string(argv[1]));
    else if (argc == 3)
        SendCommand(argv[1], atoi(argv[2]));
    else if (argc == 4)
        SendCommand(argv[1], atoi(argv[3]), argv[4]);
    else {
        help(argv[0]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
