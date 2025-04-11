#include "precomp.h"
#include "Files.h"

#include <iostream>

FILE* Files::OpenForRead(char const* path)
{
	FILE* file = fopen(path, "r"); 
	if (!file) FatalError("Couldn't open file: %s", path); 
	return file; 
}

int* Files::ParseCSVToInt(char* csv, uint const amount)
{
	int* values = new int[amount];
	char* token = strtok(csv, ",");
	for (int i = 0; i < amount; i++)
	{
		values[i] = atoi(token);
		token = strtok(NULL, ","); 
	}
	return values; 
}

void Files::ParseCSVToInt(char* csv, int* values, uint const amount) 
{
	char* token = strtok(csv, ","); 
	for (int i = 0; i < amount; i++)
	{
		values[i] = atoi(token);
		token = strtok(NULL, ",");
	}
}

char* Files::GetLine(FILE* file, uint const maxLength)
{
	char* line = new char[maxLength]; 
	fgets(line, maxLength, file); 
	return line; 
}

void Files::GetLine(FILE* file, char* line, uint const maxLength)  
{
	fgets(line, maxLength, file);
}

FILE* Files::StartLoading(char const* path)
{
	cout << "[LOADING]:\t" << path << endl; 
	return OpenForRead(path);  
}

void Files::PrintFailedToLoad(char const* path)
{ 
	cout << "[FAILED]:\t" << path << endl;
}

void Files::PrintLoadedSuccesfully(char const* path) 
{
	cout << "[SUCCES]:\t" << path << endl; 
}

FILE* Files::ReadFile(char const* path) 
{
	FILE* file = fopen(path, "r"); 
	if (!file) FatalError("Couldn't open file %s", path); 
	return file; 
}

FILE* Files::WriteFile(char const* path)
{
	FILE* file = fopen(path, "w"); 
	if (!file) FatalError("Couldn't open file %s", path); 
	return file; 
}

void Files::PrintLoading(char const* path)
{
	printf("[LOADING]\t%s\n", path); 
}

void Files::PrintSuccess(char const* path)
{
	printf("[SUCCESS]\t%s\n", path);
}

void Files::PrintFailure(char const* path) 
{
	printf("[FAILURE]\t%s\n", path);
}

void Files::PrintSaving(char const* path)  
{
	printf("[SAVING]\t%s\n", path);  
}

char const* Files::FindDirectory(char const* path) 
{
	char* result = nullptr;  
	char const* lastSlash = strrchr(path, '/');
	if (lastSlash)
	{
		size_t len = lastSlash - path + 1;
		result = new char[len]; 
		strncpy(result, path, len);  
		result[len] = '\0'; // null terminating character 
	}
	return result; 
} 
