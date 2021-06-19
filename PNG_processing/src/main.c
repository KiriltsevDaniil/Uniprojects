#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include "structs.h"
#include "PNGio.h"
#include "render.h"

void PNG_info(struct Img* img){
	printf("\nImage height: %d\n", img->height);
	printf("Image width: %d\n", img->width);
	printf("Image color_type: %hhu\n", img->color_type);
	printf("Image bit_depth: %hhu\n", img->bit_depth);
	printf("Image interlace info: %d\n", img->bit_depth);
}

char* get_file_name(char *line){
    
    int size = 1;
    char* fname = (char *)malloc(size);
    int i = 0;

    while (i - 1 != strlen(line) + 1){
        if (i + 1 > size){
            size++;
            fname = (char *)realloc(fname, size);
        }
        fname[i++] = line[i];
    }
    fname[i - 1] = '\0';

    return fname;
}

void printHelp(){
	printf("\n\nexample of command: ./app -m 1 -p 2,2 -c 100,50,200,255 -t 10 -o 2 --input object.png\n\n");
	printf("-f, --fill - fill.\n\n");
	printf("-c, --color args: \n<red_channel_value>,<green_channel_value>,<blue_channel_value>,<alpha_channel_value> : 0...255 for each - line color.\n\n");
	printf("-t, --thickness args:  <value> : 0...25 - thickness or width of border.\n\n");
	printf("-a, --angle args:  <value> : 90, 180, 270 - angle\n\n");
	printf("-o, --option args:  <value> : 1..3 - border option\n\n");
	printf("-s, --start args:  <x_value>,<y_value> - upper left angle coordinates.\n\n");
	printf("-e, --end args:  <x_value>,<y_value> - lower right angle coordinates.\n\n");
	printf("-p, --partition args:  <x_partition>,<y_partition> : 1...20 for each - picture partitions.\n\n");
	printf("-m, --manipulation args:  <value> : 1...4 - manipulation.\n\n");
	printf("-b, --bgcolor args:  \n<red_channel_value>,<green_channel_value>,<blue_channel_value>,<alpha_channel_value> : 0...255 for each - background color.\n\n");
	printf("-r, --result args:  <file_name> output file name.\n\n");
	printf("-i, --input args:  <input_name> output file name.\n\n");
	printf("-n, --info - image info call.\n\n");
	printf("-h, --help - help call.\n\n");
}

void cleanup(struct Img* img, struct Configs* config){
	if(config->input !=  NULL)
		free(config->input);
	if(config->result != NULL)
		free(config->result);
	if(img->row_pts != NULL ){
		for (int y = 0; y < img->height; y++)
       		free(img->row_pts[y]);
    	free(img->row_pts);
    }
    printHelp();
}

int main(int argc, char* argv[]){
	struct Img img;
	int flag = 0;
	struct Configs config = {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL};
	char *opts = "m:c:t:a:o:s:e:p:b:i:r:fnh";
	struct option longOpts[] = {
		{"fill", no_argument, NULL, 'f'},
		{"help", no_argument, NULL, 'h'},
		{"color", required_argument, NULL, 'c'},
		{"thickness", required_argument, NULL, 't'},
		{"angle", required_argument, NULL, 'a'},
		{"option", required_argument, NULL, 'o'},
		{"start", required_argument, NULL, 's'},
		{"end", required_argument, NULL, 'e'},
		{"partition", required_argument, NULL, 'p'},
		{"manipulation", required_argument, NULL, 'm'},
		{"bgcolor", required_argument, NULL, 'b'},
		{"result", required_argument, NULL, 'r'},
		{"input", required_argument, NULL, 'i'},
		{"info", no_argument, NULL, 'n'},
		{ NULL, 0, NULL, 0}
	};
	int opt;
	int longIndex;
	opt = getopt_long(argc, argv, opts, longOpts, &longIndex);
		while(opt!=-1){
			switch(opt){
				case 'f':
					config.filled = 1;
					break;
				case 'n':
					flag = 1;
					break;
				case 'c':
					if(optarg) 
						sscanf(optarg, "%d,%d,%d,%d", &config.red, &config.green, &config.blue, &config.alpha);
					if((config.red > 255 || config.red < 0) || (config.green > 255 || config.green < 0) || (config.blue > 255 || config.blue < 0) ||(config.alpha > 255 || config.alpha < 0)){
						printf("\nColor value is incorrect.\n");
						cleanup(&img, &config);
						return 0;
					}
					break;
				case 'b':
					if(optarg) 
						sscanf(optarg, "%d,%d,%d,%d", &config.d_red, &config.d_green, &config.d_blue, &config.d_alpha);
					if((config.d_red > 255 || config.d_red < 0) || (config.d_green > 255 || config.d_green < 0) || (config.d_blue > 255 || config.d_blue < 0) ||(config.d_alpha > 255 || config.d_alpha < 0)){
						printf("\nBackground color value is incorrect.\n");
						cleanup(&img, &config);
						return 0;
					}
					break;
				case 't':
					if(optarg)
						config.thick = atoi(optarg);
					if(config.thick < 1 || config.thick >25){
						printf("\nThickness value is incorrect.\n");
						cleanup(&img, &config);
						return 0;
					}
					break;
				case 'a':
					if(optarg)
						config.angle = atoi(optarg);
					if((config.angle != 90) && (config.angle != 180) && (config.angle != 270)){
						printf("\nAngle value is incorrect.\n");
						cleanup(&img, &config);
						return 0;
					}
					break;
				case 'o':
					if(optarg)
						config.option = atoi(optarg);
					if((config.option != 1) && (config.option != 2) && (config.option != 3)){
						printf("\nBorder option value is incorrect.\n");
						cleanup(&img, &config);
						return 0;
					}
					break;
				case 's':
					if(optarg) 
						sscanf(optarg, "%i,%i", &config.start_x, &config.start_y);
					break;
				case 'e':
					if(optarg) 
						sscanf(optarg, "%i,%i", &config.end_x, &config.end_y);
					break;
				case 'p':
					if(optarg) 
						sscanf(optarg, "%d,%d", &config.x_partition, &config.y_partition);
					if(config.x_partition < 0 || config.y_partition < 0){
						printf("\nPartition value is incorrect.\n");
						cleanup(&img, &config);
						return 0;
					}
					break;
				case 'm':
					if(optarg)
						config.manipulation = atoi(optarg);
					if(config.manipulation > 4 && config.manipulation < 0){
						printf("\nChosen manipulation is incorrect.\n");
						cleanup(&img, &config);
						return 0;
					}
				case 'i':
					if(optarg)
						config.input = get_file_name(optarg);
					break;
				case 'r':
					if(optarg)
						config.result = get_file_name(optarg);
					break;
				case 'h':
				default:
					cleanup(&img, &config);
					return 0;
			}
			opt = getopt_long(argc, argv, opts, longOpts, &longIndex);
		}

		argc -= optind;
		argv += optind;
	int anormal = rfile(&img, config.input);
	if(anormal == -1){
		cleanup(&img, &config);
		return 0;
	}
	if(anormal){
		if((config.end_x > img.width  || config.end_x < 0) || (config.end_y > img.height || config.end_y < 0)){
			printf("\nLower right angle coordinates are incorrect.\n");
			cleanup(&img, &config);
			return 0;
		}
		if((config.start_x > img.width || config.start_x < 0) || (config.start_y > img.height || config.start_y < 0)){
			printf("\nUpper left angle coordinates are incorrect.\n");
			cleanup(&img, &config);
			return 0;				
		}
	if(anormal && flag)
		PNG_info(&img);
	}
	if(anormal){
		switch(config.manipulation){
			case 1:
				if((config.x_partition == 0) || (config.y_partition == 0) || (config.x_partition > 20) || (config.y_partition > 20)){
					printf("\nPartition value is incorrect.\n");
					cleanup(&img, &config);
					return 0;
				}	
				anormal = dgrid(&img, config.x_partition, config.y_partition, config.thick, config.red, config.green, config.blue, config.alpha);
				break;
			case 2:
				if(((config.end_x - config.start_x) < 0) || ((config.end_y - config.start_y) < 0)){
						printf("\nIncorrect coordinates.\n");
						cleanup(&img, &config);
						return 0;
					}
				anormal = drectangle(&img, config.start_x, config.start_y, config.end_x, config.end_y, config.thick, config.filled,config.red, config.green, config.blue, config.alpha ,config.d_red, config.d_green, config.d_blue, config.d_alpha);
				break;
			case 3:
				anormal = dborder(&img, config.option, config.thick, config.red, config.green, config.blue, config.alpha);
				break;
			case 4:
				if(config.angle == 0){
					printf("\nAngle value is incorrect.\n");
					cleanup(&img, &config);
					return 0;
				}
				if((config.start_x == config.end_x) && (config.start_y == config.end_y)){
					printf("\nIncorrect coordinates.\n");
					cleanup(&img, &config);
					return 0;
				}
				if(((config.end_x - config.start_x) < 0) || ((config.end_y - config.start_y) < 0)){
					printf("\nIncorrect coordinates.\n");
					cleanup(&img, &config);
					return 0;
				}
				anormal = rotate(&img, config.start_x, config.start_y, config.end_x, config.end_y, config.angle);
				break;
			default:
				if(config.manipulation == 0 && flag == 1){
					break;
				}
				printf("\nInvalid manipulation input.\n");
				cleanup(&img, &config);
				return 0;
				break;
		}
	}
	//
	if(anormal && config.manipulation != 0)
		wfile(&img, config.result);
	if(config.input !=  NULL)
		free(config.input);
	if(config.result != NULL)
		free(config.result);
	return 0;
}