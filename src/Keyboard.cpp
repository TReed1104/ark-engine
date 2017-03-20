#include "Keyboard.h"
#include "Engine.h"

Engine* Keyboard::Engine_Pointer;

Keyboard::Keyboard() {
	// Initialise the dictionary of the lower case keys
	lowerKeyStates["a"] = false;
	lowerKeyStates["b"] = false;
	lowerKeyStates["c"] = false;
	lowerKeyStates["d"] = false;
	lowerKeyStates["e"] = false;
	lowerKeyStates["f"] = false;
	lowerKeyStates["g"] = false;
	lowerKeyStates["h"] = false;
	lowerKeyStates["i"] = false;
	lowerKeyStates["j"] = false;
	lowerKeyStates["k"] = false;
	lowerKeyStates["l"] = false;
	lowerKeyStates["m"] = false;
	lowerKeyStates["n"] = false;
	lowerKeyStates["o"] = false;
	lowerKeyStates["p"] = false;
	lowerKeyStates["q"] = false;
	lowerKeyStates["r"] = false;
	lowerKeyStates["s"] = false;
	lowerKeyStates["t"] = false;
	lowerKeyStates["u"] = false;
	lowerKeyStates["v"] = false;
	lowerKeyStates["w"] = false;
	lowerKeyStates["x"] = false;
	lowerKeyStates["y"] = false;
	lowerKeyStates["z"] = false;

	// Initialise the dictionary of the upper case keys
	capitalKeyStates["A"] = false;
	capitalKeyStates["B"] = false;
	capitalKeyStates["C"] = false;
	capitalKeyStates["D"] = false;
	capitalKeyStates["E"] = false;
	capitalKeyStates["F"] = false;
	capitalKeyStates["G"] = false;
	capitalKeyStates["H"] = false;
	capitalKeyStates["I"] = false;
	capitalKeyStates["J"] = false;
	capitalKeyStates["K"] = false;
	capitalKeyStates["L"] = false;
	capitalKeyStates["M"] = false;
	capitalKeyStates["N"] = false;
	capitalKeyStates["O"] = false;
	capitalKeyStates["P"] = false;
	capitalKeyStates["Q"] = false;
	capitalKeyStates["R"] = false;
	capitalKeyStates["S"] = false;
	capitalKeyStates["T"] = false;
	capitalKeyStates["U"] = false;
	capitalKeyStates["V"] = false;
	capitalKeyStates["W"] = false;
	capitalKeyStates["X"] = false;
	capitalKeyStates["Y"] = false;
	capitalKeyStates["Z"] = false;

	// Initialise the dictionary of the number keys
	numberKeyStates["0"] = false;
	numberKeyStates["1"] = false;
	numberKeyStates["2"] = false;
	numberKeyStates["3"] = false;
	numberKeyStates["4"] = false;
	numberKeyStates["5"] = false;
	numberKeyStates["6"] = false;
	numberKeyStates["7"] = false;
	numberKeyStates["8"] = false;
	numberKeyStates["9"] = false;
}

Keyboard::~Keyboard() {

}

bool Keyboard::GetKeyState(const LowerKeys& key) {
	switch (key) {
		case Keyboard::a:
			break;
		case Keyboard::b:
			break;
		case Keyboard::c:
			break;
		case Keyboard::d:
			break;
		case Keyboard::e:
			break;
		case Keyboard::f:
			break;
		case Keyboard::g:
			break;
		case Keyboard::h:
			break;
		case Keyboard::i:
			break;
		case Keyboard::j:
			break;
		case Keyboard::k:
			break;
		case Keyboard::l:
			break;
		case Keyboard::m:
			break;
		case Keyboard::n:
			break;
		case Keyboard::o:
			break;
		case Keyboard::p:
			break;
		case Keyboard::q:
			break;
		case Keyboard::r:
			break;
		case Keyboard::s:
			break;
		case Keyboard::t:
			break;
		case Keyboard::u:
			break;
		case Keyboard::v:
			break;
		case Keyboard::x:
			break;
		case Keyboard::y:
			break;
		case Keyboard::z:
			break;
		default:
			break;
	}
}

bool Keyboard::GetKeyState(const CapitalKeys& key) {
	switch (key) {
		case Keyboard::A:
			break;
		case Keyboard::B:
			break;
		case Keyboard::C:
			break;
		case Keyboard::D:
			break;
		case Keyboard::E:
			break;
		case Keyboard::F:
			break;
		case Keyboard::G:
			break;
		case Keyboard::H:
			break;
		case Keyboard::I:
			break;
		case Keyboard::J:
			break;
		case Keyboard::K:
			break;
		case Keyboard::L:
			break;
		case Keyboard::M:
			break;
		case Keyboard::N:
			break;
		case Keyboard::O:
			break;
		case Keyboard::P:
			break;
		case Keyboard::Q:
			break;
		case Keyboard::R:
			break;
		case Keyboard::S:
			break;
		case Keyboard::T:
			break;
		case Keyboard::U:
			break;
		case Keyboard::V:
			break;
		case Keyboard::X:
			break;
		case Keyboard::Y:
			break;
		case Keyboard::Z:
			break;
		default:
			break;
	}
}

bool Keyboard::GetKeyState(const NumberKeys& key) {
	switch (key) {
		case Keyboard::num0:
			break;
		case Keyboard::num1:
			break;
		case Keyboard::num2:
			break;
		case Keyboard::num3:
			break;
		case Keyboard::num4:
			break;
		case Keyboard::num5:
			break;
		case Keyboard::num6:
			break;
		case Keyboard::num7:
			break;
		case Keyboard::num8:
			break;
		case Keyboard::num9:
			break;
		default:
			break;
	}
}


void Keyboard::UpdateKetStates(const SDL_KeyboardEvent & keyboardEvent) {

}