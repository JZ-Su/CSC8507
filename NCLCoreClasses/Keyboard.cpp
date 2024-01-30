#include "Keyboard.h"

using namespace NCL;

Keyboard::Keyboard() {
	//Initialise the arrays to false!
	memset(keyStates , 0, KeyCodes::MAXVALUE * sizeof(bool));
	memset(holdStates, 0, KeyCodes::MAXVALUE * sizeof(bool));
}

/*
Updates variables controlling whether a keyboard key has been
held for multiple frames.
*/
void Keyboard::UpdateFrameState(float msec) {
	memcpy(holdStates, keyStates, KeyCodes::MAXVALUE * sizeof(bool));
}

void Keyboard::Sleep() {
	isAwake = false; 
	memset(keyStates , 0, KeyCodes::MAXVALUE * sizeof(bool));
	memset(holdStates, 0, KeyCodes::MAXVALUE * sizeof(bool));
}

void Keyboard::Wake() {
	isAwake = true; 
}

char Keyboard::GetKey() const {
	if (KeyPressed(KeyCodes::A)) return 'a';
	else if (KeyPressed(KeyCodes::B)) return 'b';
	else if (KeyPressed(KeyCodes::C)) return 'c';
	else if (KeyPressed(KeyCodes::D)) return 'd';
	else if (KeyPressed(KeyCodes::E)) return 'e';
	else if (KeyPressed(KeyCodes::F)) return 'f';
	else if (KeyPressed(KeyCodes::G)) return 'g';
	else if (KeyPressed(KeyCodes::H)) return 'h';
	else if (KeyPressed(KeyCodes::I)) return 'i';
	else if (KeyPressed(KeyCodes::J)) return 'j';
	else if (KeyPressed(KeyCodes::K)) return 'k';
	else if (KeyPressed(KeyCodes::L)) return 'l';
	else if (KeyPressed(KeyCodes::M)) return 'm';
	else if (KeyPressed(KeyCodes::N)) return 'n';
	else if (KeyPressed(KeyCodes::O)) return 'o';
	else if (KeyPressed(KeyCodes::P)) return 'p';
	else if (KeyPressed(KeyCodes::Q)) return 'q';
	else if (KeyPressed(KeyCodes::R)) return 'r';
	else if (KeyPressed(KeyCodes::S)) return 's';
	else if (KeyPressed(KeyCodes::T)) return 't';
	else if (KeyPressed(KeyCodes::U)) return 'u';
	else if (KeyPressed(KeyCodes::V)) return 'v';
	else if (KeyPressed(KeyCodes::W)) return 'w';
	else if (KeyPressed(KeyCodes::X)) return 'x';
	else if (KeyPressed(KeyCodes::Y)) return 'y';
	else if (KeyPressed(KeyCodes::Z)) return 'z';
	else if (KeyPressed(KeyCodes::BACK)) return 8;
	else return '0';
}