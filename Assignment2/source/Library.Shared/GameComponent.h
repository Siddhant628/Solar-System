#pragma once

#include "RTTI.h"

namespace Library
{
	class Game;
	class GameTime;

	class GameComponent : public RTTI
	{
		RTTI_DECLARATIONS(GameComponent, RTTI)

	public:
		GameComponent();
		GameComponent(Game& game);
		GameComponent(const GameComponent&) = default;
		GameComponent& operator=(const GameComponent&) = default;
		GameComponent(GameComponent&&) = default;
		GameComponent& operator=(GameComponent&&) = default;
		virtual ~GameComponent() = default;

		Game* GetGame();
		void SetGame(Game& game);
		bool Enabled() const;
		void SetEnabled(bool enabled);

		virtual void Initialize();
		virtual void Update(const GameTime& gameTime);

	protected:
		Game* mGame;
		bool mEnabled;
	};
}
