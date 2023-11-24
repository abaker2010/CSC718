#include <stdio.h>
#include <stdlib.h>

int cost = 0;

typedef struct travelInfo {
    int *visited_cities;
    int num_cities;
    int best_tour_cost;
    int *best_tour;
    int **matrix;
} TravelInfo;

TravelInfo *newTravelInfo(int num_cities, int matrix[num_cities][num_cities], int visited_cities[num_cities]) {
    TravelInfo *travelInfo = (TravelInfo *) malloc(sizeof(TravelInfo));
    
    // Allocate memory for the matrix
    travelInfo->matrix = (int **) malloc(sizeof(int *) * num_cities);
    for (int i = 0; i < num_cities; i++) {
        travelInfo->matrix[i] = (int *) malloc(sizeof(int) * num_cities);
        for (int j = 0; j < num_cities; j++) {
            travelInfo->matrix[i][j] = matrix[i][j];
        }
    }
    
    // Allocate memory for the visited_cities array
    travelInfo->visited_cities = (int *) malloc(sizeof(int) * num_cities);
    for (int i = 0; i < num_cities; i++) {
        travelInfo->visited_cities[i] = visited_cities[i];
    }
    
    travelInfo->num_cities = num_cities;
    travelInfo->best_tour_cost = 0;
    travelInfo->best_tour = NULL;
    
    return travelInfo;
}

int check_path_cost(TravelInfo *ti, int path[])
{
    int i, j, cost = 0;
    for (i = 0; i < ti->num_cities; i++)
    {
        if (i+1 == ti->num_cities)
        {
            cost += ti->matrix[i][0];
        }
        else
        {
            cost += ti->matrix[i][i+1];
        }
    }
    return cost;
    
}
// Function to swap two elements in an array
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void gen_perms(TravelInfo *ti, int path[], int currentIndex) {
    if (currentIndex == ti->num_cities - 1) {
        // Calculate the weight of the current path
        int currentWeight = check_path_cost(ti, path);

        if (ti->best_tour == NULL || currentWeight < ti->best_tour_cost) {
            ti->best_tour_cost = currentWeight;
            free(ti->best_tour);
            ti->best_tour = malloc(sizeof(int) * (ti->num_cities));
            for (int i = 0; i < ti->num_cities; i++) {
                ti->best_tour[i] = path[i];
            }
        }
        // Print the current path and its weight
        printf("Path: ");
        for (int i = 0; i < ti->num_cities; i++) {
            printf("%d ", path[i]);
        }
        printf("-> Total Weight: %d\n", currentWeight);

        // return currentWeight;
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
    int visited_cities[num_cities];

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
        for (j = 0; j < num_cities; j++)
        {
            printf("%d ", matrix[i][j]);
        }
    }

    TravelInfo *travelInfo = newTravelInfo(num_cities, matrix, visited_cities);
    // int paths[numPermutations][num_cities];
    int tour[num_cities];
    for (int i = 0; i < num_cities; i++) {
        tour[i] = i;
    }

    for (int i = 0; i < num_cities; i++) {
        printf("%d ", tour[i]);
    }

    gen_perms(travelInfo, tour, 1);

    return 0;
}
