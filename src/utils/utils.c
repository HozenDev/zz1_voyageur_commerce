#include "utils.h"

const struct colors_s colors_available = 
{
    .BLACK    = (SDL_Color) {30, 30, 30, 255},
    .WHITE    = (SDL_Color) {220, 220, 220, 255},
    .RED = (SDL_Color) {255, 100, 100, 255},
    .GREEN  = (SDL_Color) {50, 200, 50, 255},
    .BLUE = (SDL_Color) {100, 100, 255, 255},
    .YELLOW = (SDL_Color) {255, 255, 100, 255}
};

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

/**
 * \fn int max(int a, int b)
 * \brief Permet de connaître le max entre 2 entiers
 * 
 * 
 * \param[in] int a 
 * \param[in] int b
 * 
 * \return int : retourne le plus grand entier
 * 
 */
int max(int a, int b)
{
    return (a > b) ? a : b;
}

/**
 * \fn int min(int a, int b)
 * \brief Permet de connaître le max entre 2 entiers
 * 
 * 
 * \param[in] int a 
 * \param[in] int b
 * 
 * \return int : retourne le plus petit entier
 * 
 */
int min(int a, int b)
{
    return (a > b) ? b : a;
}

/**
 * \fn int generate_random_number(int min, int max)
 * \brief Permet de générer un nombre entre min et max
 * 
 * 
 * \param[in] int min
 * \param[in] int max
 * 
 * \return int : retourne le nombre généré
 * 
 */
int generate_random_number(int min, int max)
{
    return rand()%(max-min) + min;
}

/**
 * @brief Creates a copy of a matrix
 *
 * @param float ** srxmatrix ,source matrix
 * @param float ** destmatrix ,destination matrix
 * @param int n ,matrixes size
 */
void utils_matrix_copy(float ** srxmatrix,float ** destmatrix,int n)
{
    for (int i=0;i<n;i++){
        for(int j=i + 1; j<n;j++){
            destmatrix[i][j]=srxmatrix[i][j];
        }
    }
}
void utils_swap(int*a,int*b){
	int tmp;
	tmp=*a;
	*a=*b;
	*b=tmp;

}
void utils_shuffle(int* liste,int taille,int temperature){
	for(int i=0;i<taille;i++){
		utils_swap(&liste[rand()%taille],&liste[i]);
	}
	
}
void utils_distance_liste(int * indice,float ** distance,float * dist,int taille)
{
	for(int i=0;i<taille-1;i++)
	{
		*dist+=distance[indice[i]][indice[i+1]];
	}
}
void utils_copy_list(int *listsrc,int* listdst,int taille){
	for(int i=0;i<taille;i++)
	{
		listdst[i]=listsrc[i];
	}
}
void utils_initlist0(int * list,int taille){
	for(int i=0;i<taille;i++){
		list[i]=0;
	}
}
