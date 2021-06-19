
struct Configs{
	int filled;
	int red;
	int green;
	int blue;
	int alpha;
	int thick;
	int angle;
	int option;
	int start_x;
	int start_y;
	int end_x;
	int end_y;
	int x_partition;
	int y_partition;
	int manipulation;
	int d_red;
	int d_green;
	int d_blue;
	int d_alpha;
	char* input;
	char* result;
};


struct Img{
	int width, height;
	png_byte color_type;
	png_byte bit_depth;

	png_structp png_ptr;
	png_infop info_ptr;
	int nop;
	png_bytep *row_pts;
};