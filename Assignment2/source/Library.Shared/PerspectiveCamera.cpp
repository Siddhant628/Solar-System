#include "pch.h"

using namespace DirectX;

namespace Library
{
    RTTI_DEFINITIONS(PerspectiveCamera)

    const float PerspectiveCamera::DefaultFieldOfView = XM_PIDIV4;
	const float PerspectiveCamera::DefaultAspectRatio = 4.0f / 3.0f;

    PerspectiveCamera::PerspectiveCamera(Game& game) :
		Camera(game),
        mFieldOfView(DefaultFieldOfView), mAspectRatio(DefaultAspectRatio)
    {
    }

    PerspectiveCamera::PerspectiveCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance) :
		Camera(game, nearPlaneDistance, farPlaneDistance),
        mFieldOfView(fieldOfView), mAspectRatio(aspectRatio)
    {
    }

    float PerspectiveCamera::AspectRatio() const
    {
        return mAspectRatio;
    }

    float PerspectiveCamera::FieldOfView() const
    {
        return mFieldOfView;
    }

    void PerspectiveCamera::UpdateProjectionMatrix()
    {
        XMMATRIX projectionMatrix = XMMatrixPerspectiveFovRH(mFieldOfView, mAspectRatio, mNearPlaneDistance, mFarPlaneDistance);
        XMStoreFloat4x4(&mProjectionMatrix, projectionMatrix);
    }
}
