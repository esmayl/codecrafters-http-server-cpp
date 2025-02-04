#include <iostream>
#include <string>
#include <cstring>
#include <filesystem>
#include <thread>
#include <unistd.h>
#include <vector>
#include <sys/types.h>

#include "WebServer.h"

// argv == parameter
// argc == amount of character executable call
int main(int argc, char **argv)
{
    WebServer webServer(4221);

    for (size_t i = 0; i < argc; i++)
    {
        // printf("%s\n",argv[i]);

        // Setup directory for using with FileController endpoint
        if(strcmp(argv[i],"--directory") == 0 && i + 1 < argc)
        {

            if(std::filesystem::exists(argv[i+1]))
            {
                printf("Setup directory: %s\n", argv[i+1]);
                webServer.SetupDirectory(argv[i+1]);
            }
            else
            {
                printf("Directory '%s\n' does not exist", argv[i+1]);
                return 1;
            }
            break;
        }
    }

    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cout << "Logs from your program will appear here!\n";

    if(webServer.Start() == 1)
    {
        return 1;
    }

    return 0;
}
