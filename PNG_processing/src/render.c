#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "structs.h"
#include "render.h"

int drectangle(struct Img *img, int x1, int y1, int x2, int y2, int thick, int filled, int red, int green, int blue, int alpha, int d_red, int d_green, int d_blue, int d_alpha) {
    int x,y;
    if (png_get_color_type(img->png_ptr, img->info_ptr) == PNG_COLOR_TYPE_RGB){
        printf("input file must be PNG_COLOR_TYPE_RGBA\n");
    	return 0;
    }

    if (png_get_color_type(img->png_ptr, img->info_ptr) != PNG_COLOR_TYPE_RGBA){
        printf("input file must be PNG_COLOR_TYPE_RGBA\n");
        return 0;
    }

    for (y = 0; y < img->height; y++) {
        png_byte* row = img->row_pts[y];
        for (x = 0; x < img->width; x++){
            png_byte* ptr = &(row[x * 4]);
            if(filled){
	            if(((y > y1) && (y < y2)) && ((x < x2) && (x > x1))){ 
					ptr[0] = d_red;
			        ptr[1] = d_green;
			        ptr[2] = d_blue;
			        ptr[3] = d_alpha;	            
			    }
	        }
            for(int i = thick * -1; i <= thick; i++ ){
                if(((x == x2 + i) || (x == x1 + i)) && (y <= y2 + thick) && (y >= y1 - thick) || ((y == y2 + i) || (y == y1 + i)) && (x <= x2 + thick) && (x >= x1 + thick)){ 
                    ptr[0] = red;
                    ptr[1] = green;
                    ptr[2] = blue;
                    ptr[3] = alpha;
                }
            }
        }
    }
    return 1;
}

int dgrid(struct Img *img, int x_partition, int y_partition,int thick, int red, int green, int blue, int alpha) {
    int x,y;
    if (png_get_color_type(img->png_ptr, img->info_ptr) == PNG_COLOR_TYPE_RGB){
        printf("input file is PNG_COLOR_TYPE_RGB must be PNG_COLOR_TYPE_RGBA\n");
        return 0;
    }

    if (png_get_color_type(img->png_ptr, img->info_ptr) != PNG_COLOR_TYPE_RGBA){
        printf("input file must be PNG_COLOR_TYPE_RGBA\n");
        return 0;
    }

    for (y = 0; y < img->height; y++) {
        png_byte* row = img->row_pts[y];
        for (x = 0; x < img->width; x++){
            png_byte* ptr = &(row[x * 4]);
            for(int i = thick * -1; i <= thick; i++ ){
	            if((((x + i) % (img->width/x_partition)) == 0 || ((y + i) % (img->height/y_partition)) == 0)){ 
			        ptr[0] = red;
			        ptr[1] = green;
			        ptr[2] = blue;
			        ptr[3] = alpha;	
			    }
			}
        }
    }
    return 1;
}

int rotate(struct Img* img, int x1, int y1, int x2, int y2, int angle) {
    if (png_get_color_type(img->png_ptr, img->info_ptr) == PNG_COLOR_TYPE_RGB){
        printf("input file is PNG_COLOR_TYPE_RGB must be PNG_COLOR_TYPE_RGBA\n");
        return 0;
    }

    if (png_get_color_type(img->png_ptr, img->info_ptr) != PNG_COLOR_TYPE_RGBA){
        printf("input file must be PNG_COLOR_TYPE_RGBA\n");
        return 0;
    }
    int new_height, new_width;
    png_bytep * rownewnew_pts;

    if(angle == 90){
        new_height = x2 - x1;
        new_width = y2 - y1;
        rownewnew_pts = (png_bytep *) malloc(sizeof(png_bytep) * new_height);
        for (int y = 0; y < new_height; y++){
            rownewnew_pts[y] = (png_byte *)malloc(new_width * 4);
        }
        for (int y = y1, y_new = new_width - 1; y < y2; y++, y_new--) {
            png_byte * row = img->row_pts[y];
            for (int x = x1, x_new = 0; x < x2; x++, x_new++){
                png_byte* ptr = &(row[x * 4]);
                png_byte * row_new = rownewnew_pts[x_new];
                png_byte * ptr_new = &(row_new[y_new * 4]);
                for(int k=0; k<4; k++){
                    ptr_new[k] = ptr[k];}
            }
        }
    }
    if(angle == 180){
        new_height = y2 - y1;
        new_width = x2 - x1;
        rownewnew_pts = (png_bytep *) malloc(sizeof(png_bytep) * new_height);
        for (int y = 0; y < new_height; y++){
            rownewnew_pts[y] = (png_byte *)malloc(new_width * 4);
        }
        for (int y = y1, y_new = new_height-1; y < y2; y++, y_new--) {
            png_byte * row = img->row_pts[y];
            png_byte * row_new = rownewnew_pts[y_new];
            for (int x = x1, x_new = new_width-1; x < x2; x++, x_new--){
                png_byte* ptr = &(row[x * 4]);
                png_byte * ptr_new = &(row_new[x_new * 4]);
                for(int k=0; k<4; k++)
                    ptr_new[k] = ptr[k];
            }
        }
    }

    if(angle == 270){
        new_height = x2 - x1;
        new_width = y2 - y1;
        rownewnew_pts = (png_bytep *) malloc(sizeof(png_bytep) * new_height);
        for (int y = 0; y < new_height; y++){
            rownewnew_pts[y] = (png_byte *)malloc(new_width * 4 );
        }

        for (int y = y1, y_new = 0; y < y2; y++, y_new++) {
            png_byte * row = img->row_pts[y];
            for (int x = x1, x_new = new_height-1; x < x2; x++, x_new--){
                png_byte* ptr = &(row[x * 4]);
                png_byte * row_new = rownewnew_pts[x_new];
                png_byte * ptr_new = &(row_new[y_new * 4]);
                for(int k=0; k<4; k++){
                    ptr_new[k] = ptr[k];}
            }
        }
    }

    for (int y = 0; y < img->height; y++)
        free(img->row_pts[y]);
    free(img->row_pts);

    img->height = new_height;
    img->width = new_width;
    img->row_pts = (png_bytep *) malloc(sizeof(png_bytep) * new_height);
    for (int y = 0; y < new_height; y++){
            img->row_pts[y] = (png_byte *)malloc(new_width * 4);
        }

    for(int y = 0; y < new_height; y++){
        png_byte* row = img->row_pts[y];
        png_byte* row_cpy = rownewnew_pts[y];
        for (int x = 0; x < new_width; x++){
            png_byte* ptr = &(row[x * 4]);
            png_byte* ptr_new = &(row_cpy[x * 4]);
            for(int k=0; k<4; k++){
                ptr[k] = ptr_new[k];
            }
        }
    }
    for (int y = 0; y < img->height; y++)
        free(rownewnew_pts[y]);
    free(rownewnew_pts);
    return 1;
}

int dborder(struct Img *img, int option, int thick, int red, int green, int blue, int alpha){
    int x, y;
    if (png_get_color_type(img->png_ptr, img->info_ptr) == PNG_COLOR_TYPE_RGB){
        printf("input file is PNG_COLOR_TYPE_RGB must be PNG_COLOR_TYPE_RGBA\n");
        return 0;
    }

    if (png_get_color_type(img->png_ptr, img->info_ptr) != PNG_COLOR_TYPE_RGBA){
        printf("input file must be PNG_COLOR_TYPE_RGBA\n");
        return 0;
    }
    if(option == 1){
        for (y = 0; y < img->height; y++){
            png_byte* row = img->row_pts[y];
            for (x = 0; x < img->width; x++){
                png_byte* ptr = &(row[x * 4]);
                if((y <= thick && ((x % 6 != 0 && y % 3 == 0) || ((x % 6 > 1 && x % 6 < 5) && y % 3 == 1) || (x % 6 == 3 && y % 3 == 2))) || \
                    ( x >= img->width - thick -1 && ((x % 3 == 2 && y % 6 != 0) || ((y % 6 > 1 && y % 6 < 5) && x % 3 == 1) || (y % 6 == 3 && x % 3 == 0))) || \
                    (x <= thick && ((y % 6 != 0 && x % 3 == 0) || ((y % 6 > 1 && y % 6 < 5) && x % 3 == 1) || (y % 6 == 3 && x % 3 == 2))) || \
                    (y >= img->height - thick -1 && ((x % 6 != 0 && y % 3 == 2) || ((x % 6 > 1 && x % 6 < 5) && y % 3 == 1) || (x % 6 == 3 && y % 3 == 0)))){
                    ptr[0] = red;
                    ptr[1] = green;
                    ptr[2] = blue;
                    ptr[3] = alpha; 
                }
            }
        }
    }
    else if(option == 2){
        for (y = 0; y < img->height; y++){
            png_byte* row = img->row_pts[y];
            for (x = 0; x < img->width; x++){
                png_byte* ptr = &(row[x * 4]);
                if((y <= thick && ((x % 2 == 0 && y%2 == 1)|| (x % 2 == 1 && y%2 == 0))) || (x <= thick &&\
                ((x % 2 == 0 && y%2 == 1)|| (x % 2 == 1 && y%2 == 0))) || (x >= img->width - thick-1 &&\
                ((x % 2 == 0 && y%2 == 1)|| (x % 2 == 1 && y%2 == 0))) ||  (y >= img->height - thick-1 &&\
                ((x % 2 == 0 && y%2 == 1)|| (x % 2 == 1 && y%2 == 0)))){ 
                    ptr[0] = red;
                    ptr[1] = green;
                    ptr[2] = blue;
                    ptr[3] = alpha; 
                }
            }
        }
    }

    else if(option == 3){
        for (y = 0; y < img->height; y++){
            png_byte* row = img->row_pts[y];
            for (x = 0; x < img->width; x++){
                png_byte* ptr = &(row[x * 4]);

                if( (y <= thick && (y % 3 == 0 || (x%3 ==2 && y%3 == 2) || (x%3 == 1 && y%3 == 1))) || \
                    (x <= thick && (y % 3 == 0 || x == 0 || (x%3 ==2 && y%3 == 2) || (x%3 == 1 && y%3 == 1))) || \
                    (x >= img->width - thick-1 && (y % 3 == 0 || y == img->width  || (x%3 ==2 && y%3 == 2) || (x%3 == 1 && y%3 == 1))) || \
                    (y >= img->height - thick-1 && (y % 3 == 0 || (y  == img->height) || (x%3 ==2 && y%3 == 2) || (x%3 == 1 && y%3 == 1))) || \
                    (y == 0) || (x == 0) || (y == img->height - 1) || (x == img->width - 1)){ 

                    ptr[0] = red;
                    ptr[1] = green;
                    ptr[2] = blue;
                    ptr[3] = alpha; 
                }
            }
        }
    }
    return 1;
}