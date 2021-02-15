#pragma once
#include "Post/PostEffect.h"

//aka the PP wrapper
class FrameEffects
{
public:
	FrameEffects();

	//for all effects (put static inits here)
	static void Init();
	//same as the init, but for unloading
	static void Unload();

	//change the buffers
	void Init(unsigned width, unsigned height);
	void Resize(unsigned width, unsigned height);

	//in order of added
	void AddEffect(PostEffect* effect);
	void RemoveEffect(int slot);

	//binds everything needed
	void Bind();
	void UnBind();

	//for drawing purposes
	void Clear();
	void Draw();

	//Get amount of effects
	size_t size() { return layersOfEffects.size(); }

	//get the effect at the index, and nullptr if invalid
	PostEffect* operator[](size_t index) {
		if (layersOfEffects.size() <= index)	return nullptr;
		return layersOfEffects[index];
	}

private:
	PostEffect baseEffect;
	std::vector<PostEffect*> layersOfEffects = {};
};
