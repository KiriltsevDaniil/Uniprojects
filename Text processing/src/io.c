#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include "structs.h"

void output(struct Text *text){
		wprintf(L"##############################Text##################################\n\n");
		for(int i = 0; i < text->am_sen; i++)
		wprintf(L"%ls\n", text->sen_arr[i].str);
		wprintf(L"\n##################################################################\n\n");
}

void input(struct Text *text){
	int ft_size = 20, str_size = 20, k = 0, first_word = 1, code_sum = 0;
	wchar_t wsymbol;
	text->am_sen = 0;
	text->sen_arr = (struct Sentence*)malloc(ft_size * sizeof(struct Sentence));
	text->sen_arr[text->am_sen].lenth = 0;
	text->sen_arr[text->am_sen].str = (wchar_t*)malloc(str_size * sizeof(wchar_t));
	while((wsymbol = getwchar()) != L'\n'){
		text->sen_arr[text->am_sen].str[k++] = wsymbol;
		if(k >= str_size){
			text->sen_arr[text->am_sen].str = (wchar_t*)realloc(text->sen_arr[text->am_sen].str, 2 * str_size * sizeof(wchar_t));
			str_size *= 2;
		}

		if((first_word == 1) && (wsymbol != L' ') && (wsymbol != L'.') && (wsymbol != L','))
			code_sum += (int)wsymbol;
		if((first_word == 1) && ((wsymbol == L' ')||(wsymbol == L','))){
			text->sen_arr[text->am_sen].fw_sum = code_sum;
			first_word = 0;
		}

		if(wsymbol == L'.'){
			if(first_word == 1){
				text->sen_arr[text->am_sen].fw_sum = code_sum;
				first_word = 0;
			}
			text->sen_arr[text->am_sen].lenth = k;
			k = 0;
			if(text->am_sen + 8 >= ft_size){
				text->sen_arr = (struct Sentence *)realloc(text->sen_arr, ft_size * 2 * sizeof(struct Sentence *));
				ft_size *= 2;
			}
			str_size = 20;
			text->am_sen++;
			text->sen_arr[text->am_sen].str = (wchar_t*)calloc(str_size, sizeof(wchar_t));
			first_word = 1;
			code_sum = 0;
		}
	}
}
