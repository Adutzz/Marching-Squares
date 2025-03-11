#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functii.h"

int main(void)
{
	char ppm, command[20];
	int ppm_val, max_intensity, **matrix, succes, exited = 0, did_grid = 0;
	int read = 0, find = -1;
	int *n = (int *)malloc(sizeof(int));//height
	int *m = (int *)malloc(sizeof(int));//width
	//salvez matricea gasita la apelul march(), din init_contur(), in found
	int **found = (int **)malloc(4 * sizeof(int *));
	if (!found || !n || !m) {
		printf("Eroare: alocare memorie eșuată.\n");
		return 1;
	}
	for (int i = 0; i < 4; i++) {
		found[i] = (int *)malloc(4 * sizeof(int));
		if (!found[i]) {
			printf("Eroare: alocare memorie eșuată.\n");
			return 1;
		}
	}
	//furnizez inputul prin "command" pentru a stii ce functie apelez
	scanf("%s", command);
	while (strcmp(command, "EXIT") != 0) {
		if (strcmp(command, "READ") == 0) {
			//eliberez memoria anterioara doar daca nu este primul READ
			if (read == 1) {
				for (int i = 0; i < *n; i++)
					free(matrix[i]);
				free(matrix);
			}
			if (read == 1)
				did_grid = 0;
			read = 1; //am facut o citire
			scanf(" %c%d%d%d%d", &ppm, &ppm_val, n, m, &max_intensity);
			*m *= ppm_val;
			matrix = (int **)malloc(*n * sizeof(int *));
			if (!matrix) {
				printf("Eroare: alocare memorie eșuată.\n");
				return 1;
			}
			for (int i = 0; i < *n; i++) {
				matrix[i] = (int *)malloc(*m * sizeof(int));
				if (!matrix[i]) {
					printf("Eroare: alocare memorie eșuată.\n");
					return 1;
				}
			}
			succes = errors(n, m, matrix, ppm, ppm_val, max_intensity);
			if (succes == 0) {
				strcpy(command, "EXIT");
				exited = 1;
			}
		}
		if (strcmp(command, "WRITE") == 0 && succes == 1)
			write(n, m, matrix, did_grid);
		if (strcmp(command, "INIT_CONTUR") == 0)
			if (init_contur(find, &found) == 1)
				return 1;
		if (strcmp(command, "GRID") == 0) {
			if (grid(n, m, &matrix, 0) == 0)
				did_grid = 1;
			else
				return 1;
		}
		if (strcmp(command, "MARCH") == 0) {
			if (march(n, m, &matrix, &found, did_grid) == 0)
				did_grid = 1;
			else
				return 1;
		} else if (strcmp(command, "RESIZE") == 0)
			if (resize(n, m, &matrix) == 1)
				return 1;
		if (exited == 1)
			break;
		scanf("%s", command);
	}
	if (exited == 0)
		printf("Gigel a terminat\n");

	free_memory(n, m, matrix, found, read);
	return 0;
}
