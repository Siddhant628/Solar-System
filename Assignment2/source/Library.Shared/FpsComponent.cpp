#include "pch.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(FpsComponent)

	FpsComponent::FpsComponent(Game& game) :
		DrawableGameComponent(game),
		mTextPosition(0.0f, 20.0f), mFrameCount(0), mFrameRate(0)
	{
	}

	XMFLOAT2& FpsComponent::TextPosition()
	{
		return mTextPosition;
	}

	int FpsComponent::FrameRate() const
	{
		return mFrameCount;
	}

	void FpsComponent::Initialize()
	{
		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");
	}

	void FpsComponent::Update(const GameTime& gameTime)
	{
		if ((gameTime.TotalGameTime() - mLastTotalGameTime).count() >= 1000)
		{
			mLastTotalGameTime = gameTime.TotalGameTime();
			mFrameRate = mFrameCount;
			mFrameCount = 0;
		}

		++mFrameCount;
	}

	void FpsComponent::Draw(const GameTime& gameTime)
	{
		mSpriteBatch->Begin();

		wostringstream fpsLabel;
		fpsLabel << setprecision(4) << L"Frame Rate: " << mFrameRate << "    Total Elapsed Time: " << gameTime.TotalGameTimeSeconds().count();
		mSpriteFont->DrawString(mSpriteBatch.get(), fpsLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
	}
}