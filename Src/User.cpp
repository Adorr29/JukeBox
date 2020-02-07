/*
** EPITECH PROJECT, 2019
** JukeBox
** File description:
** User.cpp
*/

#include <algorithm>
#include <regex>
#include "User.hpp"
#include "Error.hpp"
#include "Utility/Split.hpp"

User::User(IoService &ioService, JukeBox &_jukeBox)
    : socket(ioService),
      jukeBox(_jukeBox)
{
}

TcpSocket &User::getSocket()
{
    return socket;
}

void User::run()
{
    read();
}

void User::read()
{
    shared_ptr<string> data = make_shared<string>(DataSize, 0); // or unique_ptr // TODO

    socket.async_read_some(Buffer(*data, data->size()),
                           boost::bind(&User::onRead, shared_from_this(),
                                       data, ErrorPlaceholders));
    // or use but need to add a header with msg body size
    /*async_read(*socket,
      Buffer(header, header.lentgh()),
      boost::bind(&User::onRead, shared_from_this(), socket,
      data, ErrorPlaceholders));*/
}

void User::write(const string &message)
{
    async_write(socket, Buffer(message, message.size()),
                boost::bind(&User::onWrite, shared_from_this(),
                            ErrorPlaceholders));
}

void User::onRead(shared_ptr<string> data, const ErrorCode &errorCode)
{
    if (!errorCode) {
        *data = data->substr(0, data->find('\0'));
        const vector<string> dataTab = split(*data);

        /*if (dataTab.size() == 1 && dataTab[0] == "exit") {
            socket.close();
            return;
        }*/
        pasreCommand(dataTab);
        read();
    }
    else
        socket.close();
}

void User::onWrite(const ErrorCode &errorCode)
{
    if (errorCode)
        socket.close();
}

void User::pasreCommand(const vector<string> &dataTab)
{
    string result = "OK";
    smatch match;

    try {
        if (dataTab.size() == 1 && dataTab[0] == "exit")
            jukeBox.exit();
        else if (dataTab.size() == 1 && dataTab[0] == "pause")
            jukeBox.pause();
        else if (dataTab.size() == 1 && dataTab[0] == "play")
            jukeBox.play();
        else if (dataTab.size() == 1 && dataTab[0] == "togglePlay")
            jukeBox.getStatus() == SoundSource::Playing ? jukeBox.pause() : jukeBox.play();
        else if (dataTab.size() == 1 && dataTab[0] == "next")
            jukeBox.next();
        else if (dataTab.size() == 2 && dataTab[0] == "playMusic")
            jukeBox.playMusic(dataTab[1]);
        else if (dataTab.size() == 2 && dataTab[0] == "volume" && regex_search(dataTab[1], match, regex(R"(^([+-]|)(\d+)$)"))) {
            float volume = jukeBox.getVolume();
            const float volumeMin = 0;
            const float volumeMax = 100;

            if (match[1] == "-")
                volume -= stof(match[2]);
            else if (match[1] == "+")
                volume += stof(match[2]);
            else
                volume = stof(match[2]);
            volume = min(volumeMax, max(volumeMin, volume));
            jukeBox.setVolume(volume);
        }
        else
            result = "Command doesn't exist";
    }
    catch (string &error) {
        result = error;
    }
    write(result);
}
