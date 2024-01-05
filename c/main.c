#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>

#define BUFFER_SIZE (1024 * 64)

typedef struct {
    size_t chars;
    size_t lines;
    size_t bytes;
    size_t words;
} stats;

void print_usage(const char* program_name) {
    printf("Usage: %s [options] [file]\n\
\nWC will count the number of new lines, bytes, chars (UTF-8) and words for the given input\n\
\nOptions:\n\
    -c   --chars    print the character counts\n\
    -l   --lines    print the newline counts\n\
    -w   --words    print the word counts\n\
    -b   --bytes    print the byte counts\n\
    -h   --help     print this help menu\n",
           program_name);
}

bool parse_arguments(int argc, char *argv[], bool *count_chars, bool *count_words, bool *count_lines, bool *count_bytes, char **filename) {
    *filename = NULL;
    bool any_option = false;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--chars") == 0) {
                *count_chars = true;
                any_option = true;
            } else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--words") == 0) {
                *count_words = true;
                any_option = true;
            } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--lines") == 0) {
                *count_lines = true;
                any_option = true;
            } else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bytes") == 0) {
                *count_bytes = true;
                any_option = true;
            } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                print_usage(argv[0]);
                return false;
            } else {
                fprintf(stderr, "Unknown option: %s\n", argv[i]);
                print_usage(argv[0]);
                return false;
            }
        } else {
            if (*filename == NULL) {
                *filename = argv[i];
            } else {
                fprintf(stderr, "Multiple files not supported.\n");
                return false;
            }
        }
    }

    if (!any_option) {
        *count_chars = *count_words = *count_lines = *count_bytes = true;
    }

    return true;
}


void count_stats(FILE *input, stats *wc, bool count_chars, bool count_words, bool count_lines, bool count_bytes) {
    char buffer[BUFFER_SIZE];
    int read_bytes = 0;
    bool in_word = false;

    while ((read_bytes = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
        for (int i = 0; i < read_bytes; i++) {
            if (count_lines && buffer[i] == '\n') wc->lines++;

            if (count_words) {
                if (isspace(buffer[i])) {
                    in_word = false;
                } else if (!in_word) {
                    in_word = true;
                    wc->words++;
                }
            }

            if (count_chars) {
                if ((buffer[i] & 0x80) == 0x00) {
                    wc->chars++;
                } else if ((buffer[i] & 0xe0) == 0xc0) {
                    wc->chars++;
                } else if ((buffer[i] & 0xf0) == 0xe0) {
                    wc->chars++;
                } else if ((buffer[i] & 0xf8) == 0xf0) {
                    wc->chars++;
                }
            }
        }

        if (count_bytes) wc->bytes += read_bytes;
    }
}

int main(int argc, char *argv[]) {
    stats wc = {0};
    bool count_chars, count_words, count_lines, count_bytes = false;
    char *filename = NULL;

    if (!parse_arguments(argc, argv, &count_chars, &count_words, &count_lines, &count_bytes, &filename)) {
        return 1;
    }

    FILE *input = (filename != NULL) ? fopen(filename, "rb") : stdin;
    if (input == NULL) {
        perror("Error opening file");
        return 1;
    }

    count_stats(input, &wc, count_chars, count_words, count_lines, count_bytes);

    if (filename != NULL) {
        fclose(input);
    }

    if (count_bytes) printf("Bytes: %zu\t", wc.bytes);
    if (count_lines) printf("Lines: %zu\t", wc.lines);
    if (count_words) printf("Words: %zu\t", wc.words);
    if (count_chars) printf("Chars: %zu\t", wc.chars);
    printf("\n");
    return 0;
}