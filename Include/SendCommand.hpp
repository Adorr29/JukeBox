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
    void onConnect(boost::system::error_code ec);
    void onRead(shared_ptr<string> data, boost::system::error_code ec);
    void onWrite(boost::system::error_code ec);

private:
    IoService ioService;
    TcpSocket socket;
    string command;
};
