/*
** EPITECH PROJECT, 2019
** JukeBox
** File description:
** JukeBox.cpp
*/

#include "JukeBox.hpp"
#include "Utility/Globpp.hpp"
#include "Error.hpp"

#include <iostream> // tmp for debug log
JukeBox::JukeBox(const string &musicDirectoryPath, const ushort &port)
    : ioService(),
      acceptor(ioService, TcpEndpoint(TcpV4(), port)),
      noNext(false)
{
    srand(time(nullptr));
    fileNameList = getAllFileName(musicDirectoryPath);

    for (const string &fileName : fileNameList)
        cerr << fileName << endl;

    if (fileNameList.empty())
        throw ERROR("PlayList is empty");
    next();
    listen();
    ioService.run();
}

JukeBox::~JukeBox()
{
    while (!threadList.empty()) { // TODO move in a thread call in ctor
        threadList.front().join();
        threadList.pop_front();
    }
}

const string JukeBox::getCurrentMusicPath() const
{
    return currentMusicPath;
}

void JukeBox::next()
{
    if (playList.empty())
        playList = fileNameList;
    const size_t npa = rand() % playList.size();
    const string musicPath = playList[npa];

    playList.erase(playList.begin() + npa);
    try {
        playMusic(musicPath);
    }
    catch (...) {
        next();
    }
}

void JukeBox::playMusic(const string &musicPath)
{
    if (openFromFile(musicPath)) {
        currentMusicPath = musicPath;
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
    vector<string> result;

    for (const string &fileName : fileNameList)
        if (fileName.back() == '/') {
            vector<string> list = getAllFileName(fileName);

            result.insert(result.end(), list.begin(), list.end());
        }
        else
            result.push_back(fileName);
    return result;
}

void JukeBox::autoNext()
{
    const string musicPathAtRun = currentMusicPath;

    while (true) {
        const Int64 toWait = (getDuration() - getPlayingOffset()).asMicroseconds();

        this_thread::sleep_for(chrono::microseconds(toWait));
        if (noNext)
            return;
        if (currentMusicPath != musicPathAtRun)
            return;
        if (getStatus() == SoundSource::Stopped) {
            next();
            return;
        }
    }
}
