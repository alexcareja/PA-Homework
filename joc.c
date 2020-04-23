#include <stdio.h>
#include <stdlib.h>

int solve(int *, int, int, int);

int main() {
	int solution = 0, i, n, k, l;
	FILE *f = fopen("joc.in", "r");
	fscanf(f, "%d %d %d\n", &n, &k, &l);
	int *v = (int *) malloc(n * sizeof(int));
	int even = 0, odd = 0;
	for (i = 0; i < n; i++) {
		fscanf(f, "%d ", &v[i]);
		if (v[i] % 2) {
			odd++;
		} else {
			even++;
		}
	}
	fclose(f);
	if (odd == 0 || n < l + k - 1 || k > l
		|| (even == 0 && l % 2 == 0)) {
		solution = -1;
	}
	if (solution != -1) {
		solution = solve(v, n, k, l);
	}
	FILE *o = fopen("joc.out", "w");
	fprintf(o, "%d\n", solution);
	fclose(o);
	free(v);
	return 0;
}

int solve(int *v, int n, int k, int l) {
	return 0;
}
