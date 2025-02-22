#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <vector>
#include <sys/types.h>

#include "WebServer.h"

// argv == parameter
// argc == amount of character executable call
int main(int argc, char **argv)
{
    WebServer webServer(4221);

    if (argc <= 1)
    {
        printf( "File controller will not work without directory. \nUsage: server.exe %s", " --directory C:/ \n\n");
    }
    else
    {
        for (size_t i = 0; i < argc; i++)
        {
            // printf("%s\n",argv[i]);

            // Setup directory for using with FileController endpoint
            if(strcmp(argv[i],"--directory") == 0 && i + 1 < argc)
            {
                if (webServer.SetupDirectory(argv[i+1]))
                {
                    printf("Setup directory: %s\n", argv[i+1]);
                    break;
                }
                else
                {
                    printf("Directory '%s\n' does not exist", argv[i+1]);
                    return 1;
                }
                break;
            }
        }
    }

    printf("Logs from your program will appear here!\n");

    if(webServer.Start() == 1)
    {
        return 1;
    }

    return 0;
}
