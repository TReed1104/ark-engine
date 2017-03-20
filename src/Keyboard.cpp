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
			return lowerKeyStates["a"];
		case Keyboard::b:
			return lowerKeyStates["b"];
		case Keyboard::c:
			return lowerKeyStates["c"];
		case Keyboard::d:
			return lowerKeyStates["d"];
		case Keyboard::e:
			return lowerKeyStates["e"];
		case Keyboard::f:
			return lowerKeyStates["f"];
		case Keyboard::g:
			return lowerKeyStates["g"];
		case Keyboard::h:
			return lowerKeyStates["h"];
		case Keyboard::i:
			return lowerKeyStates["i"];
		case Keyboard::j:
			return lowerKeyStates["j"];
		case Keyboard::k:
			return lowerKeyStates["k"];
		case Keyboard::l:
			return lowerKeyStates["l"];
		case Keyboard::m:
			return lowerKeyStates["m"];
		case Keyboard::n:
			return lowerKeyStates["n"];
		case Keyboard::o:
			return lowerKeyStates["o"];
		case Keyboard::p:
			return lowerKeyStates["p"];
		case Keyboard::q:
			return lowerKeyStates["q"];
		case Keyboard::r:
			return lowerKeyStates["r"];
		case Keyboard::s:
			return lowerKeyStates["s"];
		case Keyboard::t:
			return lowerKeyStates["t"];
		case Keyboard::u:
			return lowerKeyStates["u"];
		case Keyboard::v:
			return lowerKeyStates["v"];
		case Keyboard::w:
			return lowerKeyStates["w"];
		case Keyboard::x:
			return lowerKeyStates["x"];
		case Keyboard::y:
			return lowerKeyStates["y"];
		case Keyboard::z:
			return lowerKeyStates["z"];
		default:
			return false;
	}
}

bool Keyboard::GetKeyState(const CapitalKeys& key) {
	switch (key) {
		case Keyboard::A:
			return capitalKeyStates["A"];
		case Keyboard::B:
			return capitalKeyStates["B"];
		case Keyboard::C:
			return capitalKeyStates["C"];
		case Keyboard::D:
			return capitalKeyStates["D"];
		case Keyboard::E:
			return capitalKeyStates["E"];
		case Keyboard::F:
			return capitalKeyStates["F"];
		case Keyboard::G:
			return capitalKeyStates["G"];
		case Keyboard::H:
			return capitalKeyStates["H"];
		case Keyboard::I:
			return capitalKeyStates["I"];
		case Keyboard::J:
			return capitalKeyStates["J"];
		case Keyboard::K:
			return capitalKeyStates["K"];
		case Keyboard::L:
			return capitalKeyStates["L"];
		case Keyboard::M:
			return capitalKeyStates["M"];
		case Keyboard::N:
			return capitalKeyStates["N"];
		case Keyboard::O:
			return capitalKeyStates["O"];
		case Keyboard::P:
			return capitalKeyStates["P"];
		case Keyboard::Q:
			return capitalKeyStates["Q"];
		case Keyboard::R:
			return capitalKeyStates["R"];
		case Keyboard::S:
			return capitalKeyStates["S"];
		case Keyboard::T:
			return capitalKeyStates["T"];
		case Keyboard::U:
			return capitalKeyStates["U"];
		case Keyboard::V:
			return capitalKeyStates["V"];
		case Keyboard::W:
			return capitalKeyStates["W"];
		case Keyboard::X:
			return capitalKeyStates["X"];
		case Keyboard::Y:
			return capitalKeyStates["Y"];
		case Keyboard::Z:
			return capitalKeyStates["Z"];
		default:
			break;
	}
}

bool Keyboard::GetKeyState(const NumberKeys& key) {
	switch (key) {
		case Keyboard::num0:
			return numberKeyStates["0"];
		case Keyboard::num1:
			return numberKeyStates["1"];
		case Keyboard::num2:
			return numberKeyStates["2"];
		case Keyboard::num3:
			return numberKeyStates["3"];
		case Keyboard::num4:
			return numberKeyStates["4"];
		case Keyboard::num5:
			return numberKeyStates["5"];
		case Keyboard::num6:
			return numberKeyStates["6"];
		case Keyboard::num7:
			return numberKeyStates["7"];
		case Keyboard::num8:
			return numberKeyStates["8"];
		case Keyboard::num9:
			return numberKeyStates["9"];
		default:
			break;
	}
}


void Keyboard::UpdateKetStates(const SDL_KeyboardEvent & keyboardEvent) {

}