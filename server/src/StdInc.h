#pragma once

#include <iostream>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <Poco/Thread.h>

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/FIFOBufferStream.h>
#include <Poco/Runnable.h>

using Poco::Net::ServerSocket;
using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::FIFOBuffer;
using Poco::FIFOBufferStream;
using Poco::Exception;

#include "Packets.h"

#include "packets/CPacket.h"
#include "packets/CMessagePacket.h"
#include "packets/CMessageSendPacket.h"

#include "CSingleton.h"
#include "CPacketHandler.h"
#include "CWhiteList.h"
#include "CActionManager.h"
#include "CNetwork.h"
#include "CServer.h"
