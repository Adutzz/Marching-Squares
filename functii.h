#include <stdio.h>
#include <stdlib.h>
#include "init_contur.h"

int errors(int *n, int *m, int **matrix, char ppm, int ppm_val, int max_i)
{
	if (ppm != 'P' || ppm_val != 3) {
		printf("Eroare: trebuie sa fie P3\n");
		return 0;
	}
	for (int i = 0; i < *n; i++) {
		for (int j = 0; j < *m; j++) {
			if (scanf("%d", &matrix[i][j]) != 1) {
				printf("Eroare: eroare citire pixeli\n");
				return 0;
			}
			if (matrix[i][j] > max_i || matrix[i][j] < 0) {
				printf("Eroare: valoare pixel necorespunzatoare\n");
				return 0;
			}
		}
	}
	printf("Imagine citita cu succes [%d-%d]\n", *n, *m / 3);
	return 1;
}

void write(int *n, int *m, int **matrix, int did_grid)
{
	if (did_grid == 0) {
		printf("P3\n%d %d\n%d\n", *n, *m / 3, 255);
		for (int i = 0; i < *n; i++) {
			for (int j = 0; j < *m; j += 3) {
				printf("%d %d ", matrix[i][j], matrix[i][j + 1]);
				printf("%d\n", matrix[i][j + 2]);
			}
		}
	} else {
		printf("P3\n%d %d\n%d\n", *n, *m, 255);
		for (int i = 0; i < *n; i++) {
			for (int j = 0; j < *m; j++) {
				printf("%d %d %d\n", matrix[i][j], matrix[i][j], matrix[i][j]);
			}
		}
	}
}

void directions(int *n, int *m, int **matrix, int **tmp)
{
	int i = 0, j = 0, up_left = 0, down_left = 0, up_right = 0, down_right = 0;
	int new_line = 1, finish = 0, sigma = 200, self = 0;
	while (finish < *m / 4 + 1) {
		j += 4;
		if (new_line == 1) {
			j -= 4;
			new_line = 0;
		}
		int nr_dir = 0;
		up_left = 1;
		down_left = 1;
		up_right = 1;
		down_right = 1;
		self = 1;
		if (j == 0) {
			up_left = 0;
			down_left = 0;
		}
		if (j >= *m) {
			up_right = 0;
			down_right = 0;
			self = 0;
		}
		if (i == 0) {
			up_left = 0;
			up_right = 0;
		}
		if (i >= *n) {
			down_left = 0;
			down_right = 0;
			self = 0;
			finish++;
		}
		if (up_left == 1) {
			nr_dir++;
			tmp[i / 4][j / 4] += matrix[i - 1][j - 1];
		}
		if (up_right == 1) {
			nr_dir++;
			tmp[i / 4][j / 4] += matrix[i - 1][j + 1];
		}
		if (down_left == 1) {
			nr_dir++;
			tmp[i / 4][j / 4] += matrix[i + 1][j - 1];
		}
		if (down_right == 1) {
			nr_dir++;
			tmp[i / 4][j / 4] += matrix[i + 1][j + 1];
		}
		if (self == 1) {
			nr_dir++;
			tmp[i / 4][j / 4] += matrix[i][j];
		}
		tmp[i / 4][j / 4] /= nr_dir;
		if (tmp[i / 4][j / 4] <= sigma)
			tmp[i / 4][j / 4] = 1;
		else
			tmp[i / 4][j / 4] = 0;
		if (j >= *m) {
			i += 4;
			j = 0;
			new_line = 1;
		}
	}
}

int grid(int *n, int *m, int ***matrix, int march_call)
{
	int light_pixel = 0, **tmp;
	tmp = (int **)malloc(*n * sizeof(int *));
	if (!tmp) {
		printf("Eroare: alocare memorie eșuată.\n");
		return 1;
	}
	for (int i = 0; i < *n; i++) {
		tmp[i] = (int *)malloc(*m / 3 * sizeof(int));
		if (!tmp[i]) {
			printf("Eroare: alocare memorie eșuată.\n");
			return 1;
		}
	}
	int l = 0;
	//luminozitatea
	for (int i = 0; i < *n; i++) {
		for (int j = 0; j < *m; j += 3) {
			light_pixel += (*matrix)[i][j] + (*matrix)[i][j + 1];
			light_pixel = (light_pixel +  + (*matrix)[i][j + 2]) / 3;
			tmp[i][l] = light_pixel;
			light_pixel = 0;
			l++;
		}
		l = 0;
	}
	*m /= 3;
	//inlocuiesc cu valorile noi
	for (int i = 0; i < *n; i++) {
		free((*matrix)[i]);
		(*matrix)[i] = (int *)malloc(*m * sizeof(int));
		if (!(*matrix)[i]) {
			printf("Eroare: alocare memorie eșuată.\n");
			return 1;
		}
		for (int j = 0; j < *m; j++)
			(*matrix)[i][j] = tmp[i][j];
		free(tmp[i]);
	}
	free(tmp);

	tmp = (int **)calloc((*n / 4 + 1), sizeof(int *));
	for (int i = 0; i < *n / 4 + 1; i++)
		tmp[i] = (int *)calloc((*m / 4 + 1), sizeof(int));

	directions(n, m, *matrix, tmp);

	if (march_call == 0) {
		printf("Grid calculat cu succes [%d-%d]\n", *n / 4, *m / 4);
		for (int i = 0; i < *n / 4; i++) {
			for (int j = 0; j < *m / 4; j++) {
				printf("%d ", tmp[i][j]);
			}
			printf("\n");
		}
	}

	for (int i = 0; i < *n; i++)
		free((*matrix)[i]);
	free((*matrix));
	*n = *n / 4 + 1;
	*m = *m / 4 + 1;
	(*matrix) = (int **)malloc(*n * sizeof(int *));
	if (!(*matrix)) {
		printf("Eroare: alocare memorie eșuată.\n");
		return 1;
	}
	for (int i = 0; i < *n; i++) {
		(*matrix)[i] = (int *)malloc(*m * sizeof(int));
		if (!(*matrix)[i]) {
			printf("Eroare: alocare memorie eșuată.\n");
			return 1;
		}
		for (int j = 0; j < *m; j++)
			(*matrix)[i][j] = tmp[i][j];
		free(tmp[i]);
	}
	free(tmp);

	return 0;
}

int march(int *n, int *m, int ***matrix, int ***found, int did_grid)
{
	if (did_grid == 0)
		if (grid(n, m, &(*matrix), 1) == 1)
			return 1;

	printf("Marching Squares aplicat cu succes ");
	printf("[%d-%d]\n", *n * 4 - 4, *m * 4 - 4);

	//salvez valorile in temp dupa aplicarea algoritmului marching squares
	int **temp = (int **)malloc((*n * 4) * sizeof(int *));
	if (!temp) {
		printf("Eroare: alocare memorie eșuată.\n");
		return 1;
	}
	for (int i = 0; i < *n * 4; i++) {
		temp[i] = (int *)malloc((*m * 4) * sizeof(int));
		if (!temp[i]) {
			printf("Eroare: alocare memorie eșuată.\n");
			return 1;
		}
	}
	//iau patratelele din dreapta sus, astfel merg pana la n-1 linii si
	//incep de la coloana 1
	for (int i = 0; i < *n - 1; i++) {
		for (int j = 1; j < *m; j++) {
			//patternul pe care il caut il salvez in k
			int find = (*matrix)[i + 1][j - 1] + (2 * (*matrix)[i + 1][j]) +
			(4 * (*matrix)[i][j]) + (8 * (*matrix)[i][j - 1]);
			//gasesc matricea din init_contur
			init_contur(find, &(*found));

			//parcurc matricea "found" prin intermediul variabilelor e si f
			int e = 0, f = 0;
			for (int l = i * 4; l < i * 4 + 4; l++) {
				for (int o = (j - 1) * 4; o < (j - 1) * 4 + 4; o++) {
					temp[l][o] = (*found)[e][f];
					f++;
				}
				e++;
				f = 0;
			}
		}
	}

	for (int i = 0; i < *n; i++)
		free((*matrix)[i]);
	free((*matrix));
	*n = *n * 4;
	*m = *m * 4;
	(*matrix) = (int **)malloc((*n - 4) * sizeof(int *));
	if (!(*matrix)) {
		printf("Eroare: alocare memorie eșuată.\n");
		return 1;
	}
	//inlocuiesc valorile din matrix cu cele noi din temp
	for (int i = 0; i < *n - 4; i++) {
		(*matrix)[i] = (int *)malloc((*m - 4) * sizeof(int));
		if (!(*matrix)[i]) {
			printf("Eroare: alocare memorie eșuată.\n");
			return 1;
		}
		for (int j = 0; j < *m - 4; j++)
			(*matrix)[i][j] = temp[i][j];
	}
	for (int i = 0; i < *n; i++)
		free(temp[i]);
	free(temp);
	*n -= 4;
	*m -= 4;

	return 0;
}

int resize(int *n, int *m, int ***matrix)
{
	//prin tmp (temporary) salvez matricea marita si apoi o inlocuiesc
	//cu cea oriliginala (matrix)
	int **tmp = (int **)malloc(*n * 4 * sizeof(int *));
	if (!tmp) {
		printf("Eroare: alocare memorie eșuată.\n");
		return 1;
	}
	for (int i = 0; i < *n * 4; i++) {
		tmp[i] = (int *)malloc(*m * 4 * sizeof(int));
		if (!tmp[i]) {
			printf("Eroare: alocare memorie eșuată.\n");
			return 1;
		}
	}
	for (int i = 0; i < *n; i++) {
		for (int j = 0; j < *m; j += 3) {
			//pentru deplasarea in tmp
			for (int l = i * 4; l < i * 4 + 4; l++) {
				for (int k = j * 4; k < j * 4 + 12; k += 3) {
					tmp[l][k] = (*matrix)[i][j];
					tmp[l][k + 1] = (*matrix)[i][j + 1];
					tmp[l][k + 2] = (*matrix)[i][j + 2];
				}
			}
		}
	}
	for (int i = 0; i < *n; i++)
		free((*matrix)[i]);
	free((*matrix));
	*n *= 4;
	*m *= 4;
	(*matrix) = (int **)malloc(*n * sizeof(int *));
	if (!(*matrix)) {
		printf("Eroare: alocare memorie eșuată.\n");
		return 1;
	}
	//realoc in matricea originala
	for (int i = 0; i < *n; i++) {
		(*matrix)[i] = (int *)malloc(*m * sizeof(int));
		if (!(*matrix)[i]) {
			printf("Eroare: alocare memorie eșuată.\n");
			return 1;
		}
		for (int j = 0; j < *m; j++)
			(*matrix)[i][j] = tmp[i][j];
		free(tmp[i]);
	}
	free(tmp);
	printf("Imagine redimensionata cu succes [%d-%d]\n", *n, *m / 3);

	return 0;
}

void free_memory(int *n, int *m, int **matrix, int **found, int read)
{
	if (read != 0) {
		for (int i = 0; i < *n; i++) {
			free(matrix[i]);
		}
		free(matrix);
	}
	free(n);
	free(m);
	for (int i = 0; i < 4; i++)
		free(found[i]);
	free(found);
}
