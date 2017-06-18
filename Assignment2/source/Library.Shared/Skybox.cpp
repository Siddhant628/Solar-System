#include "pch.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(Skybox)

	Skybox::Skybox(Game& game, const shared_ptr<Camera>& camera, const wstring& cubeMapFileName, float scale) :
		DrawableGameComponent(game, camera),
		mCubeMapFileName(cubeMapFileName), mIndexCount(0),
		mWorldMatrix(MatrixHelper::Identity), mScaleMatrix(MatrixHelper::Identity)
	{
		XMStoreFloat4x4(&mScaleMatrix, XMMatrixScaling(scale, scale, scale));
	}

	void Skybox::Initialize()
	{

		// Load a compiled vertex shader
		vector<char> compiledVertexShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\SkyboxVS.cso", compiledVertexShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, mVertexShader.GetAddressOf()), "ID3D11Device::CreatedVertexShader() failed.");

		// Load a compiled pixel shader
		vector<char> compiledPixelShader;
		Utility::LoadBinaryFile(L"Content\\Shaders\\SkyboxPS.cso", compiledPixelShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, mPixelShader.GetAddressOf()), "ID3D11Device::CreatedPixelShader() failed.");

		// Create an input layout
		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		ThrowIfFailed(mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(), mInputLayout.GetAddressOf()), "ID3D11Device::CreateInputLayout() failed.");

		// Load the model
		Library::Model model = Library::Model("Content\\Models\\Sphere.obj.bin");

		// Create vertex and index buffers for the model
		Mesh* mesh = model.Meshes().at(0).get();
		CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, mVertexBuffer.ReleaseAndGetAddressOf());
		mesh->CreateIndexBuffer(*mGame->Direct3DDevice(), mIndexBuffer.ReleaseAndGetAddressOf());
		mIndexCount = static_cast<UINT>(mesh->Indices().size());

		ThrowIfFailed(DirectX::CreateDDSTextureFromFile(mGame->Direct3DDevice(), mCubeMapFileName.c_str(), nullptr, mSkyboxTexture.GetAddressOf()), "CreateDDSTextureFromFile() failed.");

		// Create constant buffer
		D3D11_BUFFER_DESC constantBufferDesc = { 0 };
		constantBufferDesc.ByteWidth = sizeof(mVertexCBufferPerObjectData);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerObject.GetAddressOf()), "ID3D11Device::CreateBuffer() failed.");
	}

	void Skybox::Update(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		const XMFLOAT3& position = mCamera->Position();
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mScaleMatrix) * XMMatrixTranslation(position.x, position.y, position.z));
	}

	void Skybox::Draw(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());

		UINT stride = sizeof(VertexPositionTexture);
		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX wvp = worldMatrix * mCamera->ViewProjectionMatrix();
		XMStoreFloat4x4(&mVertexCBufferPerObjectData.WorldViewProjection, XMMatrixTranspose(wvp));

		direct3DDeviceContext->UpdateSubresource(mVertexCBufferPerObject.Get(), 0, nullptr, &mVertexCBufferPerObjectData, 0, 0);
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mVertexCBufferPerObject.GetAddressOf());

		direct3DDeviceContext->PSSetShaderResources(0, 1, mSkyboxTexture.GetAddressOf());
		direct3DDeviceContext->PSSetSamplers(0, 1, SamplerStates::TrilinearClamp.GetAddressOf());

		direct3DDeviceContext->RSSetState(RasterizerStates::DisabledCulling.Get());
		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);
		direct3DDeviceContext->RSSetState(nullptr);
	}

	void Skybox::CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
		vector<XMFLOAT3>* textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates->size() == sourceVertices.size());

		vector<VertexPositionTexture> vertices;
		vertices.reserve(sourceVertices.size());
		for (UINT i = 0; i < sourceVertices.size(); i++)
		{
			const XMFLOAT3& position = sourceVertices.at(i);
			const XMFLOAT3& uv = textureCoordinates->at(i);
			vertices.push_back(VertexPositionTexture(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y)));
		}

		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(VertexPositionTexture) * vertices.size());
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData = { 0 };
		vertexSubResourceData.pSysMem = &vertices[0];
		ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer), "ID3D11Device::CreateBuffer() failed.");
	}
}