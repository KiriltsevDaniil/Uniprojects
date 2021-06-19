#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "structs.h"
#include "PNGio.h"

#define PNGSIGSIZE 8



int rfile(struct Img* img, char* fname){

	FILE *fp = fopen(fname, "rb");
	int type_flag = 0;
    png_byte buff[PNGSIGSIZE]; 
    
    if(!fp){
        if(fname == NULL)
            return -1;
        else
            printf("Chosen file doesn't exist.\n");
        return 0; 
    }
    fread(buff, 1, PNGSIGSIZE, fp);
    type_flag = !png_sig_cmp(buff, 0, PNGSIGSIZE);
    if (!type_flag){
        printf("File is not a PNG image.\n");
    	fclose(fp);
        return 0;
    } 
    else{
    	img->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!img->png_ptr){
        	fclose(fp);
            printf("Error during creation of png_read_struct\n");
            return 0;
        }
      
        img->info_ptr = png_create_info_struct(img->png_ptr);
        if(!img->info_ptr) {
            png_destroy_read_struct(&img->png_ptr, NULL, NULL);
            fclose(fp);
            printf("Error during creation of png_info_struct\n");
            return 0;
        }
        if(setjmp(png_jmpbuf(img->png_ptr))){
            png_destroy_read_struct(&img->png_ptr, &img->info_ptr, NULL);
            fclose(fp);
            printf("Image reading error.\n");            
            return 0;
        }
       	png_init_io(img->png_ptr, fp);
        png_set_sig_bytes(img->png_ptr, PNGSIGSIZE);
        png_read_info(img->png_ptr, img->info_ptr);

        img->width = png_get_image_width(img->png_ptr, img->info_ptr);
	    img->height = png_get_image_height(img->png_ptr, img->info_ptr);
	    img->color_type = png_get_color_type(img->png_ptr, img->info_ptr);
	    img->bit_depth = png_get_bit_depth(img->png_ptr, img->info_ptr);
        

	    img->nop = png_set_interlace_handling(img->png_ptr);
	    png_read_update_info(img->png_ptr, img->info_ptr);

	    if (setjmp(png_jmpbuf(img->png_ptr))){
            printf("Image reading error.\n");    
	    	return 0;
        }

	    img->row_pts = (png_bytep *) malloc(sizeof(png_bytep) * img->height);
	    for (int y = 0; y < img->height; y++)
	        img->row_pts[y] = (png_byte *) malloc(png_get_rowbytes(img->png_ptr, img->info_ptr));

	    png_read_image(img->png_ptr, img->row_pts);

	    fclose(fp);

        return 1;
    }     
}

int wfile(struct Img *img, char* fname){
    int x,y;
    FILE *fp;
    if(fname == NULL)
        fp = fopen("result.png", "wb");
    else
        fp = fopen(fname, "wb");

    if (!fp){
        printf("File openning error.\n");
        return 0;
    }

    img->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!img->png_ptr){
        printf("Error during creation of png_write_struct\n");
        return 0;
    }

    img->info_ptr = png_create_info_struct(img->png_ptr);
    if (!img->info_ptr){
        printf("Error during creation of png_info_struct\n");
        return 0;
    }

    if (setjmp(png_jmpbuf(img->png_ptr))){
        printf("Error in png_init_io\n");
        return 0;
    }

    png_init_io(img->png_ptr, fp);


    if (setjmp(png_jmpbuf(img->png_ptr))){
        printf("Header writing  error\n");
        return 0;
    }

    png_set_IHDR(img->png_ptr, img->info_ptr, img->width, img->height,
                 img->bit_depth, img->color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(img->png_ptr, img->info_ptr);


    if (setjmp(png_jmpbuf(img->png_ptr))){
        printf("Bytes writing  error\n");
        return 0;
    }

    png_write_image(img->png_ptr, img->row_pts);

    if (setjmp(png_jmpbuf(img->png_ptr))){
        printf("End of write error\n");
        return 0;
    }

    png_write_end(img->png_ptr, NULL);

    for (y = 0; y < img->height; y++)
        free(img->row_pts[y]);
    free(img->row_pts);
    fclose(fp);
    png_destroy_write_struct(&img->png_ptr, &img->info_ptr);
    return 1;
}