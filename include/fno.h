/*
 * fileoperations.h
 *
 *  Created on: Feb 4, 2015
 *      Author: root
 */

#ifndef INCLUDE_FILENAMEOPERATIONS_H_
#define INCLUDE_FILENAMEOPERATIONS_H_

#include <string.h>
#include <stdlib.h>

int getLastIndex(const char *string, char c);
char* getPath(const char *qualifiedName);
char* getFileName(char *qualifiedName);
char* removeLastExtension(char *fileName);

//#include "FileNameOperations.c"
#endif /* INCLUDE_FILENAMEOPERATIONS_H_ */
