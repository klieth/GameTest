#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <GL/glew.h>

class GameState {
	public:
		static bool forward;
		static bool backward;
		static bool left;
		static bool right;
		static bool up;
		static bool down;
		static GLuint gWorldLocation;
};

#endif
