/*
** EPITECH PROJECT, 2019
** JukeBox
** File description:
** SendCommand.cpp
*/

#include <iostream>
#include "SendCommand.hpp"

SendCommand::SendCommand(const string &_command, const ushort &port, const string &address)
    : ioService(),
      socket(ioService),
      command(_command)
{
    doConnect(address, port);
    ioService.run();
}

void SendCommand::doConnect(const string &address, const ushort &port)
{
    socket.async_connect(TcpEndpoint(address::from_string(address), port),
                         boost::bind(&SendCommand::onConnect, this, ErrorPlaceholders));
}

void SendCommand::doRead()
{
    shared_ptr<string> data = make_shared<string>(DataSize, 0);

    socket.async_read_some(Buffer(*data, data->size()),
                           boost::bind(&SendCommand::onRead, this,
                                       data, ErrorPlaceholders));
}

void SendCommand::doWrite(const string &data)
{
    async_write(socket,
                Buffer(data.data(), data.size()),
                boost::bind(&SendCommand::onWrite, this, ErrorPlaceholders));
}

void SendCommand::onConnect(const ErrorCode &errorCode)
{
    if (!errorCode) {
        doRead();
        doWrite(command);
    }
    else
        cerr << "Connect failed" << endl;
}

void SendCommand::onRead(shared_ptr<string> data, const ErrorCode &errorCode)
{
    if (!errorCode)
        cout << *data << endl;
    else
        socket.close();
}

void SendCommand::onWrite(const ErrorCode &errorCode)
{
    if (!errorCode) {
        //
    }
    else
        socket.close();
}
