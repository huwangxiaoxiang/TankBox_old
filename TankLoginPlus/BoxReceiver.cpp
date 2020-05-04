#include "pch.h"
#include "BoxReceiver.h"

BoxReceiver::BoxReceiver() :SocketClient(10824, "127.0.0.1")
{
}
