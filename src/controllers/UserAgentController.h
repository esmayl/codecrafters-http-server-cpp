//
// Created by esmayl on 05/06/2024.
//

#ifndef USERAGENTCONTROLLER_H
#define USERAGENTCONTROLLER_H

#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sys/types.h>

#include "../Globals.h"
#include "../objects/HttpPacket.h"
#include "../objects/SocketWrapper.h"


class UserAgentController
{
    public:
        static void GetResponse(HttpPacket* packet, SocketWrapper* connectedClient);
};



#endif //USERAGENTCONTROLLER_H
