#pragma once

#include <iostream>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <vector>

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>

using Poco::Net::ServerSocket;
using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::FIFOBuffer;
using Poco::Exception;

#include "CSingleton.h"
#include "Packets.h"
#include "CPacketHandler.h"
#include "CWhiteList.h"
#include "CActionManager.h"
#include "CNetwork.h"
#include "CServer.h"
