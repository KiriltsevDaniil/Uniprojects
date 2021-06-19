#include <wchar.h>

struct Sentence{
	int lenth;
	int fw_sum;
	wchar_t *str;
};

struct Text{
	int am_sen;
	struct Sentence *sen_arr;
};