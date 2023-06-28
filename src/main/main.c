#include "../log/log.h"
#include "../game/game.h"


int main(int argc, char *argv[])
{
    enable_log(argc, argv);

    if (argc > 1)
    {
        game_loop();
    }
    
    return 0;
}
