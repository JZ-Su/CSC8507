#pragma once
#include "BasicExamples.h"
#include "GameTechRenderer.h"

namespace NCL {
	struct Level
	{
		vector<GameObject*> objectList;
		int listSize;
	};

	class GameLevel : public BasicExamples
	{
	public:
		GameLevel(GameTechRenderer* render);
		void AddLevelToWorld(GameWorld* world, Level l);

		void CreateLevel1();
		void CreateLevel2();
		void CreateLevel3();
		void CreateLevel4();

		Level GetLevel1() { return level1; }
		Level GetLevel2() { return level2; }
		Level GetLevel3() { return level3; }
		Level GetLevel4() { return level4; }

	protected:
		Level level1;
		Level level2;
		Level level3;
		Level level4;
	};
}