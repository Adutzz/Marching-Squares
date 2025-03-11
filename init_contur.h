#include <stdio.h>
#include <stdlib.h>

//cele 4 functi de tip "colt_model()"
void x_model(int ***shape)
{
	(*shape)[0][1] = 255;
	(*shape)[1][0] = 255;
	(*shape)[0][0] = 180;
}

void y_model(int ***shape)
{
	(*shape)[0][2] = 255;
	(*shape)[1][3] = 255;
	(*shape)[0][3] = 180;
}

void z_model(int ***shape)
{
	(*shape)[2][3] = 255;
	(*shape)[3][2] = 255;
	(*shape)[3][3] = 180;
}

void t_model(int ***shape)
{
	(*shape)[2][0] = 255;
	(*shape)[3][1] = 255;
	(*shape)[3][0] = 180;
}

void cases(int x, int y, int z, int t, int near_x, int near_z, int ***shape)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			(*shape)[i][j] = 0;
			if (z == 1 && near_z == 1) {
				z_model(&(*shape));
				if (x == 1)
					x_model(&(*shape));
			} else if (near_x == 1 && x == 1) {
				x_model(&(*shape));
				if (z == 1)
					z_model(&(*shape));
			} else if (near_x == 1 && x == 0 && y == 1) {
				y_model(&(*shape));
				if (z == 1)
					z_model(&(*shape));
			} else if (near_x == 1 && x == 0 && t == 1) {
				t_model(&(*shape));
				if (z == 1)
					z_model(&(*shape));
			} else if (near_x == 2 && x == 0) {
				y_model(&(*shape));
				t_model(&(*shape));
				if (z == 1)
					z_model(&(*shape));
			} else if (near_x == 2 && y == 1) {
				x_model(&(*shape));
				y_model(&(*shape));
				if (z == 1)
					z_model(&(*shape));
			} else if (near_x == 2 && t == 1) {
				x_model(&(*shape));
				t_model(&(*shape));
				if (z == 1)
					z_model(&(*shape));
			} else if (near_x == 3) {
				x_model(&(*shape));
				y_model(&(*shape));
				t_model(&(*shape));
				if (z == 1)
					z_model(&(*shape));
			}
		}
	}
}

void merge(int ***shape)
{
	if ((*shape)[0][0] == 180 && (*shape)[0][3] == 180 &&
		(*shape)[3][0] == 0 && (*shape)[3][3] == 0) {
		(*shape)[0][1] = 180;
		(*shape)[0][2] = 180;
		(*shape)[1][1] = 255;
		(*shape)[1][2] = 255;
	} else if ((*shape)[0][3] == 180 && (*shape)[3][3] == 180 &&
		(*shape)[3][0] == 0 && (*shape)[0][0] == 0) {
		(*shape)[1][3] = 180;
		(*shape)[2][3] = 180;
		(*shape)[1][2] = 255;
		(*shape)[2][2] = 255;
	} else if ((*shape)[3][3] == 180 && (*shape)[3][0] == 180 &&
		(*shape)[0][0] == 0 && (*shape)[0][3] == 0) {
		(*shape)[3][1] = 180;
		(*shape)[3][2] = 180;
		(*shape)[2][1] = 255;
		(*shape)[2][2] = 255;
	} else if ((*shape)[3][0] == 180 && (*shape)[0][0] == 180 &&
		(*shape)[0][3] == 0 && (*shape)[3][3] == 0) {
		(*shape)[1][0] = 180;
		(*shape)[2][0] = 180;
		(*shape)[1][1] = 255;
		(*shape)[2][1] = 255;
	} else if ((*shape)[0][0] == 180 && (*shape)[0][3] == 180 &&
		(*shape)[3][3] == 180 && (*shape)[3][0] == 0) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				(*shape)[i][j] = 180;
		(*shape)[3][0] = 0;
		(*shape)[2][0] = 255;
		(*shape)[3][1] = 255;
	} else if ((*shape)[0][0] == 0 && (*shape)[0][3] == 180 &&
		(*shape)[3][3] == 180 && (*shape)[3][0] == 180) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				(*shape)[i][j] = 180;
		(*shape)[0][0] = 0;
		(*shape)[0][1] = 255;
		(*shape)[1][0] = 255;
	} else if ((*shape)[0][0] == 180 && (*shape)[0][3] == 0 &&
		(*shape)[3][3] == 180 && (*shape)[3][0] == 180) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				(*shape)[i][j] = 180;
		(*shape)[0][3] = 0;
		(*shape)[0][2] = 255;
		(*shape)[1][3] = 255;
	} else if ((*shape)[0][0] == 180 && (*shape)[0][3] == 180 &&
		(*shape)[3][3] == 0 && (*shape)[3][0] == 180) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				(*shape)[i][j] = 180;
		(*shape)[3][3] = 0;
		(*shape)[3][2] = 255;
		(*shape)[2][3] = 255;
	} else if ((*shape)[0][0] == 180 && (*shape)[0][3] == 180 &&
		(*shape)[3][3] == 180 && (*shape)[3][0] == 180) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				(*shape)[i][j] = 255;
	}
}

void binary(int ***shape, int new)
{
	//prin x,y,z,t ma refer la colturile patratului curent
	static int x, y, z, t;

	//resetez valorile daca apelez iar functia init_contur()
	if (new == 0) {
		x = 0;
		y = 0;
		z = 0;
		t = -1;
	}
	//ma ajut de numarul de vecini a 2 colturi opuse pt a construii matricea
	int near_x = 0, near_z = 0;
	t++;
	if (t > 1) {
		t = 0;
		z++;
	}
	if (z > 1) {
		z = 0;
		y++;
	}
	if (y > 1) {
		y = 0;
		x = 1;
	}
	if (x == 1)
		near_x++;
	if (y == 1) {
		near_x++;
		near_z++;
	}
	if (z == 1)
		near_z++;
	if (t == 1) {
		near_x++;
		near_z++;
	}
	//daca un colt este 1, ofer valorile corespunzatoare, ignorand vecinii
	cases(x, y, z, t, near_x, near_z, &(*shape));
	//daca exista colturi egale cu 1 vecine, le concatenez in forma corecta
	merge(&(*shape));

}

int init_contur(int find, int ***found)
{
	int **shape = (int **)malloc(4 * sizeof(int *));
	if (!shape) {
		printf("Eroare: alocare memorie eșuată.\n");
		return 1;
	}
	for (int i = 0; i < 4; i++) {
		shape[i] = (int *)malloc(4 * sizeof(int));
		if (!shape[i]) {
			printf("Eroare: alocare memorie eșuată la rândul %d.\n", i);
			return 1;
		}
	}
	//Voi folosi "new" pentru a face 0 variabilele statice din binary()
	int new = 0;
	for (int k = 0; k < 16; k++) {
		binary(&shape, new);
		new = 1;
		//prin "find" caut patternul de care eu am nevoie din init_contur()
		if (find == k) {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					(*found)[i][j] = shape[i][j];
			break;
		} else if (find == -1) {
			//daca "find" nu se schimba si ramande -1 la apelul functiei
			//init_contur(), dau print la toate sabloanele
			printf("P3\n4 4\n255\n");
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					(*found)[i][j] = 0;
					printf("%d %d ", shape[i][j], shape[i][j]);
					printf("%d\n", shape[i][j]);
				}
			}
		}
	}
	for (int i = 0; i < 4; i++)
		free(shape[i]);
	free(shape);

	return 0;
}
