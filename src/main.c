#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "include/help.h"
#include "include/version.h"

#define MAX_WORDS 4096
#define MAX_WORD_LEN 128

/* Функция для проверки, является ли символ допустимым для слова */
int is_word_char(char c) {
        return isalnum((unsigned char)c) || c == '-' || c == '_';
}

/* Функция для обработки строки и извлечения слов */
int process_string(const char *input, char words[][MAX_WORD_LEN], int max_words) {
        int word_count = 0;
        int in_word = 0;
        int word_pos = 0;

        for (int i = 0; input[i] != '\0'; i++) {
                char c = input[i];

                if (is_word_char(c)) {
                        if (!in_word) {
                                in_word = 1;
                                word_pos = 0;
                        }
                        if (word_pos < MAX_WORD_LEN - 1) {
                                words[word_count][word_pos++] = c;
                        }
                } else {
                        if (in_word) {
                                words[word_count][word_pos] = '\0';
                                word_count++;
                                in_word = 0;
                                if (word_count >= max_words) break;
                        }
                }
        }

        /* Добавляем последнее слово, если мы все еще в слове */
        if (in_word && word_count < max_words) {
                words[word_count][word_pos] = '\0';
                word_count++;
        }

        return word_count;
}

void version()
{
        puts(VERSION_MESSAGE);
}

void help()
{
        printf(HELP_MESSAGE);
}

int main(int argc, char *argv[])
{
        char words[MAX_WORDS][MAX_WORD_LEN];
        int word_count = 0;
        int use_spaces = 1;
        int word_limit = -1;

        int opt;
        int preview = 0;


        while ((opt = getopt(argc, argv, "c:svh")) != -1) {
                switch (opt) {
                        case 'v':
                                version();
                                preview = 1;
                                break;
                        case 'h':
                                help();
                                preview = 1;
                                break;
                        case 'c':
                                word_limit = atoi(optarg);
                                if (word_limit <= 0) {
                                        fprintf(stderr, "Error: count of words must be a above zero!\n");
                                        return 1;
                                }
				break;
                        case 's':
                                use_spaces = 0;
                                break;
                        default:
                                word_limit = MAX_WORDS;
                                use_spaces = 1;
                                break;
                }
        }

        if (!preview) {
                /* Чтение из стандартного ввода */
                char buffer[4096];
                size_t total_size = 0;
                char *input = malloc(1);
                input[0] = '\0';

                if (!input) {
                        fprintf(stderr, "Memory error\n");
                        return 1;
                }

                while (fgets(buffer, sizeof(buffer), stdin)) {
                        size_t buffer_len = strlen(buffer);
                        char *new_input = realloc(input, total_size + buffer_len + 1);
                        if (!new_input) {
                                free(input);
                                fprintf(stderr, "Memory error\n");
                                return 1;
                        }
                        input = new_input;
                        strcpy(input + total_size, buffer);
                        total_size += buffer_len;
                }

                /* Обработка ввода и извлечение слов */
                word_count = process_string(input, words, MAX_WORDS);
                free(input);

                /* Определение количества слов для вывода */
                int words_to_print = word_count;
                if (word_limit > 0 && word_limit < word_count) {
                        words_to_print = word_limit;
                }

                /* Вывод результата */
                for (int i = 0; i < words_to_print; i++) {
                        printf("%s", words[i]);
                        if (use_spaces && i < words_to_print - 1) {
                                printf(" ");
                        }
                }
                printf("\n");
        }
        else
                return 0;

        return 0;
}
