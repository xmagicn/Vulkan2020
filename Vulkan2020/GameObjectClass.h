#pragma once

class Model;
class VulkanTexture;

class GameObject
{
public:
	GameObject();
	GameObject( const GameObject& ) = default;
	GameObject& operator=( const GameObject& ) = default;
	~GameObject();

	void InitDraw( const char* model, const char* texture );
	void Update();
	void Cleanup();

private:
	Model* pModel;
	VulkanTexture* pTexture;
};