#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
void special_remove(struct Text *text){
	int check = 1;
	wchar_t triggers[] = L"йЙцЦкКнНгГшШщЩзЗхХбБвВдДжЖлЛмМпПрРсСтТфФчЧbBcCdDfFgGhHjJkKlLmMnNpPqQrRsStTvVwWxXzZ";
	for(int i = 0; i < (text->am_sen); i++){
		if(text->sen_arr[i].lenth >= 4){
			for (int j = 0; j < (text->sen_arr[i].lenth); j++)
			{
				if(text->sen_arr[i].str[j] == L'.'){
					for(int n = 0; ((n < 83) && (check < 4)); n++){
						if((text->sen_arr[i].str[j - check]) == triggers[n]){
							check++;
							n = 0;
						}
					}

					if(check == 4){
						text->am_sen--;
						for(int k = i; k < text->am_sen; k++)
							text->sen_arr[k] = text->sen_arr[k + 1];
						i--;
					}
					check = 1;
				}	
			}
		}
	}
}
int compare_func(const void *a, const void *b){
	struct Sentence* sen1 = (struct Sentence*)a;
	struct Sentence* sen2 = (struct Sentence*)b;
	return (sen1->fw_sum - sen2->fw_sum);
}



void time(struct Text *text){
	wchar_t* p;
	int ind = 0, k = 2, q = 1;
	int sec = 0;
	for(int i = 0; i < text->am_sen; i++){
	ind = 0;
	while ((p = wcsstr(text->sen_arr[i].str + ind + wcslen(L" sec "), L" sec ")) != NULL){
		ind = p - text->sen_arr[i].str;

			if(iswdigit(text->sen_arr[i].str[ind -1])){
				while(iswdigit(text->sen_arr[i].str[ind - k]))
					k++;
			}
			if (text->sen_arr[i].str[ind - k] != L' '){
				continue;
			}
			q = 1;
			for(int s = 0; s < k - 1; s++){
				sec += ((int)text->sen_arr[i].str[ind - 1 - s] - (int)L'0') * q;
				q = q * 10;
			}
		}
	}
	wprintf(L"В тексте %f минут\n", sec / 60.0);
}

void built_in_filter(struct Text *text){
	for(int i = 0; i < text->am_sen-1; i++){
		for(int j = i + 1; j < text->am_sen; j++){
			if(!wcscasecmp(text->sen_arr[i].str, text->sen_arr[j].str)){
				text->am_sen--;
				for(int k = j; k < text->am_sen; k++)
					text->sen_arr[k] = text->sen_arr[k + 1];
				j--;
			}
		}
	}
}

void spec_symbols_replace(struct Text *text){ 
	wchar_t* at = L"(at)";
	wchar_t* hash = L"<решетка>";
	wchar_t* percent = L"<percent>";

	for(int i = 0; i < text->am_sen; i++){
		for(int j = 0; j < text->sen_arr[i].lenth; j++){
			
			if(text->sen_arr[i].str[j] == L'@'){
				text->sen_arr = (struct Sentence *)realloc(text->sen_arr, sizeof(struct Sentence) * (text->am_sen +1));
				text->sen_arr[i].str = (wchar_t*)realloc(text->sen_arr[i].str, wcslen(text->sen_arr[i].str) * 2 *sizeof(wchar_t));
				for(int s = text->sen_arr[i].lenth; s != j;s--)
					text->sen_arr[i].str[s + 3] = text->sen_arr[i].str[s];
				for(int n = j;(n-j) < 4;n++)
					text->sen_arr[i].str[n] = at[n-j];
				text->sen_arr[i].lenth += 3;
			}

			if((text->sen_arr[i].str[j] == L'#')||(text->sen_arr[i].str[j] == L'%')){
				text->sen_arr = (struct Sentence *)realloc(text->sen_arr, sizeof(struct Sentence) * (text->am_sen +1));
				text->sen_arr[i].str = (wchar_t*)realloc(text->sen_arr[i].str, wcslen(text->sen_arr[i].str) * 2 *sizeof(wchar_t));
				for(int s = text->sen_arr[i].lenth; s != j;s--)
					text->sen_arr[i].str[s + 8] = text->sen_arr[i].str[s];
				
				if(text->sen_arr[i].str[j] == L'#'){
					for(int n = j;(n-j) < 9;n++)
						text->sen_arr[i].str[n] = hash[n-j];
				}
				else{
					for(int n = j;(n-j) < 9;n++)
						text->sen_arr[i].str[n] = percent[n-j];
				}
				
				text->sen_arr[i].lenth += 8;
			}
		}
	}
}
