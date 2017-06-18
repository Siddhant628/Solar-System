#pragma once

#include "Game.h"
#include "RenderStateHelper.h"
#include <windows.h>
#include <functional>

namespace Library
{
	class KeyboardComponent;
	class MouseComponent;
	class FpsComponent;
	class Camera;
}

namespace Rendering
{
	class AstronomicalObject;

	class RenderingGame final : public Library::Game
	{
	public:
		RenderingGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;
		virtual void Shutdown() override;

		void Exit();

	private:
		static const DirectX::XMVECTORF32 BackgroundColor;

		Library::RenderStateHelper mRenderStateHelper;
		std::shared_ptr<Library::KeyboardComponent> mKeyboard;
		std::shared_ptr<Library::MouseComponent> mMouse;
		std::shared_ptr<Library::FpsComponent> mFpsComponent;
		std::shared_ptr<Library::Camera> mCamera;
		
		/**
		* Astronomical objects cooresponding to those in the solar system.
		*/
		std::shared_ptr<AstronomicalObject> mSun;
		std::shared_ptr<AstronomicalObject> mMercury;
		std::shared_ptr<AstronomicalObject> mVenus;
		std::shared_ptr<AstronomicalObject> mEarth;
		std::shared_ptr<AstronomicalObject> mMoon;
		std::shared_ptr<AstronomicalObject> mMars;
		std::shared_ptr<AstronomicalObject> mJupiter;
		std::shared_ptr<AstronomicalObject> mSaturn;
		std::shared_ptr<AstronomicalObject> mUranus;
		std::shared_ptr<AstronomicalObject> mNeptune;
		std::shared_ptr<AstronomicalObject> mPluto;

	public:
		/**
		* The default rotation for the camera to face the solar system.
		*/
		static const float sCameraRotation;
		/**
		* The default position for the camera to face the solar system.
		*/
		static const DirectX::XMFLOAT3 sCameraPosition;
	};
}