#pragma once
#include "BasicExamples.h"
#include "GameTechRenderer.h"

namespace NCL {
	struct Level
	{
		vector<GameObject*> objectList;
		GameObject* portal;

		void AddObject(GameObject* o) { objectList.push_back(o); }
		void AddObject(vector<GameObject*> l) { objectList.insert(objectList.end(), l.begin(), l.end()); }

		// deep copy
		//Level& operator=(const Level& from) {
		//	if (this != &from) {
		//		for (auto ptr : objectList) {
		//			delete ptr;
		//		}
		//		objectList.clear();
		//		for (const auto& ptr : from.objectList) {
		//			GameObject* newPtr = new GameObject(*ptr);
		//			objectList.push_back(newPtr);
		//		}
		//	}
		//	return *this;
		//}
	};

	class GameLevel : public BasicExamples
	{
	public:
		GameLevel(GameTechRenderer* render);
		void AddLevelToWorld(GameWorld* world, Level l);
		static void RemoveLevel(GameWorld* world, Level* l, bool andClear = false, bool andDelete = false);

		void CreateLevel1();
		void CreateLevel2();
		void CreateLevel3();
		void CreateLevel4();
		void CreateGeneric();
		void CreateConnectionLevel();

		vector<GameObject*> CreatePortal(const Vector3& position);

		Level* GetLevel1() { return &level1; }
		Level* GetLevel2() { return &level2; }
		Level* GetLevel3() { return &level3; }
		Level* GetLevel4() { return &level4; }
		Level* GetLevel4r() { return &level4r; }
		Level GetGeneric() { return Generic; }
		Level* GetConnection() { return &connection; }

		void AddLevelToWorld(GameWorld* world, int i, bool isRotate, bool isReverse);
		void CreateLevel4_Normal();
		void CreateLevel4_Reverse();
		void CreateLevel4_Rotate();
		void CreateLevel4_RR();
		vector<Level> GetNormalL4() { return level4_normal; }
		vector<Level> GetReverseL4() { return level4_rotate; }
		vector<Level> GetRotateL4() { return level4_rotate; }
		vector<Level> GetReverseRotateL4() { return level4_reverse_rotate; }
		Door* GetL4Door() { return door; }

		GameObject* GetBeginArea() const { return beginArea; }
		GameObject* GetTrueEndArea() const { return trueEndArea; }
		GameObject* GetFalseEndArea() const { return falseEndArea; }

		void CreateLevel1_BookShelfs();
		void CreateLevel1_SmallShelfs();
		void CreateLevel1_Tables();
		void CreateLevel1_Columns();
		void CreateLevel1_Stairs();
		void CreateLevel1_Handrail();
		void CreateLevel1_Coins();

		vector<Door*> GetL2Doors() const { return l2_Doors; }
		bool CheckCoinList();

	protected:
		Level level1;
		Level level2;
		Level level3;
		Level level4;
		Level level4r;
		Level Generic;
		Level connection;

		GameObject* beginArea;
		GameObject* trueEndArea;
		GameObject* falseEndArea;
		Door*		door;

		vector<Level> level4_normal;
		vector<Level> level4_reverse;
		vector<Level> level4_rotate;
		vector<Level> level4_reverse_rotate;

		vector<Door*> l2_Doors;

		vector<GameObject*> coinList;
	};
}
