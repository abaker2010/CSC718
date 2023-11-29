#include <stdio.h>
#include <stdlib.h>

unsigned int factorial(unsigned int n)
{
    if (n == 1) {
      return 1;
    }
    return n * factorial(n - 1);
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
    int total_tours;
    Tour *all_tours;
} TravelInfo;

TravelInfo *newTravelInfo(int num_cities, int matrix[num_cities][num_cities]) {
    TravelInfo *travelInfo = (TravelInfo *) malloc(sizeof(TravelInfo));
    
    travelInfo->num_cities = num_cities;
    travelInfo->total_tours = factorial(num_cities-1);
    travelInfo->best_tour_cost = -1;
    travelInfo->current_tour = 0;

    // Allocate memory for the matrix
    travelInfo->matrix = (int **) malloc(sizeof(int *) * travelInfo->num_cities);
    // Allocate memory for the all_tours array
    travelInfo->all_tours = (Tour *) malloc(sizeof(Tour) * travelInfo->total_tours);

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
// Function to save the path and weight for useage later
void save_path_and_weight(TravelInfo *ti, int path[], int weight) {
    int *newPath = (int *) malloc(sizeof(int) * ti->num_cities);
    for (int i = 0; i < ti->num_cities; i++) {
        newPath[i] = path[i];
    }
    ti->all_tours[ti->current_tour] = *newTour(newPath, weight);
    ti->current_tour++;
}
// Function to set the best tour cost
void set_best_tour_cost(TravelInfo *ti, int weight) {
    if(ti->best_tour_cost == -1 || weight < ti->best_tour_cost) {
        ti->best_tour_cost = weight;
    }
}
// Function to print formatted path and weight
void print_path_with_weight(int path[], int weight, int num_cities) {
    printf("Path: ");
    for (int i = 0; i < num_cities; i++) {
        printf("%d ", path[i]);
    }
    printf("-> Total Weight: %d\n", weight);
}
// Function to print the path and weight of each tour from the travel info
void print_ti_path_weight(TravelInfo *ti) {
    for (int i = 0; i < ti->current_tour; i++) {
        print_path_with_weight(ti->all_tours[i].path, ti->all_tours[i].weight, ti->num_cities);
    }
}
// Function to get the best tours from the travel info
Tour* get_best_tours(TravelInfo *ti) {
    Tour *best_tours = (Tour *) malloc(sizeof(Tour) * ti->num_cities);
    int best_tour_count = 0;
    for (int i = 0; i < ti->current_tour; i++) {
        if (ti->all_tours[i].weight == ti->best_tour_cost) {
            best_tours[best_tour_count] = ti->all_tours[i];
            best_tour_count++;
        }
    }
    return best_tours;
}
// Function to print the best tours
void print_best_tours(Tour *best_tours, int num_cities) {
    print_path_with_weight(best_tours[0].path, best_tours[0].weight, num_cities);
}

// Function to generate all possible paths, get weight of each path, and store the best path
void gen_perms(TravelInfo *ti, int path[], int currentIndex) {
    if (currentIndex == ti->num_cities - 1) {
        // Calculate the weight of the current path
        int currentWeight = check_path_cost(ti, path);
        // Save the path and weight for useage later
        save_path_and_weight(ti, path, currentWeight);
        set_best_tour_cost(ti, currentWeight);
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

    gen_perms(travelInfo, starting_tour, 1);
    printf("\n");
    printf("************************************\n");
    printf("All Possible Paths\n");
    printf("************************************\n");
    printf("General Info\n");
    printf("  - Tours Tried: %d\n", travelInfo->current_tour);
    printf("  - Total Tours Expected: %d\n", travelInfo->total_tours);
    printf("  - Best Tour Cost: %d\n", travelInfo->best_tour_cost);
    printf("************************************\n");

    print_ti_path_weight(travelInfo);

    
    printf("\n");
    printf("************************************\n");
    printf("Best Tour Paths\n");
    printf("************************************\n");
    Tour *best_tours = get_best_tours(travelInfo);
    print_best_tours(best_tours, travelInfo->num_cities);
    printf("************************************\n");
        
    return 0;
}
