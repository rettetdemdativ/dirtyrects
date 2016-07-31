// Anna & Michael
// July 2016

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

struct rect
{
	uint32_t x1, y1, x2, y2;
};

struct rect *rect_arr, *upd_arr;
int rect_arr_length = 0, upd_arr_length = 0;

void get_update_rects()
{
	for (int i = 0; i < upd_arr_length; i++) {
		printf("start: %d, end: %d\n", upd_arr[i].y1, 
		       upd_arr[i].y2);
	}
}

int add_rect(struct rect *rectangle)
{
	struct rect *temp =
		(struct rect*)realloc(rect_arr, (rect_arr_length+1)*sizeof(struct rect));
	if (temp == NULL) {
		printf("Error allocating memory!\n");
		return 1;
	}
	rect_arr = temp;
	rect_arr[rect_arr_length++] = *rectangle;
	return 0;
}

int rem_rect(int index)
{
	for (int i = 0; i < rect_arr_length; i++) {
		if (index == i) {
			continue;
		}
		rect_arr[i] = rect_arr[i];
	}
	struct rect *temp =
		(struct rect*)realloc(rect_arr, (rect_arr_length-1)*sizeof(struct rect));
	if(temp == NULL) {
		printf("Error allocating memory!\n");
		return 1;
	}
	rect_arr = temp;
	rect_arr_length--;
	return 0;
}

int add_upd(const struct rect rectangle)
{
	struct rect *temp =
		(struct rect*)realloc(upd_arr, (upd_arr_length+1)*sizeof(struct rect));
	if (temp == NULL) {
		printf("Error allocating memory!\n");
		return 1;
	}
	upd_arr = temp;
	upd_arr[upd_arr_length++] = rectangle;
	return 0;
}

int compare_rects(const void *a, const void *b)
{
	struct rect* rect_a = (struct rect*)a;
	struct rect* rect_b = (struct rect*)b;
	return (int)(rect_a->y1 - rect_b->y1);
}

bool rects_overlap(const struct rect *dirty, const struct rect *prev_dirty)
{
	if (prev_dirty->y2 < dirty->y1) {
		return false;
	}
	return true;
}

struct rect fix_overlap(const struct rect *dirty, const struct rect *prev_dirty)
{
	struct rect fixed_rect;
	fixed_rect.x1 =
		(dirty->x1 < prev_dirty->x1 ? dirty->x1 : prev_dirty->x1);
	fixed_rect.y1 =
		(dirty->y1 < prev_dirty->y1 ? dirty->y1 : prev_dirty->y1);
	fixed_rect.x2 =
		(dirty->x2 > prev_dirty->x2 ? dirty->x2 : prev_dirty->x2);
	fixed_rect.y2 =
		(dirty->y2 > prev_dirty->y2 ? dirty->y2 : prev_dirty->y2);
	return fixed_rect;
}

void mark_dirty_rect(int index, const struct rect *dirty)
{
	if (index == 0) {
		add_upd(*dirty);
		return;
	}

	if (rects_overlap(dirty, &upd_arr[upd_arr_length-1])) {
		struct rect fixed_rect =
			fix_overlap(dirty, &upd_arr[upd_arr_length-1]);
		upd_arr[upd_arr_length-1] = fixed_rect;
		return;
	}
	add_upd(*dirty);
}

void comb()
{
	for (int i = 0; i < rect_arr_length; i++) {
		mark_dirty_rect(i, &rect_arr[i]);
	}
}

int main(int argc, char **argv)
{
	struct rect d1, d2, d3, d4, d5, d6;
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

	d5.x1 = 500;
	d5.y1 = 300;
	d5.x2 = 700;
	d5.y2 = 400;

	d6.x1 = 500;
	d6.y1 = 350;
	d6.x2 = 600;
	d6.y2 = 750;

	add_rect(&d1);
	add_rect(&d2);
	add_rect(&d3);
	add_rect(&d4);
	add_rect(&d5);
	add_rect(&d6);

	qsort(rect_arr, rect_arr_length, sizeof(struct rect), compare_rects);
	comb();

	get_update_rects();
	return 0;
}
