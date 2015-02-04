/*
 * FileNameOperations.c
 *
 *  Created on: Feb 4, 2015
 *      Author: root
 */

#include "filenameoperations.h"


int getLastIndex(const char *string, char c)
{
    int i;
    for(i=strlen(string) ; (string[i]!=c)&& (i != 0) ; i--);
    return i;
}

char* getPath(const char *qualifiedName)
{
    static char *path;
    int pathLength = getLastIndex(qualifiedName,'/')+1;
    path = malloc(pathLength);
    strncpy(path, qualifiedName, pathLength-1);
    path[pathLength-1]='\0';
    return path;
}

char* getFileName(char *qualifiedName)
{
    static char *fileName;
    int fileNameLength;
	fileNameLength = strlen(qualifiedName);
	fileNameLength -= getLastIndex(qualifiedName,'/');
	fileNameLength -= 1;
    fileName = malloc(fileNameLength+1);
    strcpy(fileName,(qualifiedName+getLastIndex(qualifiedName,'/')+1));
    return fileName;
}

char* removeLastExtension(char *fileName)
{
    static char *string;
    int fileNameLength = getLastIndex(fileName,'.');
    string = malloc(fileNameLength+1);
    strncpy(string,fileName,fileNameLength);
    string[fileNameLength]='\0';
    return string;
}
