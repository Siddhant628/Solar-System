#include "pch.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(DrawableGameComponent)

	DrawableGameComponent::DrawableGameComponent() :
		GameComponent(), mVisible(true)
	{
	}

	DrawableGameComponent::DrawableGameComponent(Game& game) :
		GameComponent(game), mVisible(true)
	{
	}

	DrawableGameComponent::DrawableGameComponent(Game& game, const shared_ptr<Camera>& camera) :
		GameComponent(game), mVisible(true), mCamera(camera)
	{
	}

	bool DrawableGameComponent::Visible() const
	{
		return mVisible;
	}

	void DrawableGameComponent::SetVisible(bool visible)
	{
		mVisible = visible;
	}

	shared_ptr<Camera> DrawableGameComponent::GetCamera()
	{
		return mCamera;
	}

	void DrawableGameComponent::SetCamera(const std::shared_ptr<Camera>& camera)
	{
		mCamera = camera;
	}

	void DrawableGameComponent::Draw(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);
	}
}