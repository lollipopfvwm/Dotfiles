/* DEFAULT APPLICATIONS */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = {  "kitty", NULL }; // change this to your term
static const char *editorcmd[] = {"subl", NULL}; // your text editor
static const char *filecmd[] = {"nemo", NULL}; // your file manager
static const char *chatcmd[] = {"telegram-desktop", NULL}; // your chat application
static const char *browsercmd[] = {"google-chrome-stable", NULL}; // your text web browser

/* VOLUME */
static const char *upvol[] = { "amixer", "-q", "sset", "Master", "3%+", NULL };
static const char *downvol[] = { "amixer", "-q", "sset", "Master", "3%-", NULL };
static const char *mute[] = { "amixer", "-q", "sset", "Master", "toggle", NULL };
static const char *micmute[] = { "amixer", "-q", "sset", "Capture", "toggle", NULL };