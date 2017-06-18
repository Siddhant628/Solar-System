#include "pch.h"

using namespace DirectX;

namespace Library
{
    RTTI_DEFINITIONS(OrthographicCamera)

	const float OrthographicCamera::DefaultViewWidth = 100.0f;
	const float OrthographicCamera::DefaultViewHeight = 100.0f;

    OrthographicCamera::OrthographicCamera(Game& game) :
		Camera(game),
		mViewWidth(DefaultViewWidth), mViewHeight(DefaultViewHeight)
    {
    }

	OrthographicCamera::OrthographicCamera(Game& game, float viewWidth, float viewHeight, float nearPlaneDistance, float farPlaneDistance) :
		Camera(game, nearPlaneDistance, farPlaneDistance),
		mViewWidth(viewWidth), mViewHeight(viewHeight)
    {
    }
    
	float OrthographicCamera::ViewWidth() const
	{
		return mViewWidth;
	}

	void OrthographicCamera::SetViewWidth(float viewWidth)
	{
		if (viewWidth > 0.0f)
		{
			mViewWidth = viewWidth;
		}
	}

	float OrthographicCamera::ViewHeight() const
	{
		return mViewHeight;
	}

	void OrthographicCamera::SetViewHeight(float viewHeight)
	{
		if (viewHeight > 0.0f)
		{
			mViewHeight = viewHeight;
		}
	}

    void OrthographicCamera::UpdateProjectionMatrix()
    {
		XMMATRIX projectionMatrix = XMMatrixOrthographicRH(mViewWidth, mViewHeight, mNearPlaneDistance, mFarPlaneDistance);
        XMStoreFloat4x4(&mProjectionMatrix, projectionMatrix);
    }
}
