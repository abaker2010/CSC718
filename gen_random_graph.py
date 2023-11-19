import sys
import random

def main():

    num_cities = 50
    if len(sys.argv) > 1:
        num_cities = int(sys.argv[1])

    print(num_cities)

    for _ in range(num_cities):
        weights = ""
        for _ in range(num_cities):
            weights += f'{random.randint(1, 100)} '
        print(weights)

if __name__ == "__main__":
    main()
