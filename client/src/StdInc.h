#pragma once

#include <iostream>
#include <algorithm>
#include <cassert>

#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>

using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::Exception;
using Poco::FIFOBuffer;

#include "Packets.h"
#include "CSingleton.h"
#include "CPacketHandler.h"
#include "CClient.h"
