#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Bitmap {
	int *data;
	int size;
} Bitmap;

Bitmap *init(int n = 1) {
	Bitmap *p = (Bitmap*)malloc(sizeof(Bitmap));
	p->size = n;
	p->data = (int*)calloc(n, sizeof(int));
	return p;
}

void expand_bitmap(Bitmap *p, int n) {
	int *temp = (int*)calloc(n, sizeof(int));
	memcpy(temp, p->data, sizeof(int) * p->size);
	free(p->data);
	p->data = temp;
	p->size = n;
}

void set_one(Bitmap *p, int n) {
	if (p->size <= (n / 30 + 1)) {
		expand_bitmap(p, n / 30 + 1);
	}
	p->data[n / 30] |= (1 << (n % 30));
}

int is_one(Bitmap *p, int n) {
	if (n / 30 + 1 > p->size) {
		expand_bitmap(p, n / 30 + 1);
	}

	return p->data[n / 30] & (1 << (n % 30));
}

void left_bitmap(Bitmap *p, int n) {
	is_one(p, n);
	p->data[0] <<= 1;
	for (int i = 1; i < p->size; i++) {
		p->data[i] <<= 1;
		p->data[i] |= !!(p->data[i - 1] & (1 << 30));
	}
}

void and_bitmap(Bitmap *bm1, Bitmap *bm2, int n) {
	is_one(bm1, n);
	is_one(bm2, n);
	int temp = 0, index = 0;
	while (temp < n) {
		bm1->data[index] &= bm2->data[index];
		temp += 30;
		index++;
	}
}

void clear(Bitmap*);

void shift_and(char *str, char *target) {
	Bitmap *cnt[256];
	for (int i = 0; i < 256; i++) {
		cnt[i] = init();
	}
	Bitmap *p = init();
	int n = 0;
	for (int i = 0; target[i]; i++) {
		set_one(cnt[target[i]], i);
		n++;
	}

	for (int i = 0; str[i]; i++) {
		left_bitmap(p, n);
		set_one(p, 0);
		and_bitmap(p, cnt[str[i]], n);
		if (is_one(p, n - 1)) {
			printf("matching success!!!\n");
			return ;
		}
	}
	printf("matching failed!!!\n");
	clear(p);
	for (int i = 0; i < 256; i++) {
		clear(cnt[i]);
	}
}

void clear(Bitmap *p) {
	free(p->data);
	free(p);
}

int main() {

	char str[1000];
    char target[1000];
	while (scanf("%s%s", str, target) != EOF) {
		shift_and(str, target);
	}


	return 0;
}
