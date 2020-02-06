/*
** EPITECH PROJECT, 2019
** JukeBox
** File description:
** JukeBox.cpp
*/

#include "JukeBox.hpp"
#include "Utility/Globpp.hpp"
#include "Error.hpp"

JukeBox::JukeBox(const string &musicDirectoryPath, const ushort &port)
    : ioService(),
      acceptor(ioService, TcpEndpoint(TcpV4(), port)),
      noNext(false)
{
    srand(time(nullptr));
    flieNameList = getAllFileName(musicDirectoryPath);
    if (flieNameList.empty())
        throw ERROR("PlayList is empty");
    next();
    listen();
    ioService.run();
}

JukeBox::~JukeBox()
{
    while (!threadList.empty()) {
        threadList.front().join();
        threadList.pop_front();
    }
}

void JukeBox::next()
{
    if (playList.empty())
        playList = flieNameList;
    const size_t npa = rand() % playList.size();
    const bool success = openFromFile(playList[npa]);

    playList.erase(playList.begin() + npa);
    if (success) {
        play();
        threadList.emplace_back(&JukeBox::autoNext, this);
    }
    else
        next();
}

void JukeBox::playMusic(const string &musicPath)
{
    if (openFromFile(musicPath)) {
        play();
        threadList.emplace_back(&JukeBox::autoNext, this);
    }
    else
        throw string("\"" + musicPath + "\" can not be open");
}

void JukeBox::exit()
{
    noNext = true;
    stop();
    ioService.stop();
}

void JukeBox::listen()
{
    UserPtr user = make_shared<User>(ioService, *this);

    acceptor.async_accept(user->getSocket(),
                          boost::bind(&JukeBox::onAccept, this,
                                      user, ErrorPlaceholders));
}

void JukeBox::onAccept(UserPtr user, const ErrorCode &errorCode)
{
    if (!errorCode)
        user->run();
    listen();
}

vector<string> JukeBox::getAllFileName(const string &directoryPath)
{
    vector<string> fileNameList = globpp(directoryPath + "/*");

    for (const string &fileName : fileNameList)
        if (fileName.back() == '/') {
            vector<string> fileNameList2 = getAllFileName(fileName);

            fileNameList.insert(fileNameList.end(), fileNameList2.begin(), fileNameList2.end());
        }
    return fileNameList;
}

void JukeBox::autoNext()
{
    const Int64 toWait = (getDuration() - getPlayingOffset() + seconds(0.1)).asMicroseconds();

    this_thread::sleep_for(chrono::microseconds(toWait));
    if (noNext)
        return;
    if (getStatus() == SoundSource::Stopped)
        next();
}
