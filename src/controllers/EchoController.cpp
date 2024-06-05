//
// Created by esmayl on 05/06/2024.
//

#include "EchoController.h"

#include "../Globals.h"

void EchoController::SendResponse(SOCKET socket, HttpPacket packet)
{
    send(socket,Globals::successResponse,sizeof Globals::successResponse,0);
}
