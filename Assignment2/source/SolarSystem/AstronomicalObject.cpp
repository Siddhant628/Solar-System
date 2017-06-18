#include "pch.h"

/**
* The amount of time in seconds for one day to pass on earth.
*/
#define SCALE_TIME_FOR_DAY 0.5f;
/**
* The amount of distance is world space a single astronomical unit will scale up with.
*/
#define SCALE_ASTRONOMICAL_UNIT 300.0f;


using namespace std;
using namespace Library;
using namespace DirectX;

namespace Rendering
{
	RTTI_DEFINITIONS(AstronomicalObject)

	const std::unordered_map<AstronomicalObjectName, AstronomicalObjectData> AstronomicalObject::sAstronomicalObjects =
	{
		{ AstronomicalObjectName::Sun,			{1.0f, 0.0f, 25.375f, 0.0f, 0.0f, 15.0f, L"Content\\Textures\\SunColorMap.jpg"}},
		{ AstronomicalObjectName::Mercury,		{0.0f, 177.43f, 58.646f, 87.969f, 0.389f, 0.382f, L"Content\\Textures\\MercuryColorMap.jpg"}},
		{ AstronomicalObjectName::Venus,		{0.0f, 2.64f, 243.01f, 224.7f, 0.723f, 0.949f, L"Content\\Textures\\VenusColorMap.jpg"}},
		{ AstronomicalObjectName::Earth,		{0.0f, 23.44f, 1.0f, 365.256f, 1.0f, 1.0f, L"Content\\Textures\\EarthColorMap.jpg"}},
		{ AstronomicalObjectName::Moon,			{0.0f, 6.687f, 27.321f, 27.321f, 0.05f/*0.00257003846f*/, 0.273f, L"Content\\Textures\\MoonColorMap.jpg"}},
		{ AstronomicalObjectName::Mars,			{0.0f, 25.19f, 1.024f, 686.98f, 1.524f, 0.532f, L"Content\\Textures\\MarsColorMap.jpg"}},
		{ AstronomicalObjectName::Jupiter,		{0.0f, 3.13f, 0.4097222f, 4328.9f, 5.203f, 11.19f, L"Content\\Textures\\JupiterColorMap.jpg"}},
		{ AstronomicalObjectName::Saturn,		{0.0f, 26.73f, 0.42638922f, 10734.65f, 9.582f, 9.26f, L"Content\\Textures\\SaturnColorMap.jpg"}},
		{ AstronomicalObjectName::Uranus,		{0.0f, 97.9f, 0.7166667f, 30674.6f, 19.20f, 4.01f, L"Content\\Textures\\UranusColorMap.jpg"}},
		{ AstronomicalObjectName::Neptune,		{0.0f, 28.32f, 0.67125f, 59757.8f, 30.05f, 3.88f, L"Content\\Textures\\NeptuneColorMap.jpg"}},
		{ AstronomicalObjectName::Pluto,		{0.0f, 122.0f, 6.3874f, 90494.45f, 39.48f, 0.18f, L"Content\\Textures\\PlutoColorMap.jpg"}},
	};
	
	const DirectX::XMFLOAT3 AstronomicalObject::sLightPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	const float AstronomicalObject::sLightRangeAU = 50.0f;

	AstronomicalObject::AstronomicalObject(Game & game, const shared_ptr<Camera>& camera, AstronomicalObjectName name) :
		DrawableGameComponent(game, camera), mAstronomicalObjectName(name), mWorldMatrix(MatrixHelper::Identity),
		mRenderStateHelper(game), mIndexCount(0), mTextPosition(0.0f, 40.0f), mAnimationEnabled(true),
		mCurrentRotationDegrees(0.0f), mCurrentRevolutionDegrees(0.0f), mRotationRate(0.0f), mRevolutionRate(0.0f), mPointLight(nullptr), mParent(nullptr)
	{
		if(mAstronomicalObjectName == Rendering::AstronomicalObjectName::Sun)
		{
			float lightRange = sLightRangeAU * SCALE_ASTRONOMICAL_UNIT;
			mPointLight = new PointLight(game, sLightPosition, lightRange);
		}
	}

	AstronomicalObject::~AstronomicalObject()
	{
		if(mAstronomicalObjectName == AstronomicalObjectName::Sun)
		{
			delete mPointLight;
		}
	}

	bool AstronomicalObject::AnimationEnabled() const
	{
		return mAnimationEnabled;
	}

	void AstronomicalObject::SetAnimationEnabled(bool enabled)
	{
		mAnimationEnabled = enabled;
	}

	void AstronomicalObject::Initialize()
	{
		// Load a compiled vertex shader
		vector<char> compiledVertexShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\PlanetVS.cso", compiledVertexShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, mVertexShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedVertexShader() failed.");

		// Load a compiled pixel shader
		vector<char> compiledPixelShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\PlanetPS.cso", compiledPixelShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, mPixelShader.ReleaseAndGetAddressOf()), "ID3D11Device::CreatedPixelShader() failed.");

		// Create an input layout
		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		ThrowIfFailed(mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(), mInputLayout.ReleaseAndGetAddressOf()), "ID3D11Device::CreateInputLayout() failed.");

		// Load the model
		Library::Model model("Content\\Models\\Sphere.obj.bin");

		// Create vertex and index buffers for the model
		Library::Mesh* mesh = model.Meshes().at(0).get();
		CreateVertexBuffer(*mesh, mVertexBuffer.ReleaseAndGetAddressOf());
		mesh->CreateIndexBuffer(*mGame->Direct3DDevice(), mIndexBuffer.ReleaseAndGetAddressOf());
		mIndexCount = static_cast<uint32_t>(mesh->Indices().size());

		// Create constant buffers
		D3D11_BUFFER_DESC constantBufferDesc = { 0 };
		constantBufferDesc.ByteWidth = sizeof(VSCBufferPerFrame);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mVSCBufferPerFrame.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		constantBufferDesc.ByteWidth = sizeof(VSCBufferPerObject);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mVSCBufferPerObject.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		constantBufferDesc.ByteWidth = sizeof(PSCBufferPerFrame);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mPSCBufferPerFrame.ReleaseAndGetAddressOf()), "ID3D11Device::CreateBuffer() failed.");

		// Load textures for the color
		wstring textureName = sAstronomicalObjects.at(mAstronomicalObjectName).TextureName;
		ThrowIfFailed(CreateWICTextureFromFile(mGame->Direct3DDevice(), textureName.c_str(), nullptr, mColorTexture.ReleaseAndGetAddressOf()), "CreateWICTextureFromFile() failed.");

		// Create text rendering helpers
		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");

		// Retrieve the keyboard service
		mKeyboard = reinterpret_cast<KeyboardComponent*>(mGame->Services().GetService(KeyboardComponent::TypeIdClass()));

		// Setup the point light
		mVSCBufferPerFrameData.LightPosition = mPointLight->Position();
		mVSCBufferPerFrameData.LightRadius = mPointLight->Radius();
		mPSCBufferPerFrameData.LightPosition = mPointLight->Position();
		mPSCBufferPerFrameData.LightColor = ColorHelper::ToFloat3(mPointLight->Color(), true);

		// Set default ambient light
		float ambientIntensity = sAstronomicalObjects.at(mAstronomicalObjectName).AmbientIntensity;
		mPSCBufferPerFrameData.AmbientColor = XMFLOAT3(ambientIntensity, ambientIntensity, ambientIntensity);

		// Update the vertex and pixel shader constant buffers
		mGame->Direct3DDeviceContext()->UpdateSubresource(mVSCBufferPerFrame.Get(), 0, nullptr, &mVSCBufferPerFrameData, 0, 0);

		// Measure the rotation and revolution rate related degrees
		mRotationRate = 360.0f / sAstronomicalObjects.at(mAstronomicalObjectName).RotationDays;			// Degrees to rotate for each day on earth
		mRotationRate /= SCALE_TIME_FOR_DAY;															// Degrees to rotate this object each second of game
		
		if (mAstronomicalObjectName != AstronomicalObjectName::Sun)
		{
			mRevolutionRate = 360.0f / sAstronomicalObjects.at(mAstronomicalObjectName).RevolutionDays;	// Degrees to revolve each day on earth
			mRevolutionRate /= SCALE_TIME_FOR_DAY;														// Degrees to revolve each second of game
		}
	}

	void AstronomicalObject::Update(const GameTime& gameTime)
	{
		if (mAnimationEnabled)
		{
			UpdateAstronomicalObjectRotation(gameTime);

			XMMATRIX transformation = XMLoadFloat4x4(&MatrixHelper::Identity);
			// Scaling;
			float scale = sAstronomicalObjects.at(mAstronomicalObjectName).Scale;
			transformation *= XMMATRIX(scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, 1);
			// Rotation about its axis
			transformation *= XMMatrixRotationY(XMConvertToRadians(mCurrentRotationDegrees));
			// Axial tilt
			transformation *= XMMatrixRotationZ(XMConvertToRadians(sAstronomicalObjects.at(mAstronomicalObjectName).AxialTilt));
			// Translation away from the sun
			float orbitalDistance = sAstronomicalObjects.at(mAstronomicalObjectName).OrbitalDistance;
			orbitalDistance *= SCALE_ASTRONOMICAL_UNIT;
			transformation *= XMMatrixTranslation(orbitalDistance, 0.0f, 0.0f);
			// Rotation around the sun
			transformation *= XMMatrixRotationY(XMConvertToRadians(mCurrentRevolutionDegrees));
			
			// Update position w.r.t to parent planet
			if(mParent)
			{
				// Parent's transformation due to distance from sun
				float parentTranslation = sAstronomicalObjects.at(mParent->mAstronomicalObjectName).OrbitalDistance;
				parentTranslation *= SCALE_ASTRONOMICAL_UNIT;
				XMMATRIX parentsTransformation = XMMatrixTranslation(parentTranslation, 0.0f, 0.0f);
				// Parent's transformation due to orbiting
				parentsTransformation *= XMMatrixRotationY(XMConvertToRadians(mParent->mCurrentRevolutionDegrees));
				// Get translation vector and translate
				XMFLOAT3 translation;
				MatrixHelper::GetTranslation(parentsTransformation, translation);
				transformation *= XMMatrixTranslation(translation.x, translation.y, translation.z);
			}
			XMStoreFloat4x4(&mWorldMatrix, transformation);
		}

		if (mKeyboard != nullptr)
		{
			if (mKeyboard->WasKeyPressedThisFrame(Keys::Space))
			{
				ToggleAnimation();
			}
		}
	}

	void AstronomicalObject::UpdateAstronomicalObjectRotation(const GameTime& gameTime)
	{
		// Update rotation about its own axis
		float seconds = gameTime.ElapsedGameTimeSeconds().count();
		mCurrentRotationDegrees += (mRotationRate * seconds);
		if (mCurrentRotationDegrees > 360.0f)
		{
			mCurrentRotationDegrees -= 360.0f;
		}
		// Update rotation about sun (revolving)
		mCurrentRevolutionDegrees += (mRevolutionRate * seconds);
		if(mCurrentRevolutionDegrees > 360.0f)
		{
			mCurrentRevolutionDegrees -= 360.0f;
		}
	}

	void AstronomicalObject::Draw(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);
		assert(mCamera != nullptr);

		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());

		UINT stride = sizeof(VertexPositionTextureNormal);
		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX wvp = worldMatrix * mCamera->ViewProjectionMatrix();
		wvp = XMMatrixTranspose(wvp);
		XMStoreFloat4x4(&mVSCBufferPerObjectData.WorldViewProjection, wvp);
		XMStoreFloat4x4(&mVSCBufferPerObjectData.World, XMMatrixTranspose(worldMatrix));
		direct3DDeviceContext->UpdateSubresource(mVSCBufferPerObject.Get(), 0, nullptr, &mVSCBufferPerObjectData, 0, 0);

		ID3D11Buffer* VSConstantBuffers[] = { mVSCBufferPerFrame.Get(), mVSCBufferPerObject.Get() };
		direct3DDeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(VSConstantBuffers), VSConstantBuffers);

		direct3DDeviceContext->UpdateSubresource(mPSCBufferPerFrame.Get(), 0, nullptr, &mPSCBufferPerFrameData, 0, 0);

		ID3D11Buffer* PSConstantBuffers[] = { mPSCBufferPerFrame.Get() };
		direct3DDeviceContext->PSSetConstantBuffers(0, ARRAYSIZE(PSConstantBuffers), PSConstantBuffers);

		ID3D11ShaderResourceView* PSShaderResources[] = { mColorTexture.Get() };
		direct3DDeviceContext->PSSetShaderResources(0, ARRAYSIZE(PSShaderResources), PSShaderResources);
		direct3DDeviceContext->PSSetSamplers(0, 1, SamplerStates::TrilinearWrap.GetAddressOf());

		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);

		// Draw help text
		mRenderStateHelper.SaveAll();
		mSpriteBatch->Begin();

		wostringstream helpLabel;
		helpLabel << L"Toggle Animation (Space)" << "\n";

		mSpriteFont->DrawString(mSpriteBatch.get(), helpLabel.str().c_str(), mTextPosition);
		mSpriteBatch->End();
		mRenderStateHelper.RestoreAll();
	}

	const Library::PointLight& AstronomicalObject::GetLight() const
	{
		if(mPointLight == nullptr)
		{
			throw std::runtime_error("Astronomical object has no light.");
		}
		return *mPointLight;
	}

	void AstronomicalObject::SetLight(const Library::PointLight& pointLight)
	{
		mPointLight = &pointLight;
	}

	const AstronomicalObject& AstronomicalObject::GetParentObject() const
	{
		if(!mParent)
		{
			throw std::runtime_error("Parent astronomical object absent.");
		}
		return *mParent;
	}

	void AstronomicalObject::SetParentObject(const AstronomicalObject& parent)
	{
		mParent = &parent;
	}

	void AstronomicalObject::CreateVertexBuffer(const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
		const vector<XMFLOAT3>& sourceNormals = mesh.Normals();
		const auto& sourceUVs = mesh.TextureCoordinates().at(0);

		vector<VertexPositionTextureNormal> vertices;
		vertices.reserve(sourceVertices.size());
		for (UINT i = 0; i < sourceVertices.size(); i++)
		{
			const XMFLOAT3& position = sourceVertices.at(i);
			const XMFLOAT3& uv = sourceUVs->at(i);
			const XMFLOAT3& normal = sourceNormals.at(i);

			vertices.push_back(VertexPositionTextureNormal(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y), normal));
		}
		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.ByteWidth = sizeof(VertexPositionTextureNormal) * static_cast<UINT>(vertices.size());
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData = { 0 };
		vertexSubResourceData.pSysMem = &vertices[0];
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer), "ID3D11Device::CreateBuffer() failed.");
	}

	void AstronomicalObject::ToggleAnimation()
	{
		mAnimationEnabled = !mAnimationEnabled;
	}
}