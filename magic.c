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

int compare_val(point , point);
int compare_sum(point , point);
void merge(point *, int , int , int, int (*f)(point, point));
void merge_sort(point *, int , int, int (*f)(point, point));
result calculate(point *, int);
result bruteForce(point *, int);
int magic(point, point);
result min_strip(point *, int , int);

int main() {
	char in[] = "magic.in";
	int n, i;
	FILE *f = fopen(in, "r");
	fscanf(f, "%d\n", &n);  // read number o numbers
	int *nums = (int *) malloc(n * sizeof(int));
	for (i = 0; i < n; i++) {  // for each number in input file, read it
		fscanf(f, "%d ", &nums[i]);
	}
	fclose(f);
	point *points = (point *) malloc(n * sizeof(point));
	int sum = 0;
	for (i = 0; i < n; i++) {
		points[i].index = i + 1;
		points[i].sum = sum;
		sum += nums[i];
	}
	// points is already sorted ascending by index
	result r = calculate(points, n);
	FILE *o = fopen("magic.out", "w");
	fprintf(o, "%d\n%d %d\n", r.magic, r.n1, r.n2);
	printf("%d\n%d %d\n", r.magic, r.n1, r.n2);
	fclose(o);
	free(nums);
	free(points);
	return 0;
}


int compare_val(point a, point b) {  // returns 1 if a.val > b.val. 0 otherwise
	if (a.index < b.index) {
		return 1;
	}
	return 0;
}

int compare_sum(point a, point b) {  // returns 1 if a.sum > b.sum. 0 otherwise
	if (a.sum < b.sum) {
		return 1;
	}
	return 0;
}

void merge(point *v, int i, int m, int j, int (*f)(point, point)) {
	//  merge two sorted arrays
	int iinit = i;
	point *u = (point *) calloc(j - i + 1, sizeof(point));
	int l = 0;
	int k = m + 1;
	while (i <= m && k <= j) {
		if ((*f)(v[i], v[j])) {
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

void merge_sort(point *v, int i, int j, int (*f)(point, point)) {
	//  split array into 2 and merge
	if (i < j) {
		int m = (i + j) / 2;
		merge_sort(v, i, m, *f);
		merge_sort(v, m + 1, j, *f);
		merge(v, i, m, j, *f);
	}
}

result calculate(point *points, int n) {
	if (n <= 3) {  // bruteforce if 2 or 3 points
		return bruteForce(points, n);
	}
	int m = n / 2, i, j;
	point mp = points[m];  // middle point
	result min_left = calculate(points, m);
	result min_right = calculate(&(points[m]), n - m);
	//  printf("ml %d %d %d\n", min_left->magic, min_left->n1, min_left->n2);
	//  printf("mr%d %d %d\n", min_right->magic, min_right->n1, min_right->n2);
	result r = min_left;
	if (r.magic > min_right.magic) {
		r = min_right;
	}
	point *strip = (point *) malloc(n * sizeof(point));
	for (i = 0, j = 0; i < n; i++) {
		if (abs(mp.index - points[i].index) < r.magic) {
			strip[j] = points[i];
			j++;
		}
	}
	result ms = min_strip(strip, j, r.magic);
	free(strip);
	// printf("r %d %d %d\n", r->magic, r->n1, r->n2);
	// printf("ms %d %d %d\n", ms->magic, ms->n1, ms->n2);
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
	int i, j, m;
	result r;
	r.magic = INF;
	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {
			m = magic(points[i], points[j]);
			//  printf("magic[%d, %d] = %d\n", i, j, m);
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
	merge_sort(strip, 0, n - 1, compare_sum);
	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n && (strip[j].index - strip[i].index)
			* (strip[j].index - strip[i].index) < min; j++) {
			m = magic(strip[i], strip[j]);
			if (m <= r.magic) {
				if (m == r.magic) {
					if (r.n1 < strip[i].index && r.n1 < strip[j].index) {
						//  keep the lowest lexicographical solution
						continue;
					}
				}
				r.magic = m;
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