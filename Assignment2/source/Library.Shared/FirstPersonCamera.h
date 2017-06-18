#pragma once

#include "PerspectiveCamera.h"
#include "GamePadComponent.h"

namespace Library
{	
	class KeyboardComponent;
	class MouseComponent;

    class FirstPersonCamera : public PerspectiveCamera
    {
		RTTI_DECLARATIONS(FirstPersonCamera, PerspectiveCamera)

    public:
        explicit FirstPersonCamera(Game& game);
        FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);
		FirstPersonCamera(const FirstPersonCamera&) = delete;
		FirstPersonCamera& operator=(const FirstPersonCamera&) = delete;
		FirstPersonCamera(FirstPersonCamera&&) = delete;
		FirstPersonCamera& operator=(FirstPersonCamera&&) = delete;
		virtual ~FirstPersonCamera() = default;

		GamePadComponent* GetGamePad() const;
		void SetGamePad(GamePadComponent* gamePad);

		KeyboardComponent* GetKeyboard() const;
		void SetKeyboard(KeyboardComponent* keyboard);

		MouseComponent* GetMouse() const;
		void SetMouse(MouseComponent* mouse);

		float& MouseSensitivity();
        float& RotationRate();
        float& MovementRate();
        
		virtual void Initialize() override;
        virtual void Update(const GameTime& gameTime) override;

		static const float DefaultMouseSensitivity;
        static const float DefaultRotationRate;
        static const float DefaultMovementRate;

	private:
		void UpdatePosition(const DirectX::XMFLOAT2& movementAmount, const DirectX::XMFLOAT2& rotationAmount, const GameTime& gameTime);
		inline bool IsGamePadConnected(DirectX::GamePad::State& gamePadState)
		{
			if (mGamePad != nullptr)
			{
				gamePadState = mGamePad->CurrentState();
				return gamePadState.IsConnected();
			}

			return false;
		}

    protected:
		GamePadComponent* mGamePad;
		KeyboardComponent* mKeyboard;
		MouseComponent* mMouse;
		float mMouseSensitivity;
		float mRotationRate;
        float mMovementRate;		
    };
}

