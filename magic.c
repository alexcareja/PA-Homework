#include <stdio.h>
#include <stdlib.h>

#define INF 1347483648

typedef struct point {
	int index;
	int sum;
} point;

typedef struct result {
	int magic;
	int n1;
	int n2;
} result;

void merge(point *, int , int , int);
void merge_sort(point *, int , int);
result calculate(point *, int);
result bruteForce(point *, int);
int magic(point, point);
result min_strip(point *, int , int);

int main() {
	char in[] = "magic.in";
	int n, i;
	FILE *f = fopen(in, "r");
	fscanf(f, "%d\n", &n);  // read number of numbers
	int *nums = (int *) malloc(n * sizeof(int));
	for (i = 0; i < n; i++) {  // for each number in input file, read it
		fscanf(f, "%d ", &nums[i]);
	}
	fclose(f);
	point *points = (point *) malloc(n * sizeof(point));
	int sum = 0;
	// calculate sum for each point and initialise index
	for (i = 0; i < n; i++) {
		points[i].index = i + 1;
		points[i].sum = sum;
		sum += nums[i];
	}
	// points vector is already sorted ascending by index
	result r = calculate(points, n);
	FILE *o = fopen("magic.out", "w");
	fprintf(o, "%d\n%d %d\n", r.magic, r.n1, r.n2);
	fclose(o);
	free(nums);
	free(points);
	return 0;
}

void merge(point *v, int i, int m, int j) {
	//  merge two sorted arrays
	int iinit = i;
	point *u = (point *) calloc(j - i + 1, sizeof(point));
	int l = 0;
	int k = m + 1;
	while (i <= m && k <= j) {
		if (v[i].sum < v[j].sum) {
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

void merge_sort(point *v, int i, int j) {
	//  split array into 2 and merge
	if (i < j) {
		int m = (i + j) / 2;
		merge_sort(v, i, m);
		merge_sort(v, m + 1, j);
		merge(v, i, m, j);
	}
}

result calculate(point *points, int n) {
	if (n <= 3) {  // bruteforce if 2 or 3 points
		return bruteForce(points, n);
	}
	int m = n / 2, i, j;
	// Divide into two and solve
	point mp = points[m];  // middle point
	result min_left = calculate(points, m);  // left side
	result min_right = calculate(&(points[m]), n - m);  // right side
	// keep the smaller solution
	result r = min_right;
	if (r.magic == min_left.magic) {
		if (r.n1 > min_left.n1) {
			r = min_left;
		}
	} else {
		if (r.magic > min_left.magic) {
			r = min_left;
		}
	}
	// put all the points close enough to the centre in a vector
	point *strip = (point *) malloc(n * sizeof(point));
	for (i = 0, j = 0; i < n; i++) {
		if (abs(mp.index - points[i].index) < r.magic) {
			strip[j] = points[i];
			j++;
		}
	}
	// find out the min magic number fron the strip
	result ms = min_strip(strip, j, r.magic);
	free(strip);
	// keep the smaller solution
	if (ms.magic <= r.magic) {
		if (ms.magic == r.magic) {
			if (r.n1 > ms.n1) {
				r = ms;
			}
		} else {
			r = ms;
		}
	}
	return r;
}

result bruteForce(point *points, int n) {
	// only called if n < 3. Calculates at max 1-2 1-3 2-3
	int i, j, m;
	result r;
	r.magic = INF;
	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {
			m = magic(points[i], points[j]);
			if (m < r.magic) {
				r.magic = m;
				r.n1 = points[i].index;
				r.n2 = points[j].index;
			}
		}
	}
	return r;
}

int magic(point a, point b) {  //  calculate the magic number for points a b
	int s = a.sum - b.sum;
	int v = a.index - b.index;
	return s * s + v * v;
}

result min_strip(point *strip, int n, int min) {
	int i, j, m;
	result r;
	r.magic = INF;
	r.n1 = INF;
	merge_sort(strip, 0, n - 1);  // sort by sum ascending
	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n && j < i + 7; j++) {   // at max 6 iterations
			m = magic(strip[i], strip[j]);
			if (m <= r.magic) {
				if (m == r.magic) {
					if (r.n1 < strip[i].index && r.n1 < strip[j].index) {
						//  keep the lowest lexicographical solution
						continue;
					}
				}
				r.magic = m;
				// n1 must be smaller than n2
				if (strip[i].index < strip[j].index) {
					r.n1 = strip[i].index;
					r.n2 = strip[j].index;
				} else {
					r.n1 = strip[j].index;
					r.n2 = strip[i].index;
				}
			}
		}
	}
	return r;
}

