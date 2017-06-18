#pragma once

#include "Camera.h"

namespace Library
{
    class GameTime;

    class PerspectiveCamera : public Camera
    {
		RTTI_DECLARATIONS(PerspectiveCamera, Camera)

    public:
		explicit PerspectiveCamera(Game& game);
		PerspectiveCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);
		PerspectiveCamera(const PerspectiveCamera&) = default;
		PerspectiveCamera& operator=(const PerspectiveCamera&) = default;
		PerspectiveCamera(PerspectiveCamera&&) = default;
		PerspectiveCamera& operator=(PerspectiveCamera&&) = default;
		virtual ~PerspectiveCamera() = default;
       
        float AspectRatio() const;
        float FieldOfView() const;

		virtual void UpdateProjectionMatrix() override;
        
        static const float DefaultFieldOfView;
		static const float DefaultAspectRatio;

    protected:
        float mFieldOfView;
        float mAspectRatio;
    };
}

