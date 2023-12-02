#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>

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
    uint8_t *path;
    int weight;
} Tour;

Tour merge_tours(Tour tour1, Tour tour2) {
    if (tour1.weight < tour2.weight) {
        return tour1;
    } else {
        return tour2;
    }
}
#pragma omp declare reduction (merge : Tour : omp_out = merge_tours(omp_out, omp_in))

typedef struct travelInfo {
    uint8_t num_cities;
    int **matrix;
    // int best_tour_cost;
    int current_tour;
    double max_tours;
    Tour *best_tour;
} TravelInfo;

// Function to print formatted path and weight
void print_tour(Tour *tour, uint8_t num_cities) {
    printf("Path: ");
    for (uint8_t i = 0; i < num_cities; i++) {
        printf("%u ", tour->path[i]);
    }
    printf("0 -> Total Weight: %d\n", tour->weight);
}

void initMinTour(TravelInfo *ti) {
    int visited[ti->num_cities];
    int bestWeight;
    int totalWeight = 0;
    int curWeight;

    for (uint8_t i = 0; i < ti->num_cities; i++) {
        visited[i] = 0;
    }

    // make sure first element = 0
    ti->best_tour->path[0] = 0;
    // iterate over path, skipping over 0
    for (uint8_t i = 1; i < ti->num_cities; i++) {
        bestWeight = -1;
        // iterate over cities
        for (uint8_t j = 1; j < ti->num_cities; j++) {
            if (visited[j] || j == ti->best_tour->path[i-1])
                continue;

            curWeight = ti->matrix[ti->best_tour->path[i-1]][j];
            if (curWeight < bestWeight || bestWeight == -1) {
                bestWeight = curWeight;
                ti->best_tour->path[i] = j;
            }
        }
        visited[ti->best_tour->path[i]] = 1;

        totalWeight += bestWeight;
    }

    // add return trip
    totalWeight += ti->matrix[ti->best_tour->path[ti->num_cities - 1]][0];
    ti->best_tour->weight = totalWeight;
    // ti->best_tour_cost = totalWeight;
    print_tour(ti->best_tour, ti->num_cities);
}

TravelInfo *newTravelInfo(uint8_t num_cities, int matrix[num_cities][num_cities]) {
    TravelInfo *travelInfo = (TravelInfo *) malloc(sizeof(TravelInfo));
    
    travelInfo->num_cities = num_cities;
    
    travelInfo->max_tours = double_factorial(num_cities-1);

    // Allocate memory for the matrix
    travelInfo->matrix = (int **) malloc(sizeof(int *) * num_cities);
    
    travelInfo->best_tour = (Tour *) malloc(sizeof(Tour));
    travelInfo->best_tour->path = (uint8_t *) malloc(sizeof(uint8_t) * num_cities);
    travelInfo->best_tour->weight = -1;

    // Initialize the matrix, visited_cities, best_tour_cost, and best_tours arrays
    for (uint8_t i = 0; i < num_cities; i++) {    
        travelInfo->matrix[i] = (int *) malloc(sizeof(int) * travelInfo->num_cities);
        for (uint8_t j = 0; j < travelInfo->num_cities; j++) {
            if (j == travelInfo->num_cities) {
                travelInfo->matrix[i][j] = 0;
            }else{
                travelInfo->matrix[i][j] = matrix[i][j];
            }
        }
    }

    initMinTour(travelInfo);
    return travelInfo;
}

// Function to swap two elements in an array
void swap(uint8_t *a, uint8_t *b) {
    uint8_t temp = *a;
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


void update_best_tour(TravelInfo *ti, uint8_t path[], int weight) {
    if (ti->best_tour->weight == -1 || weight < ti->best_tour->weight) {
        // note: ti->best_tour_cost is not needed
        ti->best_tour->weight = weight;
        memcpy(ti->best_tour->path, path, ti->num_cities * sizeof(uint8_t));
        ti->best_tour->weight = weight;
        printf("  - New Best Tour\n");
        //print_tour(ti->best_tour, ti->num_cities);
    }
}

// Function to set the best tour cost
void set_best_tour_cost(TravelInfo *ti, int weight) {
    if(ti->best_tour->weight == -1 || weight < ti->best_tour->weight) {
        ti->best_tour->weight = weight;
    }
}

// Function to generate all possible paths, get weight of each path, and store the best path
Tour gen_perms(TravelInfo *ti, Tour best_tour, uint8_t path[], uint8_t currentIndex, int currentWeight) {
    uint8_t l_path[ti->num_cities];
    memcpy(l_path, path, ti->num_cities * sizeof(uint8_t));
    if (currentIndex == ti->num_cities - 1) {
        // Calculate the weight of the current path
        //update_best_tour(ti, path, currentWeight + ti->matrix[path[currentIndex - 1]][path[currentIndex]] + ti->matrix[path[currentIndex]][path[0]]);
        int totalWeight = currentWeight + ti->matrix[l_path[currentIndex - 1]][l_path[currentIndex]];
        totalWeight += ti->matrix[l_path[currentIndex]][0];
        if (best_tour.weight < totalWeight || best_tour.weight == -1) {
            best_tour.weight = totalWeight;
            memcpy(best_tour.path, l_path, ti->num_cities * sizeof(uint8_t));
            //printf("  - New Best Tour\n");
            //print_tour(&best_tour, ti->num_cities);
        }
        
        return best_tour;
    }

    // Note: can replace ti->best_tour_cost with ti->best_tour->weight
    if (currentWeight >= best_tour.weight && best_tour.weight != -1) {
        return best_tour;
    }

    #pragma omp parallel private(l_path, currentIndex, currentWeight) if(currentIndex == (ti->num_cities / 4))
    {
        #pragma omp for reduction(merge: best_tour) 
        for (uint8_t i = currentIndex; i < ti->num_cities; i++) {
            // Swap the current element with itself and all the subsequent elements
            swap(&l_path[currentIndex], &l_path[i]);
            // Recursively generate paths
            best_tour = gen_perms(ti, best_tour, l_path, currentIndex + 1, currentWeight + ti->matrix[l_path[currentIndex - 1]][l_path[currentIndex]]);
            // Backtrack
            swap(&l_path[currentIndex], &l_path[i]);
        }
    }

    return best_tour;
}   

int main()
{
    uint8_t i, j, num_cities;

    printf("Enter Total Number of Cities:\t");
    scanf("%" SCNu8, &num_cities);

    // Calculate the number of permutations (numCities!)
    //int numPermutations = factorial(num_cities);
    int matrix[num_cities][num_cities];
    //int tours[numPermutations][num_cities];

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
        
        printf("  City (%3u) : ", i);
        for (j = 0; j < num_cities; j++)
        {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("************************************\n");
    printf("Generating Paths / Weights\n");
    printf("************************************\n");
    
    TravelInfo *travelInfo = newTravelInfo(num_cities, matrix);
    
    uint8_t starting_tour[num_cities];
    for (uint8_t i = 0; i < num_cities; i++) {
        starting_tour[i] = i;
    }

    Tour best_tour;
    best_tour.path = malloc(num_cities * sizeof(uint8_t));
    memcpy(best_tour.path, travelInfo->best_tour->path, num_cities * sizeof(uint8_t));
    best_tour.weight = travelInfo->best_tour->weight;

    // travelInfo->best_tour = (Tour *) malloc(sizeof(Tour));
    // travelInfo->best_tour->path = (uint8_t *) malloc(sizeof(uint8_t) * num_cities);
    // travelInfo->best_tour->weight = -1;


    printf(" - Max Tours: %f\n", travelInfo->max_tours);
    printf("************************************\n");

    best_tour = gen_perms(travelInfo, best_tour, starting_tour, 1, 0);
    printf("\n");
    printf("************************************\n");
    printf("All Possible Paths\n");
    printf("************************************\n");
    printf("General Info\n");
    printf("  - Best Tour Cost: %d\n", best_tour.weight);
    printf("************************************\n");
    
    printf("\n");
    printf("************************************\n");
    printf("Best Tour Paths\n");
    printf("************************************\n");
    // Tour *best_tours = get_best_tours(travelInfo);

    print_tour(&best_tour, travelInfo->num_cities);
    printf("************************************\n");
        
    return 0;
}
