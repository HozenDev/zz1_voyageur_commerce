#include "../log/log.h"
#include "../game/game.h"
#include "../parallel/parallel.h"

int main(int argc, char *argv[])
{
    enable_log(argc, argv);

    if (argc > 1)
    {
        game_loop();
    }
    
    return 0;
}
