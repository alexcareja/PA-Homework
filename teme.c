#include <stdio.h>
#include <stdlib.h>

typedef struct homework {
	int index;
	int duration;
	int points;
	int deadline;
} homework;

typedef struct stack {
	int value;
	struct stack *next;
} stack;

typedef struct cell {
	int value;
	int previous_cell;
	int current;
} cell;

cell **solve(homework *, int, int);
void merge(homework *, int , int , int);
void merge_sort(homework *, int , int);

int main() {
	char in[] = "teme.in";
	int n, i, j;
	FILE *f = fopen(in, "r");
	fscanf(f, "%d\n", &n);
	homework *h = (homework *) malloc(n * sizeof(homework));
	for (i = 0; i < n; i++) {  // for each line in input file, read line
		fscanf(f, "%d %d %d\n", &h[i].duration, &h[i].points, &h[i].deadline);
		h[i].index = i + 1;
	}
	fclose(f);
	merge_sort(h, 0, n - 1);  // sort ascending by deadline
	int max_dl = h[n - 1].deadline;

	cell **matrix = solve(h, n, max_dl);

	// initialise stack
	stack *s = (stack *) malloc(sizeof(stack));
	stack *aux;
	int prev_cell, count = 0;
	s->value = -1;
	s->next = NULL;
	i = n;
	j = max_dl;
	// reverse the homeworks using a stack
	while (i > 0) {
		if (matrix[i][j].current) {
			count ++;
			if (s->value == -1) {  // stack is empty
				s->value = i - 1;
			} else {
				aux = (stack *) malloc(sizeof(stack));
				aux->value = i - 1;
				aux->next = s;
				s = aux;
			}
		}
		prev_cell = matrix[i][j].previous_cell;
		i = prev_cell / (max_dl + 1);
		j = prev_cell % (max_dl + 1);
	}
	FILE *o = (fopen("teme.out", "w"));
	fprintf(o, "%d %d\n", matrix[n][max_dl].value, count);
	while (s != NULL) {
		fprintf(o, "%d ", h[s->value].index);
		aux = s;
		s = s->next;
		free(aux);  // free allocated memory for stack
	}
	fprintf(o, "\n");
	fclose(o);
	for (i = 0; i <= n; i++) {
		free(matrix[i]);  // free one allocated line of the matrix
	}
	free(matrix);  // free matrix vector
	free(h);  // free allocated memory for homeworks vector
	return 0;
}

cell **solve(homework *h, int n, int max_dl) {
	int i, j;
	cell **matrix = (cell **) malloc((n + 1) * sizeof(cell *));
	for (i = 0; i <= n; i++) {
		matrix[i] = (cell *) malloc((max_dl + 1) * sizeof(cell));
	}
	for (i = 0; i <= max_dl; i++) {  // initialise first line
		matrix[0][i].value = 0;
		matrix[0][i].current = 0;
		matrix[0][i].previous_cell = 0;
	}
	int new_points;
	/* matrix[i][j].value represents the maximum number of points the student
	can obtain using the first i homeworks in the time of j weeks */
	for (i = 1; i <= n; i++) {  // for each homework
		// initialise first cell from line
		matrix[i][0].value = 0;
		matrix[i][0].current = 0;
		matrix[i][0].previous_cell = 0;
		for (j = 1; j <= max_dl; j++) {  // for each week
			if (h[i - 1].duration > j) {  // check if it can be done
				matrix[i][j].value = matrix[i - 1][j].value;
				matrix[i][j].previous_cell = (i - 1) * (max_dl + 1) + j;
				matrix[i][j].current = 0;
				continue;
			}
			if (j > h[i - 1].deadline) {  // check if the hw can be done
				/* if it can't be done, then the progress must be maximum between
				left and up cells*/
				while (j <= max_dl) {
					if (matrix[i][j - 1].value > matrix[i - 1][j].value) {
						matrix[i][j].value = matrix[i][j - 1].value;
						matrix[i][j].current = matrix[i][j - 1].current;
						matrix[i][j].previous_cell = matrix[i][j - 1].previous_cell;
					} else {
						matrix[i][j].value = matrix[i - 1][j].value;
						matrix[i][j].current = 0;
						matrix[i][j].previous_cell = (i - 1) * (max_dl + 1) + j;
					}
					j++;
				}
				break;
			}
			// if it can be done, check if the student can obtain more points by doing it
			new_points = matrix[i - 1][j - h[i - 1].duration].value + h[i - 1].points;
			if (new_points > matrix[i-1][j].value) {
				matrix[i][j].value = new_points;
				matrix[i][j].previous_cell = (i - 1) * (max_dl + 1) + j - h[i - 1].duration;
				matrix[i][j].current = 1;
			} else {
				matrix[i][j].value = matrix[i-1][j].value;
				matrix[i][j].previous_cell = (i - 1) * (max_dl + 1) + j;
				matrix[i][j].current = 0;
			}
		}
	}
	return matrix;
}

void merge(homework *v, int i, int m, int j) {  // merge two sorted arrays
	int iinit = i;
	homework *u = (homework *) calloc(j - i + 1, sizeof(homework));
	int l = 0;
	int k = m + 1;
	while (i <= m && k <= j) {
		if (v[i].deadline < v[k].deadline) {
			u[l++] = v[i++];
		} else {
			u[l++] = v[k++];
		}
	}
	while (i <= m) {
		u[l++] = v[i++];
	}
	while (k <= j) {
		u[l++] = v[k++];
	}
	l = 0;
	while (iinit <= j) {
		v[iinit++] = u[l++];
	}
	free(u);
}

void merge_sort(homework *v, int i, int j) {  // split array into 2 and merge
	if (i < j) {
		int m = (i + j) / 2;
		merge_sort(v, i, m);
		merge_sort(v, m + 1, j);
		merge(v, i, m, j);
	}
}
