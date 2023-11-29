#include <stdio.h>
#include <stdlib.h>

unsigned int factorial(unsigned int n)
{
    if (n == 1) {
      return 1;
    }
    return n * factorial(n - 1);
}
double double_factorial(unsigned int n)
{
    if (n == 1) {
      return 1.0;
    }
    return (double) n * double_factorial(n - 1);
}

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
    double max_tours;
    Tour *best_tour;
} TravelInfo;

TravelInfo *newTravelInfo(int num_cities, int matrix[num_cities][num_cities]) {
    TravelInfo *travelInfo = (TravelInfo *) malloc(sizeof(TravelInfo));
    
    travelInfo->num_cities = num_cities;
    
    travelInfo->max_tours = double_factorial(num_cities-1);
    travelInfo->best_tour_cost = -1;

    // Allocate memory for the matrix
    travelInfo->matrix = (int **) malloc(sizeof(int *) * travelInfo->num_cities);
    
    travelInfo->best_tour = (Tour *) malloc(sizeof(Tour));

    // Initialize the matrix, visited_cities, best_tour_cost, and best_tours arrays
    for (int i = 0; i < num_cities; i++) {    
        travelInfo->matrix[i] = (int *) malloc(sizeof(int) * travelInfo->num_cities);
        for (int j = 0; j < travelInfo->num_cities + 1; j++) {
            if (j == travelInfo->num_cities + 1) {
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
// Function to print formatted path and weight
void print_path_with_weight(int path[], int weight, int num_cities) {
    printf("Path: ");
    for (int i = 0; i < num_cities; i++) {
        printf("%d ", path[i]);
    }
    printf("-> Total Weight: %d\n", weight);
}

// Function to print the best tours
void print_best_tours(Tour *best_tours, int num_cities) {
    print_path_with_weight(best_tours[0].path, best_tours[0].weight, num_cities);
}

// Function to calculate the total weight of a path
int check_path_cost(TravelInfo *ti, int path[])
{
    int weight = 0;
    for (int i = 0; i < ti->num_cities - 1; i++) {
        weight += ti->matrix[path[i]][path[i + 1]];
    }
    weight += ti->matrix[path[ti->num_cities - 1]][path[0]]; // Return to the starting city
    return weight;
}

void update_best_tour(TravelInfo *ti, int path[], int weight) {
    if (ti->best_tour_cost == -1 || weight < ti->best_tour_cost) {
        ti->best_tour_cost = weight;
        free(ti->best_tour);
        ti->best_tour = newTour(path, weight);
        printf("  - New Best Tour\n");
        print_best_tours(ti->best_tour, ti->num_cities);
    }
}

// Function to set the best tour cost
void set_best_tour_cost(TravelInfo *ti, int weight) {
    if(ti->best_tour_cost == -1 || weight < ti->best_tour_cost) {
        ti->best_tour_cost = weight;
    }
}

// Function to generate all possible paths, get weight of each path, and store the best path
void gen_perms(TravelInfo *ti, int path[], int currentIndex, int currentWeight) {
    if (currentIndex == ti->num_cities - 1) {
        // Calculate the weight of the current path
        update_best_tour(ti, path, currentWeight + ti->matrix[path[currentIndex - 1]][path[currentIndex]] + ti->matrix[path[currentIndex]][path[0]]);
        
        return;
    }

    if (currentWeight > ti->best_tour_cost && ti->best_tour_cost != -1) {
        return;
    }

    for (int i = currentIndex; i < ti->num_cities; i++) {
        // Swap the current element with itself and all the subsequent elements
        swap(&path[currentIndex], &path[i]);
        // Recursively generate paths
        gen_perms(ti, path, currentIndex + 1, currentWeight + ti->matrix[path[currentIndex - 1]][path[currentIndex]]);
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
    int numPermutations = factorial(num_cities);
    int matrix[num_cities][num_cities];
    int tours[numPermutations][num_cities];

    printf("\nEnter Cost Matrix\n");
    for (i = 0; i < num_cities; i++)
    {
        printf("\nEnter %d Elements in Row[%d]\n", num_cities, i + 1);
        for (j = 0; j < num_cities; j++)
        {
            scanf("%d", &matrix[i][j]);
        }
    }
    printf("\n");
    printf("************************************\n");
    printf("City Cost Matrix\n");
    printf("************************************\n");
    for (i = 0; i < num_cities; i++)
    {
        
        printf("  City (%d) : ", i);
        for (j = 0; j < num_cities; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("************************************\n");
    printf("Generating Paths / Weights\n");
    printf("************************************\n");
    
    TravelInfo *travelInfo = newTravelInfo(num_cities, matrix);
    
    int starting_tour[num_cities];
    for (int i = 0; i < num_cities; i++) {
        starting_tour[i] = i;
    }

    printf(" - Max Tours: %f\n", travelInfo->max_tours);
    printf("************************************\n");

    gen_perms(travelInfo, starting_tour, 1, 0);
    printf("\n");
    printf("************************************\n");
    printf("All Possible Paths\n");
    printf("************************************\n");
    printf("General Info\n");
    printf("  - Best Tour Cost: %d\n", travelInfo->best_tour_cost);
    printf("************************************\n");
    
    printf("\n");
    printf("************************************\n");
    printf("Best Tour Paths\n");
    printf("************************************\n");
    // Tour *best_tours = get_best_tours(travelInfo);
    print_best_tours(travelInfo->best_tour, travelInfo->num_cities);
    printf("************************************\n");
        
    return 0;
}
