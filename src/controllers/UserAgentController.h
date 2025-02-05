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

#include "../objects/HttpPacket.h"
#include "../Globals.h"
#include "../objects/SocketWrapper.h"


class UserAgentController
{
    public:
        static void SendResponse(const SocketWrapper* socketWrapper, HttpPacket* packet);
};



#endif //USERAGENTCONTROLLER_H
