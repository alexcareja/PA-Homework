#include <stdio.h>
#include <stdlib.h>

#define INF 1347483648

typedef struct duck {
	int id;  // id of the duck
	int pos;
	int dir;  // direction (-1 = left, 1 = right)
} duck;

typedef struct solution {
	int id;
	int time;
} solution;

typedef struct ListNode {
	duck *elem;
	struct ListNode *next;
	struct ListNode *prev;
}ListNode;  // node data type

typedef struct SortedList {
	ListNode *front;
	ListNode *rear;
	int size;
}SortedList;  // sorted list data type

SortedList* createList();
int isListEmpty(SortedList *);
void insert(SortedList *, duck *);
ListNode *front(SortedList *);
ListNode *rear(SortedList *);
void remove_node(SortedList *);
void solve(solution *, SortedList *, int *, int);
void merge(solution *v, int i, int m, int j);
void merge_sort(solution *v, int i, int j);

int main() {
	int i, n, m, k, p, line, col, id;
	char direction;
	FILE *f = fopen("ratisoare.in", "r");
	fscanf(f, "%d %d %d %d\n", &n, &m, &k, &p);
	SortedList **l = (SortedList **) malloc(n * sizeof(SortedList *));
	for (i = 0; i < n; i++) {
		l[i] = createList();
	}
	for (i = 0; i < p; i++) {
		// read input
		fscanf(f, "%d %d %d %c\n", &id, &line, &col, &direction);
		// create duck instance
		duck *d = (duck *) malloc(sizeof(duck));
		d->id = id;
		d->pos = col;
		// set direction
		if (direction == 'D') {
			d->dir = 1;
		} else {
			d->dir = -1;
		}
		insert(l[line - 1], d);
	}
	fclose(f);
	int index = 0;
	solution *s = (solution *) malloc(p * sizeof(solution));
	for (i = 0; i < n; i++) {  // for each line, solve
		solve(s, l[i], &index, m);
	}
	merge_sort(s, 0, p - 1);  // sort solution array ascending by time and id
	FILE *o = fopen("ratisoare.out", "w");
	fprintf(o, "%d\n", s[k - 1].id);
	fclose(o);
	for (i = 0; i < n; i++) {
		free(l[i]);
	}
	free(l);
	return 0;
}

void solve(solution *s, SortedList *l, int *i, int width) {
	ListNode *d;
	int li = 0, ri = 0, j;
	ListNode **left = (ListNode **) malloc(l->size * sizeof(ListNode *));
	ListNode **right = (ListNode **) malloc(l->size * sizeof(ListNode *));
	d = front(l);
	// build left and right vectors
	while (d != NULL) {
		if (d->elem->dir == -1) {
			// if it's facing left, add it in the left vector
			left[li] = d;
			li++;
		} else {
			// else add it in the right vector
			right[ri] = d;
			ri++;
		}
		d = d->next;
	}
	d = front(l);
	// extract ducks from left to right
	for (j = 0; j < li; j++) {  // for each left facing duck
		s[*i].id = d->elem->id;
		s[*i].time = left[j]->elem->pos;
		*i = *i + 1;
		d = d->next;
	}
	for (j = 0; j < ri; j++) {  // for each right facing duck
		s[*i].id = d->elem->id;
		s[*i].time = width - right[j]->elem->pos + 1;
		*i = *i + 1;
		d = d->next;
	}
	// free allocated memory
	for (j = 0; j < li + ri; j++) {
		remove_node(l);
	}
	free(left);
	free(right);
}

void merge(solution *v, int i, int m, int j) {  // merge 2 sorted arrays
	int iinit = i;
	solution *u = (solution *) calloc(j - i + 1, sizeof(solution));
	int l = 0;
	int k = m + 1;
	while (i <= m && k <= j) {
		if (v[i].time < v[k].time) {  // time first
			u[l++] = v[i++];
		} else {
			if (v[i].time == v[k].time) {
				if (v[i].id < v[k].id) {  // id second
					u[l++] = v[i++];
				} else {
					u[l++] = v[k++];
				}
			} else {
				u[l++] = v[k++];
			}
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

void merge_sort(solution *v, int i, int j) {
	if (i < j) {
		int m = (i + j) / 2;
		merge_sort(v, i, m);
		merge_sort(v, m + 1, j);
		merge(v, i, m, j);
	}
}

SortedList* createList() {  // create empty list
	SortedList *q = (SortedList *) malloc(sizeof(SortedList));
	q->front = NULL;
	q->rear = NULL;
	q->size = 0;
	return q;
}

int isListEmpty(SortedList *q) {  // check if list is empty
	if (q->size == 0) {
		return 1;
	}
	return 0;
}

void insert(SortedList *q, duck *elem) {
	ListNode *node = (ListNode *) malloc(sizeof(ListNode));
	node->elem = elem;
	node->next = NULL;
	node->prev = NULL;
	if (isListEmpty(q)) {
		q->size = 1;
		q->front = node;
		q->rear = node;
		return;
	}
	q->size += 1;
	if (q->front->elem->pos > elem->pos) {
		node->next = q->front;
		q->front->prev = node;
		q->front = node;
		return;
	}
	ListNode *aux1 = q->front;
	ListNode *aux2 = q->front->next;
	if (aux2 == NULL) {  // if the list had only one element
		aux1->next = node;
		node->prev = aux1;
		q->rear = node;
		return;
	}
	while (aux2 != NULL) {
		if (elem->pos < aux2->elem->pos) {  // then insert between aux1 and aux 2
			aux1->next = node;
			aux2->prev = node;
			node->next = aux2;
			node->prev = aux1;
			return;
		}
		aux1 = aux2;
		aux2 = aux2->next;
	}
	// insert last
	q->rear->next = node;
	node->prev = q->rear;
	q->rear = node;
}

ListNode *front(SortedList *q) {  // peek front of the list
	return q->front;
}

ListNode *rear(SortedList *q) {  // peek rear of the list
	return q->rear;
}

void remove_node(SortedList* q) {
	if (isListEmpty(q)) {
		return;
	}
	ListNode *aux;
	q->size -= 1;
	// remove first node
	aux = q->front;
	if (aux->next != NULL) {
		aux->next->prev = NULL;
	}
	q->front = q->front->next;
	if (q->front == NULL) {
		q->rear = NULL;
	}
	free(aux);
}
