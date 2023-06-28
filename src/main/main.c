#include "../log/log.h"
#include "../game/game.h"

int main(int argc, char *argv[])
{
    enable_log(argc, argv);
    
    game_loop();
    
    return 0;
}
