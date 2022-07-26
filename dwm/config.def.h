// theme
#include "themes/lollypop.h"
// apps
#include "apps.h"
// Media buttons
#include <X11/XF86keysym.h>

/* appearance */
//static const int user_bh = 1;                   /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int default_border = 1;  // to switch back to default border after dynamic border resizing via keybinds
static const unsigned int snap      = 1;        /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 1;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails,display systray on the 1st monitor,False: display systray on last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always };
static const int showtab            = showtab_auto;
static const int toptab             = True;
static const int topbar             = 1;        /* 0 means bottom bar */
static const int horizpadbar        = 1;
static const int vertpadbar         = 20;
static const int vertpadtab         = 40;
static const int horizpadtabi       = 20;
static const int horizpadtabo       = 20;
static const int scalepreview       = 40;
static       int tag_preview        = 1;        /* 1 means enable, 0 is off */

static const char *fonts[]          = { "consolas:style:medium:size=8",
                                        "Material Design Icons-Regular:size=10",
                                      };
static const char dmenufont[]       = "NotoSans Nerd Font:style=Medium:size=8";
static const int colorfultag        = 1;  /* 0 means use SchemeSel for selected non vacant tag */

static const char *colors[][3]      = {
    /*                      fg     bg    border */
    [SchemeNorm]       = { white, black, green },
    [SchemeSel]        = { black, blue,  pink  },
    [TabSel]           = { black, blue,  black },
    [TabNorm]          = { white, black, black },
    [SchemeTag]        = { white, black, black },
    [SchemeTag1]       = { blue,  black, black },
    [SchemeTag2]       = { red,   black, black },
    [SchemeTag3]       = { orange, black,black },
    [SchemeTag4]       = { green, black, black },
    [SchemeTag5]       = { pink,  black, black },
    [SchemeLayout]     = { white, blue, black }, 
    [SchemeBtnPrev]    = { green, black, black }, 
    [SchemeBtnNext]    = { yellow, black, black }, 
    [SchemeBtnClose]   = { red, black, black }, 
};

/* tagging */
// static char *tags[] = {"A", "B", "C", "D", "E"};
static char *tags[] = {"Terminal", "Editor", "Web", "Chat", "Music"};
// static char *tags[] = {" 🯱 ", " 🯲 ", " 🯳 ", " 🯴 ", " 🯵 "};

static const int tagschemes[] = { SchemeTag1, SchemeTag2, SchemeTag3,
                                  SchemeTag4, SchemeTag5,
                                };

static const unsigned int ulinepad  = 5;    /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke   = 2;    /* thickness / height of the underline */
static const unsigned int ulinevoffset  = 0;    /* how far above the bottom of the bar the line should appear */
static const int ulineall       = 0;    /* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class      instance      title       tags mask     iscentered   isfloating   monitor */
    { "Nitrogen",   NULL,       NULL,       0,            0,           1,           -1 },
    { "Feh",        NULL,       NULL,       1 << 8,       0,           0,           -1 },
    { "Viewnior",   NULL,       NULL,       0,            0,           1,           -1 },
    { "MEGAsync",   NULL,       NULL,       0,            0,           1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "functions.h"


static const Layout layouts[] = {
    /* symbol     arrange function */
    { "Tile",      tile },    /* first entry is default */
    { "Monocle",   monocle },
    { "Spiral",    spiral },
    { "Dwindle",   dwindle },
    { "Deck",      deck },
    { "Bstack",    bstack },
    { "Bstackhoriz",    bstackhoriz },
    { "Grid",           grid },
    { "Nrowgrid",       nrowgrid },
    { "Horizgrid",      horizgrid },
    { "Gaplessgrid",    gaplessgrid },
    { "Centeredmaster", centeredmaster },
    { "Centeredfloatingmaster",      centeredfloatingmaster },
    { "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c" ,  cmd, NULL } }

static Key keys[] = {
    { MODKEY,                                 XK_Return, spawn,          {.v = termcmd }},
    { MODKEY|ShiftMask,                       XK_s, spawn,               {.v = editorcmd}},
    { MODKEY|ShiftMask,                       XK_c, spawn,               {.v = browsercmd}},
    { MODKEY|ShiftMask,                       XK_n, spawn,               {.v = filecmd}},
    { MODKEY|ShiftMask,                       XK_t, spawn,               {.v = chatcmd}},
    { MODKEY|ShiftMask, XK_m, spawn, SHCMD("rofi -show drun")},
    { MODKEY|ShiftMask, XK_o, spawn, SHCMD("sh ~/.config/rofi/bin/powermenu")},


    { 0,        XF86XK_AudioRaiseVolume,    spawn,      {.v = upvol } },
    { 0,        XF86XK_AudioLowerVolume,    spawn,      {.v = downvol } },
    { 0,        XF86XK_AudioMute,           spawn,      {.v = mute } },
    { 0,        XF86XK_AudioMicMute,        spawn,      {.v = micmute } },
        
    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY|ControlMask,                       XK_w,      tabmode,        { -1 } },
    { MODKEY,                       XK_Right,      focusstack,     {.i = +1 } },
    { MODKEY,                       XK_Left,      focusstack,     {.i = -1 } },
    { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
    { MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
    { MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
    { MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
    { MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
    { MODKEY|ControlMask,                       XK_Return, zoom,           {0} },
    { MODKEY,                       XK_Tab,    view,           {0} },

    // overall gaps
    { MODKEY|ControlMask,           XK_i,      incrgaps,       {.i = +1 } },
    { MODKEY|ControlMask,           XK_d,      incrgaps,       {.i = -1 } },

    // inner gaps
    { MODKEY|ShiftMask,                XK_i,      incrigaps,      {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },

    // outer gaps
    { MODKEY|ControlMask,              XK_o,      incrogaps,      {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },

    { MODKEY|ControlMask,              XK_6,      incrihgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
    { MODKEY|ControlMask,              XK_7,      incrivgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
    { MODKEY|ControlMask,              XK_8,      incrohgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
    { MODKEY|ControlMask,              XK_9,      incrovgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },

    { MODKEY|ControlMask,           XK_t,      togglegaps,     {0} },
    { MODKEY|ControlMask|ShiftMask,             XK_d,      defaultgaps,    {0} },

    { MODKEY,                       XK_q,      killclient,     {0} },
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
    { MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
    { MODKEY|ControlMask,           XK_g,      setlayout,      {.v = &layouts[10]} },
    { MODKEY|ControlMask|ShiftMask, XK_t,      setlayout,      {.v = &layouts[13]} },
    { MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY|ControlMask,       XK_comma,  cyclelayout,    {.i = -1 } },
    { MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY,                       XK_f,      togglefullscr,  {0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { MODKEY|ShiftMask,             XK_minus,       setborderpx,    {.i = -1 } },
    { MODKEY|ShiftMask,             XK_p,           setborderpx,    {.i = +1 } },
    { MODKEY|ShiftMask,             XK_w,           setborderpx,    {.i = default_border } },

    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)
    {
    
    MODKEY|ShiftMask,               XK_e,      quit,           {0}
    
    },
    { MODKEY|ShiftMask,             XK_r,      quit,           {1} },
    { MODKEY,                       XK_e,      hidewin,        {0} },
    { MODKEY|ShiftMask,             XK_e,      restorewin,     {0} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },

        /* Keep movemouse? */
    /* { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} }, */

        /* placemouse options, choose which feels more natural:
         *    0 - tiled position is relative to mouse cursor
         *    1 - tiled postiion is relative to window center
         *    2 - mouse pointer warps to window center
         *
         * The moveorplace uses movemouse or placemouse depending on the floating state
         * of the selected client. Set up individual keybindings for the two if you want
         * to control these separately (i.e. to retain the feature to move a tiled window
         * into a floating position).
         */
    { ClkClientWin,         MODKEY,         Button1,        moveorplace,    {.i = 0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,     {0} },
    { ClkClientWin,         ControlMask,    Button1,        dragmfact,      {0} },
    { ClkClientWin,         ControlMask,    Button3,        dragcfact,      {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkTabBar,            0,              Button1,        focuswin,       {0} },
    { ClkTabBar,            0,              Button1,        focuswin,       {0} },
    { ClkTabPrev,           0,              Button1,        movestack,      { .i = -1 } },
    { ClkTabNext,           0,              Button1,        movestack,      { .i = +1 } },
    { ClkTabClose,          0,              Button1,        killclient,     {0} },
};
