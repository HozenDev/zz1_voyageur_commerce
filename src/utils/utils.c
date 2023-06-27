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


/**
 * \fn void free_matrix_char(char ** matrix, unsigned int n)
 * \brief Permet de libérer une matrice de char
 * 
 * 
 * \param[in] char ** matrix   : matrice à libérer 
 * \param[in] unsigned short n : taille de la matrice (carrée)
 * 
 * \return void : ne retourne rien
 * 
 */
void free_matrix_char(char ** matrix, unsigned int n)
{
    if (NULL != matrix)
    {
	for(unsigned int i=0; i<n; ++i)
	{
	    if (NULL != matrix[i])
	    {
		free(matrix[i]);
		matrix[i] = NULL;
	    }
	}
	free(matrix);
	matrix = NULL;
    }
}


/**
 * \fn void free_matrix_float(float ** matrix, unsigned int n)
 * \brief Permet de libérer une matrice de char
 * 
 * 
 * \param[in] float ** matrix   : matrice à libérer 
 * \param[in] unsigned short n : taille de la matrice (carrée)
 * 
 * \return void : ne retourne rien
 * 
 */
void free_matrix_float(float ** matrix, unsigned int n)
{
    if (NULL != matrix)
    {
	for(unsigned int i=0; i<n; ++i)
	{
	    if (NULL != matrix[i])
	    {
		free(matrix[i]);
		matrix[i] = NULL;
	    }
	}
	free(matrix);
	matrix = NULL;
    }
}


