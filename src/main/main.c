#include "../log/log.h"

int main(int argc, char *argv[])
{
    enable_log(argc, argv);
    
    zlog(stdout, INFO, "Faire le voyageur commerce", NULL);
    return 0;
}
