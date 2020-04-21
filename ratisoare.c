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

typedef struct ListNode{
	duck *elem;
	struct ListNode *next;
	struct ListNode *prev;
}ListNode;  // node data type

typedef struct SortedList{
	ListNode *front;
	ListNode *rear;
	int size;
}SortedList;  // sorted list data type

SortedList* createList(){  // create empty list
	SortedList *q = (SortedList *) malloc(sizeof(SortedList));
	q->front = NULL;
	q->rear = NULL;
	q->size = 0; 
	return q;
} 

int isListEmpty(SortedList *q){  // check if list is empty
	if (q->size == 0) {
		return 1;
	}
	return 0;
}

void insert(SortedList *q, duck *elem){
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

ListNode *front(SortedList *q){  // peek front of the list
	return q->front;
}

ListNode *rear(SortedList *q) {  // peek rear of the list
	return q->rear;
}

void remove_node(SortedList* q, int rear){ // removes an element depending on rear = 1 / 0
	if (isListEmpty(q)) {
		return;
	}
	ListNode *aux;
	q->size -= 1;
	if (rear) {  // remove last node
		aux = q->rear;
		q->rear = aux->prev;
		if (q->rear == NULL) {
			q->front = NULL;
		}
		else {
			q->rear->next = NULL;
		}
		free(aux);
		return;
	}
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

void solve(solution *, SortedList *, int *, int, int);
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
		fscanf(f, "%d %d %d %c\n", &id, &line, &col, &direction);
		duck *d = (duck *) malloc(sizeof(duck));
		d->id = id;
		d->pos = col;
		if (direction == 'D') {
			d->dir = 1;
		}
		else {
			d->dir = -1;
		}
		insert(l[line - 1], d);
	}
	fclose(f);
	int index = 0;
	solution *s = (solution *) malloc(p * sizeof(solution));
	for (i = 0; i < n; i++) {
		printf("LINIA %d\n", i);
		solve(s, l[i], &index, m, 0);
		printf("urmatoarea linie\n");
	}
	merge_sort(s, 0, p - 1);
	FILE *o = fopen("ratisoare.out", "w");
	fprintf(o, "%d\n", s[k - 1].id);
	for (i = 0; i < p; i++) {
		printf("rata %d:  %d\n", s[i].id, s[i].time);
	}
	fclose(o);
	for (i = 0; i < n; i++) {
		free(l[i]);
	}
	free(l);
	return 0;
}

void solve(solution *s, SortedList *l, int *i, int width, int time) {
	ListNode *d;
	printf("%d = size\n", l->size);
	int *moved = (int *) calloc(l->size, sizeof(int));
	// take out all the ducks facing <-
	d = front(l);
	while (d != NULL && d->elem->dir == -1) {
		printf("intru ca se uita in stanga\n");
		s[*i].id = d->elem->id;
		s[*i].time = time + d->elem->pos;
		remove_node(l, 0);  // remove first node
		d = front(l);
		*i = *i + 1;
		printf("ies\n");
	}
	// take out all the ducks facing ->
	d = rear(l);
	while (d != NULL && d->elem->dir == 1) {
		printf("intru ca se uita in dreapta\n");
		s[*i].id = d->elem->id;
		s[*i].time = time + (width - d->elem->pos) + 1;
		remove_node(l, 1);  // remove last node
		d = rear(l);
		*i = *i + 1;
		printf("ies\n");
	}
	printf("nu da return\n");
	if (isListEmpty(l)) {
		return;
	}
	// at this point there are at least 2 ducks: -> /// <-
	int i1, i2, i3, i4;
	ListNode *aux1 = l->front;
	ListNode *aux2 = aux1->next;
	i1 = 0;
	i2 = 1;
	ListNode *aux3, *aux4;

	//printf("inainte de while\n");
	 // start looking for collisions
	while (aux2 != NULL) {
		printf("intru in while de coliziune\n");
		if (aux1->elem->dir == 1 && aux2->elem->dir == -1) {  // ->...<-
			if (aux2->elem->pos - aux1->elem->pos < 3){  // if there is a collision
				if (aux2->elem->pos - aux1->elem->pos == 1) {  // -><-
					aux1->elem->dir = -1;
					aux2->elem->dir = 1;
					moved[i1] = 1;
					moved[i2] = 1;
					// start searching backwards for collisions
					aux3 = aux1->prev;
					aux4 = aux1;
					i4 = i1;
					i3 = i1 - 1;
					while (aux3 != NULL) {
						if ((aux3->elem->dir == 1 && aux4->elem->dir == -1) 
						&& (aux4->elem->pos - aux3->elem->pos == 1)) {  // -><-
							aux3->elem->dir = -1;
							aux4->elem->dir = 1;
							moved[i3] = 1;
							moved[i4] = 1;
						} else {  // no collision backwards
							break;
						}
						aux4 = aux3;
						aux3 = aux3->prev;
						i4 = i3;
						i3--;
					}
				}
				if (aux2->elem->pos - aux1->elem->pos == 2) { // ->_<-
					aux1->elem->dir = -1;
					aux2->elem->dir = 1;
					aux1->elem->pos = aux1->elem->pos + 1;
					aux2->elem->pos = aux2->elem->pos - 1;
					moved[i1] = 1;
					moved[i2] = 1;
				}
			}
		}
		aux1 = aux2;
		aux2 = aux2->next;
		i1 = i2;
		i2++;
	}
	aux1 = l->front;
	for (i1 = 0; aux1 != NULL; i1++, aux1 = aux1->next) {
		if (moved[i1]) {  // the duck has collided and moved
			continue;
		}
		aux1->elem->pos = aux1->elem->pos + aux1->elem->dir;
	}
	free(moved);
	printf("TERMIN FUNCTIA\n");
	solve(s, l, i, width, time + 1);
}

void merge(solution *v, int i, int m, int j) {
	int iinit = i;
	solution *u = (solution *) calloc(j - i + 1, sizeof(solution));
	int l = 0;
	int k = m + 1;
	while (i <= m && k <= j) {
		if (v[i].time < v[k].time) {
			u[l++] = v[i++];
		}
		else {
			if (v[i].time == v[k].time) {
				if (v[i].id < v[k].id) {
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