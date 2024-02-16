#pragma once
#include "Vector3.h"
#include "GameObject.h"

namespace NCL {
	namespace CSC8503 {
		enum FunctionType
		{
			bool_void,
			bool_float,
			bool_Vector3,
			bool_GameObject
		};

		class State;
		typedef std::function<bool()> StateTransitionFunction_bool_void;
		typedef std::function<bool(float*)> StatetransitionFunction_bool_float;
		typedef std::function<bool(NCL::Maths::Vector3)> StateTransitionFunction_bool_Vector3;
		typedef std::function<bool(GameObject*)> StateTransitionFunction_bool_GameObject;
		class StateTransition	{
		public:
			StateTransition(State* source, State* dest, StateTransitionFunction_bool_void f) {
				sourceState = source;
				destinationState = dest;
				function_bool_void = f;
				type = bool_void;
			}

			StateTransition(State* source, State* dest, StateTransitionFunction_bool_Vector3 f, NCL::Maths::Vector3 vec) {
				sourceState		      = source;
				destinationState      = dest;
				function_bool_Vector3 = f;
				vector3               = vec;
				type                  = bool_Vector3;
			}

			StateTransition(State* source, State* dest, StateTransitionFunction_bool_GameObject f, GameObject* gameObj) {
				sourceState              = source;
				destinationState         = dest;
				function_bool_GameObject = f;
				gameObject               = gameObj;
				type                     = bool_GameObject;
			}

			StateTransition(State* source, State* dest, StatetransitionFunction_bool_float func, float* fl) {
				sourceState = source;
				destinationState = dest;
				function_bool_float = func;
				f = fl;
				type = bool_float;
			}

			bool CanTransition() const {
				switch (type)
				{
				case NCL::CSC8503::bool_void:
					return function_bool_void();
					break;
				case NCL::CSC8503::bool_float:
					return function_bool_float(f);
					break;
				case NCL::CSC8503::bool_Vector3:
					return function_bool_Vector3(vector3);
					break;
				case NCL::CSC8503::bool_GameObject:
					return function_bool_GameObject(gameObject);
					break;
				default:
					std::cout << "Error: Error: Error: Error: Transition function type is undefined!\n";
					return false;
					break;
				}
			}

			State* GetDestinationState()  const {
				return destinationState;
			}

			State* GetSourceState() const {
				return sourceState;
			}

		protected:
			FunctionType type;
			StateTransitionFunction_bool_void       function_bool_void;
			StateTransitionFunction_bool_Vector3    function_bool_Vector3;
			StateTransitionFunction_bool_GameObject function_bool_GameObject;
			StatetransitionFunction_bool_float      function_bool_float;
			State* sourceState;
			State* destinationState;
			NCL::Maths::Vector3 vector3;
			GameObject* gameObject;
			float* f;
		};
	}
}

