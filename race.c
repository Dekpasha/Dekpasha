#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>
  
void sorting(char mas[][2][11], unsigned left, unsigned right, unsigned let_num);
  
int main(int argc, char *argv[]){
    FILE *fread, *frite;
    fread = fopen("race.in", "r");
    frite = fopen("race.out", "w");
  
    unsigned n;
    fscanf(fread, "%u", &n);
    char mas[n][2][11];
    int letter;
  
    //Заполняем массив
    for (unsigned i = 0; i <= n - 1; i++)
        fscanf(fread, "%s%s", &mas[i][0], &mas[i][1]);
    //Запускаем цикл сортировки обращаясь к функции ---------------------------------------------------------
    sorting(mas, 0, n - 1, 0);
    //Заканчиваем цикл сортировки ---------------------------------------------------------------------------
    //Вывожу ответ
    for (unsigned i = 0; i <= n - 1; i++){
        puts(mas[i][0]);
        printf(" ");
        puts(mas[i][1]);
        printf("\n");
    }
    char *line = (char*)calloc(11, sizeof(char));
    strcpy(line, mas[0][0]);
    if (n == 1)
        fprintf(frite, "=== %s ===\n%s", line, mas[0][1]);
    unsigned i = 1;
    while (i <= n - 1){
        if ((!strcmp(line, mas[i][0])) && i < n - 1){
            fprintf(frite, "=== %s ===\n%s\n", line, mas[i - 1][1]);
            while ((!strcmp(line, mas[i][0])) && (i < n - 1)){
                fprintf(frite, "%s\n", mas[i][1]);
                strcpy(line, mas[i++][0]);
                }
            if (i < n - 1)
                strcpy(line, mas[i++][0]);
            else{
                if (!strcmp(line, mas[i][0]))
                    fprintf(frite, "%s", mas[i][1]);
                else
                    fprintf(frite, "=== %s ===\n%s", mas[i][0], mas[i][1]);
                break;
            }
        }
        else if (i < n - 1){
            fprintf(frite, "=== %s ===\n%s\n", line, mas[i - 1][1]);
            strcpy(line, mas[i++][0]);
        }
        else{
            if (!strcmp(line, mas[i][0]))
                fprintf(frite, "=== %s ===\n%s\n%s", line, mas[i - 1][1], mas[i][1]);
            else
                fprintf(frite, "=== %s ===\n%s\n=== %s ===\n%s", line, mas[i - 1][1], mas[i][0], mas[i][1]);
            break;
        }
    }
    //Освобождаю память
    free(line);
    fclose(fread);
    fclose(frite);
}
 
void sorting(char mas[][2][11], unsigned left, unsigned right, unsigned let_num){
    char *line1 = (char*)calloc(11, sizeof(char));
    char *line2 = (char*)calloc(11, sizeof(char));
    strcpy(line1, mas[left][0]);
    short pass = 0;
    for (unsigned i = left + 1; i <= right; i++){
        strcpy(line2, mas[i][0]);
        if (strcmp(line1, line2)){
            pass = 1;
            break;
        }
    }
    short check = 0;
    for (unsigned i = left; i <= right; i++){
        if ((('A' <= (int)mas[i][0][let_num]) && ((int)mas[i][0][let_num] <= 'Z')) || ('a' <= (int)mas[i][0][let_num]) && ((int)mas[i][0][let_num] <= 'z'))
            check = 1;
            break;
    }
    if (left != right && let_num <= 10 && check == 1 && pass == 1){
        unsigned n = right - left + 1;
        int letter;
        unsigned deep = 0;
        //Создаём и заполняем вспомагательные массивы нулей
        unsigned size = 'z' - 'A' - 5;
        int *chars = (int*)calloc(size, sizeof(int));
        for (short i = 0; i <= size - 1; i++)
           chars[i] = 0;
        char ***sort_mas;
        sort_mas = (char***)calloc(n, sizeof(char**));
        for (int i = 0; i <= n - 1; i++){
            sort_mas[i] = (char**)calloc(2, sizeof(char*));
            for (int k = 0; k <= 1; k++)
                sort_mas[i][k] = (char*)calloc(11, sizeof(char));
        }
        //Считаем количество определённых букв
        for (unsigned i = left; i <= right; i++){
            if (('A' <= (int)mas[i][0][let_num]) && ((int)mas[i][0][let_num] <= 'Z')){
                letter = mas[i][0][let_num] - 'A';
                chars[letter]++;
            }
            else if (('a' <= (int)mas[i][0][let_num]) && ((int)mas[i][0][let_num] <= 'z')){
                letter = mas[i][0][let_num] - 'a' + 26;
                chars[letter]++;
            }
            else
                deep++;
        }
        //Складываем все предыдущие значения, чтобы получить нужный индекс
        for (int i = 1; i <= size - 1; i++)
            chars[i] += chars[i - 1];
        //Заполняем sort_mas с помощью chars беря элементы из mas
        unsigned count_deep = 0;
        unsigned step;
        int sign;
        for (int p = 0; p <= n - 1; p++){
            step = right - p;
            sign = mas[step][0][let_num];
            if (('A' <= sign) && (sign <= 'Z')){
                letter = sign - 'A';
                strcpy(sort_mas[chars[letter] - 1 + deep][0], mas[step][0]);
                strcpy(sort_mas[chars[letter] - 1 + deep][1], mas[step][1]);
                chars[letter]--;
            }
            else if(('a' <= sign) && (sign <= 'z')){
                letter = sign - 'a' + 26;
                strcpy(sort_mas[chars[letter] - 1 + deep][0], mas[step][0]);
                strcpy(sort_mas[chars[letter] - 1 + deep][1], mas[step][1]);
                chars[letter]--;
            }
            else{
                strcpy(sort_mas[0 + count_deep][0], mas[step][0]);
                strcpy(sort_mas[0 + count_deep][1], mas[step][1]);
                count_deep++;
            }
        }
        //Копируем массив sort_mas в mas
        unsigned m = 0;
        for (unsigned start = left; start <= right; start++){
            strcpy(mas[start][0], sort_mas[m][0]);
            strcpy(mas[start][1], sort_mas[m][1]);
            m++;
        }
     //Освобождаем память
        free(**sort_mas);
        free(*sort_mas);
        free(sort_mas);
        free(chars);
        free(line2);
        free(line1);
        //Вызываем рекурсию
        unsigned count = 1;
        letter = (mas[left][0][let_num]) ;
        for (unsigned i = left + 1 + deep; i <= right; i++){
            if (((int)(mas[i][0][let_num])) == letter)
                count++;
            else{
                sorting(mas, i - count, i - 1, let_num + 1);
                letter = (mas[i][0][let_num]);
                count = 1;
            }
        }
        sorting(mas, right - count + 1, right, let_num + 1);
        count = 1;
    }
}