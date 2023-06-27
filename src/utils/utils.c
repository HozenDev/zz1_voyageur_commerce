#include "utils.h"


/**
 * \fn float distance(SDL_Point pt1, SDL_Point pt2)
 * \brief Retourne la distance entre 2 SDL Points
 * 
 *  
 * \param[in] SDL_Point pt1
 * \param[in] SDL_Point pt2
 * 
 * \return float : la distance est un nombre réelle
 * 
 */

float distance(SDL_Point pt1, SDL_Point pt2) 
{
    int distance_x = abs(pt2.x - pt1.x);
    int distance_y = abs(pt2.y - pt1.y);

    return  sqrtf(distance_x*distance_x + distance_y*distance_y);
}
