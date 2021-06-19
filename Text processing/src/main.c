#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h> 
#include <ctype.h>
#include "structs.h"
#include "io.h"
#include "render.h"

void menu_graphics(){
	wprintf(L"\nВведите команду для её выполнения:\n\n");
	wprintf(L"1. Посчитать и вывести в минутах количество секунд в тексте.Количество секунд задается в виде подстроки '<число> sec'.\n");
	wprintf(L"2. Отсортировать предложения по увеличению сумме кодов символов первого слова в предложении.\n");
	wprintf(L"3. Заменить все символы ‘%%’, ‘#’, ‘@’ на “<percent>”, “<решетка>”, “(at)” соответственно.\n");
	wprintf(L"4. Удалить все предложения которые заканчиваются на слово с тремя согласными подряд.\n");
	wprintf(L"5. Вывести текст\n");
	wprintf(L"6. Выход из программы.\n");
	wprintf(L"h. Вывести подсказку еще раз.\n");
}

void menu(struct Text *text){
	wchar_t comm = L'0';
	menu_graphics();
	while(comm != L'6'){
		{
			comm = getwchar();
			switch (comm){
				case '1' : { time(text); break;}
				case '2' : { qsort(text->sen_arr, text->am_sen, sizeof(struct Sentence), compare_func); break;}
				case '3' : { spec_symbols_replace(text); break;}
				case '4' : { special_remove(text); break;}
				case '5' : {output(text); break;}
				case '6' : { wprintf(L"Работа программы завершена.\n"); break;}
				default : {if(comm != L'\n') menu_graphics();}
			}
		}
	}
}

int main(){
	setlocale(LC_ALL, "");
	struct Text text;
	input(&text);
	built_in_filter(&text);
	menu(&text);
	for(int i = 0; i < text.am_sen; i++)
		free(text.sen_arr[i].str);
	free(text.sen_arr);
	return 0;
}