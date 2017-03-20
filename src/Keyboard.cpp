#include "Keyboard.h"
#include "Engine.h"

Engine* Keyboard::Engine_Pointer;

Keyboard::Keyboard() {
	// Initialise the dictionary of the lower case keys
	lowerCaseKeyStates["a"] = false;
	lowerCaseKeyStates["b"] = false;
	lowerCaseKeyStates["c"] = false;
	lowerCaseKeyStates["d"] = false;
	lowerCaseKeyStates["e"] = false;
	lowerCaseKeyStates["f"] = false;
	lowerCaseKeyStates["g"] = false;
	lowerCaseKeyStates["h"] = false;
	lowerCaseKeyStates["i"] = false;
	lowerCaseKeyStates["j"] = false;
	lowerCaseKeyStates["k"] = false;
	lowerCaseKeyStates["l"] = false;
	lowerCaseKeyStates["m"] = false;
	lowerCaseKeyStates["n"] = false;
	lowerCaseKeyStates["o"] = false;
	lowerCaseKeyStates["p"] = false;
	lowerCaseKeyStates["q"] = false;
	lowerCaseKeyStates["r"] = false;
	lowerCaseKeyStates["s"] = false;
	lowerCaseKeyStates["t"] = false;
	lowerCaseKeyStates["u"] = false;
	lowerCaseKeyStates["v"] = false;
	lowerCaseKeyStates["w"] = false;
	lowerCaseKeyStates["x"] = false;
	lowerCaseKeyStates["y"] = false;
	lowerCaseKeyStates["z"] = false;

	// Initialise the dictionary of the upper case keys
	upperCaseKeyStates["A"] = false;
	upperCaseKeyStates["B"] = false;
	upperCaseKeyStates["C"] = false;
	upperCaseKeyStates["D"] = false;
	upperCaseKeyStates["E"] = false;
	upperCaseKeyStates["F"] = false;
	upperCaseKeyStates["G"] = false;
	upperCaseKeyStates["H"] = false;
	upperCaseKeyStates["I"] = false;
	upperCaseKeyStates["J"] = false;
	upperCaseKeyStates["K"] = false;
	upperCaseKeyStates["L"] = false;
	upperCaseKeyStates["M"] = false;
	upperCaseKeyStates["N"] = false;
	upperCaseKeyStates["O"] = false;
	upperCaseKeyStates["P"] = false;
	upperCaseKeyStates["Q"] = false;
	upperCaseKeyStates["R"] = false;
	upperCaseKeyStates["S"] = false;
	upperCaseKeyStates["T"] = false;
	upperCaseKeyStates["U"] = false;
	upperCaseKeyStates["V"] = false;
	upperCaseKeyStates["W"] = false;
	upperCaseKeyStates["X"] = false;
	upperCaseKeyStates["Y"] = false;
	upperCaseKeyStates["Z"] = false;

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

	// Initialise the dictionary of the symbol keys
	specialKeyStates["Escape"] = false;
	specialKeyStates["Space"] = false;
}
Keyboard::~Keyboard() {

}

bool Keyboard::GetKeyState(const LowerCaseKeys& key) {
	switch (key) {
		case Keyboard::a:
			return lowerCaseKeyStates["a"];
		case Keyboard::b:
			return lowerCaseKeyStates["b"];
		case Keyboard::c:
			return lowerCaseKeyStates["c"];
		case Keyboard::d:
			return lowerCaseKeyStates["d"];
		case Keyboard::e:
			return lowerCaseKeyStates["e"];
		case Keyboard::f:
			return lowerCaseKeyStates["f"];
		case Keyboard::g:
			return lowerCaseKeyStates["g"];
		case Keyboard::h:
			return lowerCaseKeyStates["h"];
		case Keyboard::i:
			return lowerCaseKeyStates["i"];
		case Keyboard::j:
			return lowerCaseKeyStates["j"];
		case Keyboard::k:
			return lowerCaseKeyStates["k"];
		case Keyboard::l:
			return lowerCaseKeyStates["l"];
		case Keyboard::m:
			return lowerCaseKeyStates["m"];
		case Keyboard::n:
			return lowerCaseKeyStates["n"];
		case Keyboard::o:
			return lowerCaseKeyStates["o"];
		case Keyboard::p:
			return lowerCaseKeyStates["p"];
		case Keyboard::q:
			return lowerCaseKeyStates["q"];
		case Keyboard::r:
			return lowerCaseKeyStates["r"];
		case Keyboard::s:
			return lowerCaseKeyStates["s"];
		case Keyboard::t:
			return lowerCaseKeyStates["t"];
		case Keyboard::u:
			return lowerCaseKeyStates["u"];
		case Keyboard::v:
			return lowerCaseKeyStates["v"];
		case Keyboard::w:
			return lowerCaseKeyStates["w"];
		case Keyboard::x:
			return lowerCaseKeyStates["x"];
		case Keyboard::y:
			return lowerCaseKeyStates["y"];
		case Keyboard::z:
			return lowerCaseKeyStates["z"];
		default:
			return false;
	}
}
bool Keyboard::GetKeyState(const UpperCaseKeys& key) {
	switch (key) {
		case Keyboard::A:
			return upperCaseKeyStates["A"];
		case Keyboard::B:
			return upperCaseKeyStates["B"];
		case Keyboard::C:
			return upperCaseKeyStates["C"];
		case Keyboard::D:
			return upperCaseKeyStates["D"];
		case Keyboard::E:
			return upperCaseKeyStates["E"];
		case Keyboard::F:
			return upperCaseKeyStates["F"];
		case Keyboard::G:
			return upperCaseKeyStates["G"];
		case Keyboard::H:
			return upperCaseKeyStates["H"];
		case Keyboard::I:
			return upperCaseKeyStates["I"];
		case Keyboard::J:
			return upperCaseKeyStates["J"];
		case Keyboard::K:
			return upperCaseKeyStates["K"];
		case Keyboard::L:
			return upperCaseKeyStates["L"];
		case Keyboard::M:
			return upperCaseKeyStates["M"];
		case Keyboard::N:
			return upperCaseKeyStates["N"];
		case Keyboard::O:
			return upperCaseKeyStates["O"];
		case Keyboard::P:
			return upperCaseKeyStates["P"];
		case Keyboard::Q:
			return upperCaseKeyStates["Q"];
		case Keyboard::R:
			return upperCaseKeyStates["R"];
		case Keyboard::S:
			return upperCaseKeyStates["S"];
		case Keyboard::T:
			return upperCaseKeyStates["T"];
		case Keyboard::U:
			return upperCaseKeyStates["U"];
		case Keyboard::V:
			return upperCaseKeyStates["V"];
		case Keyboard::W:
			return upperCaseKeyStates["W"];
		case Keyboard::X:
			return upperCaseKeyStates["X"];
		case Keyboard::Y:
			return upperCaseKeyStates["Y"];
		case Keyboard::Z:
			return upperCaseKeyStates["Z"];
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
bool Keyboard::GetKeyState(const SpecialKeys& key) {
	switch (key) {
		case Keyboard::Escape:
			return specialKeyStates["Escape"];
		case Keyboard::Space:
			return specialKeyStates["Space"];
		default:
			return false;
	}
}
void Keyboard::UpdateKeyStates_Up(const SDL_KeyboardEvent & keyboardEvent) {
	if (!keyboardEvent.repeat) {
		switch (keyboardEvent.keysym.sym) {
			// Lower case Keys
			case SDLK_a:
				lowerCaseKeyStates["a"] = false;
				break;
			case SDLK_b:
				lowerCaseKeyStates["b"] = false;
				break;
			case SDLK_c:
				lowerCaseKeyStates["c"] = false;
				break;
			case SDLK_d:
				lowerCaseKeyStates["d"] = false;
				break;
			case SDLK_e:
				lowerCaseKeyStates["e"] = false;
				break;
			case SDLK_f:
				lowerCaseKeyStates["f"] = false;
				break;
			case SDLK_g:
				lowerCaseKeyStates["g"] = false;
				break;
			case SDLK_h:
				lowerCaseKeyStates["h"] = false;
				break;
			case SDLK_i:
				lowerCaseKeyStates["i"] = false;
				break;
			case SDLK_j:
				lowerCaseKeyStates["j"] = false;
				break;
			case SDLK_k:
				lowerCaseKeyStates["k"] = false;
				break;
			case SDLK_l:
				lowerCaseKeyStates["l"] = false;
				break;
			case SDLK_m:
				lowerCaseKeyStates["m"] = false;
				break;
			case SDLK_n:
				lowerCaseKeyStates["n"] = false;
				break;
			case SDLK_o:
				lowerCaseKeyStates["o"] = false;
				break;
			case SDLK_p:
				lowerCaseKeyStates["p"] = false;
				break;
			case SDLK_q:
				lowerCaseKeyStates["q"] = false;
				break;
			case SDLK_r:
				lowerCaseKeyStates["r"] = false;
				break;
			case SDLK_s:
				lowerCaseKeyStates["s"] = false;
				break;
			case SDLK_t:
				lowerCaseKeyStates["t"] = false;
				break;
			case SDLK_u:
				lowerCaseKeyStates["u"] = false;
				break;
			case SDLK_v:
				lowerCaseKeyStates["v"] = false;
				break;
			case SDLK_w:
				lowerCaseKeyStates["w"] = false;
				break;
			case SDLK_x:
				lowerCaseKeyStates["x"] = false;
				break;
			case SDLK_y:
				lowerCaseKeyStates["y"] = false;
				break;
			case SDLK_z:
				lowerCaseKeyStates["z"] = false;
				break;

			// Upper Case Keys

			// Special Keys
			case SDLK_ESCAPE:
				specialKeyStates["Escape"] = false;
				break;
			case SDLK_SPACE:
				specialKeyStates["Space"] = false;
				break;
			default:
				break;
		}
	}
}

void Keyboard::UpdateKeyStates_Down(const SDL_KeyboardEvent & keyboardEvent) {
	if (!keyboardEvent.repeat) {
		switch (keyboardEvent.keysym.sym) {
			// Lower Case Keys
			case SDLK_a:
				lowerCaseKeyStates["a"] = true;
				break;
			case SDLK_b:
				lowerCaseKeyStates["b"] = true;
				break;
			case SDLK_c:
				lowerCaseKeyStates["c"] = true;
				break;
			case SDLK_d:
				lowerCaseKeyStates["d"] = true;
				break;
			case SDLK_e:
				lowerCaseKeyStates["e"] = true;
				break;
			case SDLK_f:
				lowerCaseKeyStates["f"] = true;
				break;
			case SDLK_g:
				lowerCaseKeyStates["g"] = true;
				break;
			case SDLK_h:
				lowerCaseKeyStates["h"] = true;
				break;
			case SDLK_i:
				lowerCaseKeyStates["i"] = true;
				break;
			case SDLK_j:
				lowerCaseKeyStates["j"] = true;
				break;
			case SDLK_k:
				lowerCaseKeyStates["k"] = true;
				break;
			case SDLK_l:
				lowerCaseKeyStates["l"] = true;
				break;
			case SDLK_m:
				lowerCaseKeyStates["m"] = true;
				break;
			case SDLK_n:
				lowerCaseKeyStates["n"] = true;
				break;
			case SDLK_o:
				lowerCaseKeyStates["o"] = true;
				break;
			case SDLK_p:
				lowerCaseKeyStates["p"] = true;
				break;
			case SDLK_q:
				lowerCaseKeyStates["q"] = true;
				break;
			case SDLK_r:
				lowerCaseKeyStates["r"] = true;
				break;
			case SDLK_s:
				lowerCaseKeyStates["s"] = true;
				break;
			case SDLK_t:
				lowerCaseKeyStates["t"] = true;
				break;
			case SDLK_u:
				lowerCaseKeyStates["u"] = true;
				break;
			case SDLK_v:
				lowerCaseKeyStates["v"] = true;
				break;
			case SDLK_w:
				lowerCaseKeyStates["w"] = true;
				break;
			case SDLK_x:
				lowerCaseKeyStates["x"] = true;
				break;
			case SDLK_y:
				lowerCaseKeyStates["y"] = true;
				break;
			case SDLK_z:
				lowerCaseKeyStates["z"] = true;
				break;

			// Upper Case Keys

			// Special Keys
			case SDLK_ESCAPE:
				specialKeyStates["Escape"] = true;
				break;
			case SDLK_SPACE:
				specialKeyStates["Space"] = true;
				break;
			default:
				break;
		}
	}
}
