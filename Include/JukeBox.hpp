/*
** EPITECH PROJECT, 2019
** JukeBox
** File description:
** JukeBox.hpp
*/

#pragma once

#include <list>
#include <thread>
#include <SFML/Audio.hpp>
#include "User.hpp"
#include "DefaultValue.hpp"

using namespace std;
using namespace sf;

class JukeBox : public Music
{
public:
    JukeBox(const string &musicDirectoryPath, const ushort &port = DefaultPort);
    ~JukeBox();
    const string getCurrentMusicPath() const;
    void next();
    void playMusic(const string &musicPath);
    void exit();

private:
    void listen();
    void onAccept(UserPtr user, const ErrorCode &errorCode);
    vector<string> getAllFileName(const string &directoryPath);
    void autoNext();

private:
    IoService ioService;
    TcpAcceptor acceptor;
    vector<string> fileNameList;
    vector<string> playList;
    list<thread> threadList;
    string currentMusicPath;
    bool noNext;
};
