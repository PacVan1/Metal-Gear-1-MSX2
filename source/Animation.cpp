#include "precomp.h"
#include "Animation.h"

#include "Files.h" 

Animation::Animation(uint frameCount, uint startFrame, bool hasDuration) :
	startFrame(startFrame), 
	finalFrame(startFrame + frameCount - 1), 
	hasDuration(hasDuration) 
{}

AnimationSet::AnimationSet(char const* path) : 
	count(0), 
	anims(nullptr) 
{
	Files::PrintLoading(path);
	FILE* file = Files::ReadFile(path);  

	if (!fscanf(file, "AnimCount: %d\n", &count))
	{
		Files::PrintFailure(path);
		return; 
	}

	uint* frames = new uint[count];  
	if (!fscanf(file, "Frames:"));
	for (int i = 0; i < count; i++) 
	{
		if (!fscanf(file, "%d", &frames[i]))
		{
			Files::PrintFailure(path); 
			delete[] frames;  
			return;
		} 
	}
	fscanf(file, "\n");

	uint* durations	= new uint[count];    
	if (!fscanf(file, "Durations:")); 
	for (int i = 0; i < count; i++) 
	{
		if (!fscanf(file, "%d", &durations[i])) 
		{
			Files::PrintFailure(path); 
			delete[] frames; 
			delete[] durations; 
			return; 
		}
	}
	fscanf(file, "\n");

	anims = new Animation[count];    

	uint startFrame = 0; 
	for (int i = 0; i < count; i++)
	{
		anims[i] = Animation(frames[i], startFrame, (durations[i] != 0)); 
		startFrame += frames[i]; 
	} 

	fclose(file); 
	delete[] frames;    
	delete[] durations;   
	Files::PrintSuccess(path);  
}
