// Anna & Michael
// July 2016

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define MAX_WIDTH 1920
#define MAX_HEIGHT 1080

struct rect
{
	uint32_t x1, y1, x2, y2;
};

bool display[MAX_WIDTH][MAX_HEIGHT];

void print_dsp()
{
	for (int i = 0; i < MAX_HEIGHT; ++i) {
		int filled_blks_row = 0;
		for (int j = 0; j < MAX_WIDTH; ++j) {
			if (display[j][i] == true) {
				filled_blks_row++;
			}
		}
		if (filled_blks_row > 0) {
			printf("%d: %d\n", i, filled_blks_row);
		}
	}
}

void find_dsp_rect(struct rect *dsp_rect, int *start_x, int *start_y)
{
	dsp_rect->x1 = *start_x;
	dsp_rect->y1 = *start_y;
	for (int i = *start_x; i < MAX_WIDTH; ++i) {
		for (int j = *start_y; j < MAX_HEIGHT; ++j) {
			if (display[i][j+1] == false) {
				if (i > *start_x && j != dsp_rect->y2) {
					dsp_rect->x2 = i-1;
					return;
				}
				dsp_rect->y2 = j;
				break;
			}
		}
	}
}

void clear_chkd_area(struct rect *last_dsp_rect)
{
	for (int i = last_dsp_rect->x1; i <= last_dsp_rect->x2; ++i) {
		for (int j = last_dsp_rect->y1; j <= last_dsp_rect->y2; ++j) {
				//printf("%d, %d\n", i, j);
				display[i][j] = false;
		}
	}
}

void get_update_rects()
{
	for (int i = 0; i < MAX_HEIGHT; ++i) {
		for (int j = 0; j < MAX_WIDTH; ++j) {
			if (display[j][i] == true) {
				struct rect dsp_rect;
				find_dsp_rect(&dsp_rect, &j, &i);
				clear_chkd_area(&dsp_rect);
				printf("start: %d, %d; end: %d, %d\n",
				       dsp_rect.x1*64, dsp_rect.y1*64,
				       dsp_rect.x2*64, dsp_rect.y2*64);
			}
		}
	}
}

void fill_dsp_blocks(struct rect *base, int size)
{
	for (int i = 0; i < size; ++i) {
		uint32_t start_x, start_y, end_x, end_y;
		start_x = base[i].x1/64;
		start_y = base[i].y1/64;
		end_x   = ceil(base[i].x2/64);
		end_y   = ceil(base[i].y2/64);
		printf("startx: %d, starty: %d, endx: %d, endy: %d\n",
		       start_x, start_y, end_x, end_y);
		for (int j = start_x; j <= end_x; j++) {
			for (int k = start_y; k <= end_y; k++) {
				display[j][k] = true;
			}
		}
	}
}

int insert_rect(struct rect *base, struct rect *rec, uint32_t *size)
{
	struct rect *temp =
		(struct rect*)realloc(base, (*size+1)*sizeof(struct rect));
	if (temp == NULL) {
		printf("Error allocating memory!\n");
		return 1;
	}
	base = temp;
	base[(*size)++] = *rec;
	return 0;
}

int main(int argc, char **argv)
{
	uint32_t rect_arr_length = 0;
	struct rect *rect_arr =
		(struct rect*)malloc(rect_arr_length*sizeof(struct rect));
	struct rect r1, r2, r3, r4, r5;
	r1.x1 = 95;
	r1.y1 = 32;
	r1.x2 = 256;
	r1.y2 = 155;

	r2.x1 = 576;
	r2.y1 = 64;
	r2.x2 = 767;
	r2.y2 = 320;

	r3.x1 = 480;
	r3.y1 = 200;
	r3.x2 = 832;
	r3.y2 = 352;

	r4.x1 = 256;
	r4.y1 = 576;
	r4.x2 = 512;
	r4.y2 = 703;

	r5.x1 = 257;
	r5.y1 = 703;
	r5.x2 = 383;
	r5.y2 = 767;

	insert_rect(rect_arr, &r1, &rect_arr_length);
	insert_rect(rect_arr, &r2, &rect_arr_length);
	insert_rect(rect_arr, &r3, &rect_arr_length);
	insert_rect(rect_arr, &r4, &rect_arr_length);
	insert_rect(rect_arr, &r5, &rect_arr_length);

	fill_dsp_blocks(rect_arr, rect_arr_length);
	print_dsp();
	get_update_rects();

	return 0;
}
