/*
** EPITECH PROJECT, 2019
** JukeBox
** File description:
** User.hpp
*/

#pragma once

#include <memory>
#include "UsingBoost.hpp"

using namespace std;

class JukeBox;

class User : public enable_shared_from_this<User>
{
public:
    User(IoService &ioService, JukeBox &_jukeBox);
    TcpSocket &getSocket();
    void run();

private:
    void read();
    void write(const string &message);
    void onRead(shared_ptr<string> data, const ErrorCode &errorCode);
    void onWrite(const ErrorCode &errorCode);
    void pasreCommand(const vector<string> &dataTab);

private:
    TcpSocket socket;
    JukeBox &jukeBox;
};

typedef shared_ptr<User> UserPtr;

#include "JukeBox.hpp"
