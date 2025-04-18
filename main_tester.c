/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_tester.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mipinhei <mipinhei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:47:48 by mipinhei          #+#    #+#             */
/*   Updated: 2025/04/18 16:54:00 by mipinhei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "get_next_line.h"

void run_test(const char *filename, const char *description)
{
    printf("\n\033[1;34m=== Testing: %s (%s) ===\033[0m\n", filename, description);
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        printf("\033[1;31mError opening file!\033[0m\n");
        return;
    }

    char *line;
    int line_count = 0;
    while ((line = get_next_line(fd))) 
    {
        printf("%3d: [%s]\n", ++line_count, line);
        free(line);
    }

    close(fd);
    printf("Total lines: %d\n", line_count);
}

void test_stdin()
{
    printf("\n\033[1;34m=== Testing STDIN (type input, press Ctrl+D to end) ===\033[0m\n");
    char *line;
    int line_count = 0;
    
    while ((line = get_next_line(STDIN_FILENO))) 
    {
        printf("%3d: [%s]\n", ++line_count, line);
        free(line);
    }
    
    printf("Total lines from stdin: %d\n", line_count);
}

void test_invalid_fd()
{
    printf("\n\033[1;34m=== Testing Invalid File Descriptor ===\033[0m\n");
    char *line = get_next_line(-1);
    if (line == NULL)
        printf("Correctly returned NULL for invalid FD\n");
    else
    {
        printf("ERROR: Got non-NULL for invalid FD!\n");
        free(line);
    }
}

void test_empty_file()
{
    printf("\n\033[1;34m=== Testing Empty File ===\033[0m\n");
    system("touch empty_test_file");
    run_test("empty_test_file", "empty file");
    system("rm empty_test_file");
}

void test_binary_file()
{
    printf("\n\033[1;34m=== Testing Binary File ===\033[0m\n");
    system("head -c 100 /dev/urandom > binary_test_file");
    run_test("binary_test_file", "binary data");
    system("rm binary_test_file");
}
// Compile with: gcc -Wall -Wextra -Werror -g -fsanitize=address get_next_line.c get_next_line_utils.c main_tester.c -o gnl_test
int main(void)
{
    // Create test files
    system("echo -e 'Line 1\\nLine 2\\nLine 3' > normal.txt");
    system("echo -e 'Line 1\\n\\nLine 3' > empty_lines.txt");
    system("echo -n 'No newline at end' > no_newline.txt");
    system("echo -e 'Very\\nlong\\nline\\nwith\\nmultiple\\nnewlines' > long_lines.txt");

    // Run tests
    run_test("normal.txt", "normal file with 3 lines");
    run_test("empty_lines.txt", "file with empty line");
    run_test("no_newline.txt", "file without ending newline");
    run_test("long_lines.txt", "file with multiple lines");
    test_empty_file();
    test_binary_file();
    test_invalid_fd();
    test_stdin();

    // Cleanup
    system("rm normal.txt empty_lines.txt no_newline.txt long_lines.txt");

    // Check for memory leaks (run with valgrind or fsanitize)
    printf("\n\033[1;33mDon't forget to run with:\n");
    printf("valgrind --leak-check=full --show-leak-kinds=all ./a.out\n");
    printf("or compile with -fsanitize=address\033[0m\n");

    return 0;
}
