#pragma once
#include "GameObject.h"

namespace NCL {
	namespace CSC8503 {
		enum StateFunctionType
		{
			void_float,
			void_float_Vector3,
			void_float_GameObject,
		};

		typedef std::function<void(float)> StateUpdateFunction_float;
		typedef std::function<void(float, Vector3)> StateupdateFunction_float_Vector3;
		typedef std::function<void(float, GameObject*)> StateUpdateFunction_float_GameObject;

		class  State {
		public:
			State() {}
			State(StateUpdateFunction_float someFunc) {
				type            = void_float;
				func_float      = someFunc;
			}

			State(StateupdateFunction_float_Vector3 f, Vector3 vec) {
				type                    = void_float_Vector3;
				func_float_Vector3_Grid = f;
				vector3                 = vec;
			}

			State(StateUpdateFunction_float_GameObject f, GameObject* gameObj) {
				type                       = void_float_GameObject;
				func_float_GameObject_Grid = f;
				gameObject                 = gameObj;
			}

			void Update(float dt) {
				switch (type)
				{
				case NCL::CSC8503::void_float:
					if (func_float != nullptr) {
						func_float(dt);
					}
					break;
				case NCL::CSC8503::void_float_Vector3:
					if (func_float_Vector3_Grid != nullptr) {
						func_float_Vector3_Grid(dt, vector3);
					}
					break;
				case NCL::CSC8503::void_float_GameObject:
					if (func_float_GameObject_Grid != nullptr) {
						func_float_GameObject_Grid(dt, gameObject);
					}
					break;
				default:
					std::cout << "Error: Error: Error: Error: State function type is undefined!\n";
					break;
				}
			}
		protected:
			StateFunctionType type;
			StateUpdateFunction_float func_float;
			StateupdateFunction_float_Vector3 func_float_Vector3_Grid;
			StateUpdateFunction_float_GameObject func_float_GameObject_Grid;
			Vector3 vector3;
			GameObject* gameObject;
		};
	}
}