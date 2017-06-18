#pragma once

#include "DrawableGameComponent.h"
#include "RenderStateHelper.h"
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <unordered_map>

namespace Library
{
	class Mesh;
	class KeyboardComponent;
}

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Rendering
{
	/**
	* An enum with all astronomical object names.
	*/
	enum class AstronomicalObjectName
	{
		Invalid,
		Sun,
		Mercury,
		Venus,
		Earth,
		Moon,
		Mars,
		Jupiter,
		Saturn,
		Uranus,
		Neptune,
		Pluto
	};

	/**
	* Data associated with each astronomical object.
	*/
	struct AstronomicalObjectData
	{
		float AmbientIntensity;
		float AxialTilt;
		float RotationDays;
		float RevolutionDays;
		float OrbitalDistance;
		float Scale;
		std::wstring TextureName;
	};

	/**
	* A class for drawing astronomical objects such as planets and their moons and the Sun.
	*/
	class AstronomicalObject final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(AstronomicalObject, Library::DrawableGameComponent)

	public:
		AstronomicalObject(Library::Game& game, const std::shared_ptr<Library::Camera>& camera, AstronomicalObjectName name);
		~AstronomicalObject();

		bool AnimationEnabled() const;
		void SetAnimationEnabled(bool enabled);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

		/**
		* Get the point light which is illumiating this astronomical object.
		* @return A reference to the point light illuminating this astronomical object.
		*/
		const Library::PointLight& GetLight() const;
		/**
		* Set the point light which is illumiating this astronomical object.
		* @param pointLight A reference to the point light.
		*/
		void SetLight(const Library::PointLight& pointLight);
		/**
		* Get the parent astronomical object for this astronomical object.
		* @return A reference to a parent astronomical object.
		*/
		const AstronomicalObject& GetParentObject() const;
		/**
		* Set the parent astronomical object for this astronomical object.
		* @param parent A reference to a parent object to set.
		*/
		void SetParentObject(const AstronomicalObject& parent);

	private:
		/**
		* Update the degrees of the rotation and revolution of this astronomical object.
		* @param gameTime A game time object with data about time elapsed.
		*/
		void UpdateAstronomicalObjectRotation(const Library::GameTime& gameTime);
		void CreateVertexBuffer(const Library::Mesh& mesh, ID3D11Buffer** vertexBuffer) const;
		void ToggleAnimation();
		struct VSCBufferPerFrame
		{
			DirectX::XMFLOAT3 LightPosition;
			float LightRadius;

			VSCBufferPerFrame() :
				LightPosition(Library::Vector3Helper::Zero), LightRadius(50.0f) { }
			VSCBufferPerFrame(const DirectX::XMFLOAT3 lightPosition, float lightRadius) :
				LightPosition(lightPosition), LightRadius(lightRadius) { }
		};

		struct VSCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;
			DirectX::XMFLOAT4X4 World;

			VSCBufferPerObject() = default;
			VSCBufferPerObject(const DirectX::XMFLOAT4X4& wvp, const DirectX::XMFLOAT4X4& world) :
				WorldViewProjection(wvp), World(world) { }
		};

		struct PSCBufferPerFrame
		{
			DirectX::XMFLOAT3 AmbientColor;
			float Padding;
			DirectX::XMFLOAT3 LightPosition;
			float Padding2;
			DirectX::XMFLOAT3 LightColor;
			float Padding3;

			PSCBufferPerFrame() :
				AmbientColor(Library::Vector3Helper::Zero), LightPosition(Library::Vector3Helper::Zero), LightColor(Library::Vector3Helper::Zero)
			{
			}

			PSCBufferPerFrame(const DirectX::XMFLOAT3& ambientColor, const DirectX::XMFLOAT3& lightPosition, const DirectX::XMFLOAT3& lightColor) :
				AmbientColor(ambientColor), LightPosition(lightPosition), LightColor(lightColor)
			{
			}
		};

		PSCBufferPerFrame mPSCBufferPerFrameData;
		DirectX::XMFLOAT4X4 mWorldMatrix;
		VSCBufferPerFrame mVSCBufferPerFrameData;
		VSCBufferPerObject mVSCBufferPerObjectData;
		Library::RenderStateHelper mRenderStateHelper;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVSCBufferPerFrame;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVSCBufferPerObject;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mPSCBufferPerFrame;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mColorTexture;
		Library::KeyboardComponent* mKeyboard;
		std::uint32_t mIndexCount;
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::SpriteFont> mSpriteFont;
		DirectX::XMFLOAT2 mTextPosition;
		bool mAnimationEnabled;

		/**
		* The name of this astronomical object.
		*/
		AstronomicalObjectName mAstronomicalObjectName;
		/**
		* The current rotation of this astronomical object about its own Y-axis.
		*/
		float mCurrentRotationDegrees;
		/**
		* The rate at which this astronomical object rotates (degrees to rotate each second).
		*/
		float mRotationRate;
		/**
		* The current revolution of this astronomical object.
		*/
		float mCurrentRevolutionDegrees;
		/**
		* The rate at wgucg tgus astronomical object rotates (degrees to revolve each second).
		*/
		float mRevolutionRate;
		/**
		* A pointer to the point light illuminating this astronomical object.
		*/
		const Library::PointLight *mPointLight;
		/**
		* A pointer to the parent astronomical obejct. This object revolves around the parent (for example, the moon).
		*/
		const AstronomicalObject* mParent;
	public:
		/**
		* A map of astronomical object names as keys to access their corresponding properties data.
		*/
		static const std::unordered_map<AstronomicalObjectName, AstronomicalObjectData> sAstronomicalObjects;
		/**
		* The default position at which the light is to be positioned on start.
		*/
		static const DirectX::XMFLOAT3 sLightPosition;
		/**
		* The range of the light in atomic units in order to attenuate the light over distance.
		*/
		static const float sLightRangeAU;
	};
}
