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
#include <unistd.h>
#include <sys/types.h>

#include "../HttpPacket.h"
#include "../Globals.h"
#include "../SocketWrapper.h"


class UserAgentController
{
    public:
        static void SendResponse(SocketWrapper* socketWrapper, HttpPacket* packet);
};



#endif //USERAGENTCONTROLLER_H
