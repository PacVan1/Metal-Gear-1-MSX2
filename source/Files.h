#pragma once

class Files
{
public:
	static FILE*	OpenForRead(char const* path);    
	static int*		ParseCSVToInt(char* csv, uint const amount);
	static void		ParseCSVToInt(char* csv, int* values, uint const amount);  
	static char*	GetLine(FILE* file, uint const maxLength);
	static void		GetLine(FILE* file, char* line, uint const maxLength); 

	// TODO: when done with project. Remove all iostream statements:
	static FILE*	StartLoading(char const* path);  
	static void		PrintFailedToLoad(char const* path); 
	static void		PrintLoadedSuccesfully(char const* path);   

	static FILE* ReadFile(char const* path); 
	static FILE* WriteFile(char const* path);  

	static void PrintLoading(char const* path);   
	static void PrintSuccess(char const* path);    
	static void PrintFailure(char const* path);  
	static void PrintSaving(char const* path); 

	static char const* FindDirectory(char const* path); 
};