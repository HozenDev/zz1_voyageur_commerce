#include "../log/log.h"
#include "../graph/graph.h"

int main(int argc, char *argv[])
{
    enable_log(argc, argv);
    
    graph_game_loop();
    
    return 0;
}
