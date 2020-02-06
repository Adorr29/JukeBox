/*
** EPITECH PROJECT, 2019
** JukeBox
** File description:
** SendCommand.hpp
*/

#pragma once

#include "UsingBoost.hpp"
#include "DefaultValue.hpp"

using namespace std;

class SendCommand
{
public:
    SendCommand(const string &_command, const ushort &port = DefaultPort, const string &address = "127.0.0.1");
    void doConnect(const string &address, const ushort &port);
    void doRead();
    void doWrite(const string &data);

private:
    void onConnect(const ErrorCode &errorCode);
    void onRead(shared_ptr<string> data, const ErrorCode &errorCode);
    void onWrite(const ErrorCode &errorCode);

private:
    IoService ioService;
    TcpSocket socket;
    string command;
};
