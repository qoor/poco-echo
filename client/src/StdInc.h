#pragma once

#include <iostream>
#include <algorithm>
#include <cassert>

#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/FIFOBufferStream.h>
#include <Poco/Runnable.h>

using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::Exception;
using Poco::FIFOBuffer;
using Poco::FIFOBufferStream;

#include "Packets.h"
#include "CNetwork.h"
#include "CSingleton.h"
#include "CPacketHandler.h"
#include "CClient.h"
