#pragma once
#include "ColliderObject.h"
#include "DrawableObject.h"
#include "EnemyInfo.h"
#include "EnemyObject.h"
#include "SimpleObject.h"
#include "Transform.h"
#include "PlayerObject.h"
#include <map>
#include <string>
#include <utility>
#include <list>

using namespace std;

constexpr float MAP_SCALE = 50.0f;

class MapLoader {
	private:
		enum ObjectType {
			AIR = 0,
			GROUND,
			PLAYER,
			PHYSICS_OBJ,
			ENEMY_NORMAL,
			PROCESSED
		};

		struct ObjectProperty {
			ObjectProperty(ObjectType type, Transform transform, float width, float height);
			ObjectType type;
			Transform transform;
			float width;
			float height;
		};

		struct RawMapData {
			vector<vector<ObjectType>> data;
			vector<ObjectType>& operator[](int i);
			int size();
			void clear();
		};

		RawMapData rawMapData;
		vector<ObjectProperty> processedMapData;

		map<EnemyType, EnemyInfo> enemyTypeMap;

		/// Helper functions ///
		void alignData(int length);
		pair<pair<int, int>, pair<int, int>> getObjectRange(int x, int y, ObjectType type);
		void markAsProcessed(int startX, int endX, int startY, int endY, ObjectType type);
		ObjectProperty getObjectProperty(int x, int y, ObjectType type);

	public:
		MapLoader();
		~MapLoader();
		void readData(string fileName);
		void loadDataToScene(list<DrawableObject*>& objectList, PlayerObject*& player);
		void appendDataToScene(list<DrawableObject*>& objectList, PlayerObject*& player);
		void offsetMap(list<DrawableObject*>& objectList, glm::vec3 offset);
		void offsetMapToCenter(list<DrawableObject*>& objectList);
		void clearData();

		void addEnemyType(EnemyType type, EnemyInfo enemyInfo);
		map<EnemyType, EnemyInfo>& getEnemyTypeMap();
};