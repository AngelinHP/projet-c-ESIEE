// Key management

void GXinternal_resetKeyPressed();
void GXinternal_initKeys();
void GXinternal_keyboard(unsigned char key, int x, int y);
void GXinternal_keyUp(unsigned char key, int x, int y);
void GXinternal_keySpecial(int key, int x, int y);
void GXinternal_keySpecialUp(int key, int x, int y);


// Mouse management
void GXinternal_mouseMove(int x, int y);
void GXinternal_mouse(int button, int state, int x, int y);

// Window size
int GXinternal_getWindowWidth();
int GXinternal_getWindowHeight();

//Anim 
void GXanim(int value);