#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct rect
{
	uint32_t x1, y1, x2, y2;
};

struct rect *rect_arr;
int rect_arr_length = 0;

void get_update_rects()
{
	for (int i = 0; i < rect_arr_length; i++) {
		printf("start: %d, end: %d\n", rect_arr[i].y1, 
		       rect_arr[i].y2);
	}
}

int add_rect(struct rect *rectangle)
{
	struct rect *temp = realloc(rect_arr, (rect_arr_length+1)*sizeof(struct rect));
	if (temp == NULL) {
		printf("Error allocating memory!\n");
		return 1;
	}
	rect_arr = temp;
	rect_arr[rect_arr_length++] = *rectangle;
	return 0;
}

int compare_rects(const void *a, const void *b)
{
	struct rect* rect_a = (struct rect*)a;
	struct rect* rect_b = (struct rect*)b;
	return (int)(rect_a->y1 - rect_b->y1);
}

void mark_dirty_rect(const struct rect *dirty)
{
}

int main(int argc, char **argv)
{
	struct rect d1, d2, d3, d4;
	d1.x1 = 100;
	d1.y1 = 50;
	d1.x2 = 250;
	d1.y2 = 180;

	d2.x1 = 150;
	d2.y1 = 150;
	d2.x2 = 1024;
	d2.y2 = 200;

	d3.x1 = 256;
	d3.y1 = 768;
	d3.x2 = 1664;
	d3.y2 = 1024;

	d4.x1 = 300;
	d4.y1 = 600;
	d4.x2 = 1050;
	d4.y2 = 1080;

	add_rect(&d1);
	add_rect(&d2);
	add_rect(&d3);
	add_rect(&d4);

	qsort(rect_arr, rect_arr_length, sizeof(struct rect), compare_rects);

	get_update_rects();
	return 0;
}
