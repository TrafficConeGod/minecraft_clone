#pragma once
#include <stdio.h>
#include <sys/stat.h>
#include <bits/types/error_t.h>

error_t open_files(size_t num_files, const char* const paths[], const char* const modes[], struct stat stats[], FILE* files[]);
void close_files(size_t num_files, FILE* const files[]);