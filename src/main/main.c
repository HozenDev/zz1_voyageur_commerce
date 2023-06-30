#include "../log/log.h"
#include "../game/game.h"
#include "../parallel/parallel.h"

int main(int argc, char *argv[])
{
    int save = 1;
    int simulation = 0;
    long int seed;

    create_seed(&seed);
    
    enable_log(argc, argv);

    if (argc > 2)
    {
        if (strcmp(argv[2], "--load") == 0)
        {
            load_seed(&seed);
            save = 0;
            zlog(stdout, INFO, "chargement de la seed: %ld", seed);
        }
        else if (strcmp(argv[2], "--simulation") == 0)
        {
            simulation = 1;
            zlog(stdout, INFO, "Programme lancé en simulation", NULL);
        }
    }
    
    if (save) save_seed(seed);
    srand(seed);

    if (simulation) thread_main(5);
    else game_loop();
    
    return 0;
}
