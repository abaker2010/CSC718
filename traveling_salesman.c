#include <stdio.h>
#include <stdlib.h>


typedef struct tour {
    int *path;
    int weight;
} Tour;

Tour *newTour(int *path, int weight) {
    Tour *tour = (Tour *) malloc(sizeof(Tour));
    tour->path = path;
    tour->weight = weight;
    return tour;
}

typedef struct travelInfo {
    int num_cities;
    int **matrix;
    int best_tour_cost;
    int current_tour;
    Tour *all_tours;
} TravelInfo;

unsigned int factorial(unsigned int n)
{
    if (n == 1) {
      return 1;
    }
   
    return n * factorial(n - 1);
}

TravelInfo *newTravelInfo(int num_cities, int matrix[num_cities][num_cities]) {
    TravelInfo *travelInfo = (TravelInfo *) malloc(sizeof(TravelInfo));
    
    travelInfo->num_cities = num_cities;

    // Allocate memory for the matrix
    travelInfo->matrix = (int **) malloc(sizeof(int *) * num_cities);
    // Allocate memory for the all_tours array
    travelInfo->all_tours = (Tour *) malloc(sizeof(Tour) * factorial(num_cities));
    travelInfo->best_tour_cost = -1;
    travelInfo->current_tour = 0;   

    // Initialize the matrix, visited_cities, best_tour_cost, and best_tours arrays
    for (int i = 0; i < num_cities; i++) {    
        travelInfo->all_tours[i] = *newTour(NULL, -1);
        travelInfo->matrix[i] = (int *) malloc(sizeof(int) * num_cities);
        for (int j = 0; j < num_cities + 1; j++) {
            if (j == num_cities + 1) {
                travelInfo->matrix[i][j] = 0;
            }else{
                travelInfo->matrix[i][j] = matrix[i][j];
            }
        }
    }
    
    return travelInfo;
}

// Function to swap two elements in an array
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to calculate the total weight of a path
int check_path_cost(TravelInfo *ti, int path[])
{
    int weight = 0;
    for (int i = 0; i < ti->num_cities - 1; i++) {
        weight += ti->matrix[path[i]][path[i + 1]];
    }
    weight += ti->matrix[path[ti->num_cities - 1]][path[0]]; // Return to the starting city
    ti->all_tours[ti->current_tour] = *newTour(path, weight);
    ti->current_tour++;
    return weight;
}

// Function to generate all possible paths, get weight of each path, and store the best path
void gen_perms(TravelInfo *ti, int path[], int currentIndex) {
    if (currentIndex == ti->num_cities - 1) {
        // Calculate the weight of the current path
        int currentWeight = check_path_cost(ti, path);

        // Print the current path and its weight
        printf("Path: ");
        for (int i = 0; i < ti->num_cities; i++) {
            printf("%d ", path[i]);
        }
        printf("-> Total Weight: %d\n", currentWeight);
        return;
    }

    for (int i = currentIndex; i < ti->num_cities; i++) {
        // Swap the current element with itself and all the subsequent elements
        swap(&path[currentIndex], &path[i]);

        // Recursively generate paths
        gen_perms(ti, path, currentIndex + 1);

        // Backtrack
        swap(&path[currentIndex], &path[i]);
    }
}   


int main()
{
    int i, j, num_cities;

    printf("Enter Total Number of Cities:\t");
    scanf("%d", &num_cities);

    // Calculate the number of permutations (numCities!)
    int numPermutations = 1;
    for (int i = num_cities - 1; i > 1; i--) {
        numPermutations *= i;
    }


    int matrix[num_cities][num_cities];
    int tours[numPermutations][num_cities];
    // int visited_cities[num_cities];

    printf("\nEnter Cost Matrix\n");
    for (i = 0; i < num_cities; i++)
    {
        printf("\nEnter %d Elements in Row[%d]\n", num_cities, i + 1);
        for (j = 0; j < num_cities; j++)
        {
            scanf("%d", &matrix[i][j]);
        }
    }
    printf("\nEntered Cost Matrix\n");
    for (i = 0; i < num_cities; i++)
    {
        printf("\n");
        printf("City (%d) : ", i);
        for (j = 0; j < num_cities; j++)
        {
            printf("%d ", matrix[i][j]);
        }
    }

    TravelInfo *travelInfo = newTravelInfo(num_cities, matrix);
    
    int tour[num_cities];
    for (int i = 0; i < num_cities; i++) {
        tour[i] = i;
    }
    printf("\n\n");
    printf("All Possible Paths:\n");
    printf("\n");
    printf("************************************\n");
    printf("\n");

    gen_perms(travelInfo, tour, 0);
    printf("************************************\n");
    printf("Best Tour\n");
    printf("************************************\n");
    for (int i = 0; i < travelInfo->current_tour; i++) {
        printf("Weight: %d  -> Path: ", travelInfo->all_tours[i].weight);
        for(int j = 0; j < num_cities; j++){
            printf("%d ", travelInfo->all_tours[i].path[j]);
        }
        printf("\n");
    }   
    // for (size_t i = 0; i < num_cities; i++)
    // {
    //     printf("Weight: %d\n", travelInfo->best_tour_cost[i]);
    //     for (size_t j = 0; j < num_cities; j++){
    //         printf("%d ", travelInfo->best_tours[i][j]);
    //     }
    //     printf("\n");   
    // }
    // printf("\n%d\n", travelInfo->best_tour_cost);
    

    printf("\n");
    printf("************************************\n");
    printf("\n");
    
    return 0;
}
