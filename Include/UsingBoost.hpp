/*
** EPITECH PROJECT, 2019
** JukeBox
** File description:
** UsingBoost.hpp
*/

#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::address;


typedef boost::asio::io_service IoService;
typedef boost::asio::ip::tcp::endpoint TcpEndpoint;
typedef boost::asio::ip::tcp::acceptor TcpAcceptor;
typedef boost::asio::ip::tcp::socket TcpSocket;
typedef boost::system::error_code ErrorCode;

#define ErrorPlaceholders boost::asio::placeholders::error
#define Buffer boost::asio::buffer
#define TcpV4 boost::asio::ip::tcp::v4
