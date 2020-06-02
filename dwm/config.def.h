/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 18;       /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* Enable follow window on tag switch */
static const Bool viewontag         = True;

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };
static const char *mutecmd[] = { "/home/oskari/.scripts/toggle_audio", NULL };
static const char *volupcmd[] = { "amixer", "-q", "set", "Master", "5%+", "unmute", NULL };
static const char *voldowncmd[] = { "amixer", "-q", "set", "Master", "5%-", "unmute", NULL };
static const char *miccmd[] = { "amixer", "set", "Capture", "toggle", NULL };
static const char *brupcmd[] = { "sudo", "xbacklight", "-inc", "10", NULL };
static const char *brdowncmd[] = { "sudo", "xbacklight", "-dec", "10", NULL };
static const char *rangercmd[] = { "st", "ranger", NULL };
static const char *firefoxcmd[] = { "firefox", NULL };

#include "push.c"

static Key keys[] = {
	/* modifier             key    function        argument */
	{ MODKEY,               33,    spawn,          {.v = dmenucmd } },         // p
	{ MODKEY|ShiftMask,     36,    spawn,          {.v = termcmd } },          // Return
	{ MODKEY,               49,    togglescratch,  {.v = scratchpadcmd } },    // Grave
	{ MODKEY,               56,    togglebar,      {0} },                      // b
	{ MODKEY,               44,    focusstack,     {.i = +1 } },               // j
	{ MODKEY,               45,    focusstack,     {.i = -1 } },               // k
	{ MODKEY,               31,    incnmaster,     {.i = +1 } },               // i
	{ MODKEY,               40,    incnmaster,     {.i = -1 } },               // d
	{ MODKEY,               43,    setmfact,       {.f = -0.05} },             // h
	{ MODKEY,               46,    setmfact,       {.f = +0.05} },             // l
	{ MODKEY,               36,    zoom,           {0} },                      // Return
	{ MODKEY,               23,    view,           {0} },                      // Tab
	{ MODKEY|ShiftMask,     54,    killclient,     {0} },                      // c
	{ MODKEY,               28,    setlayout,      {.v = &layouts[0]} },       // t
	{ MODKEY,               41,    setlayout,      {.v = &layouts[1]} },       // f
	{ MODKEY,               58,    setlayout,      {.v = &layouts[2]} },       // m
	{ MODKEY,               30,    setlayout,      {.v = &layouts[3]} },       // u
	{ MODKEY,               32,    setlayout,      {.v = &layouts[4]} },       // o
	{ MODKEY|ShiftMask,     65,    togglefloating, {0} },                      // space
	{ MODKEY,               39,    togglesticky,   {0} },                      // s
	{ MODKEY,               19,    view,           {.ui = ~0 } },              // 0
	{ MODKEY|ShiftMask,     19,    tag,            {.ui = ~0 } },              // 0
	{ MODKEY,               59,    focusmon,       {.i = -1 } },               // comma
	{ MODKEY,               60,    focusmon,       {.i = +1 } },               // period
	{ MODKEY|ShiftMask,     59,    tagmon,         {.i = -1 } },               // comma
	{ MODKEY|ShiftMask,     60,    tagmon,         {.i = +1 } },               // period
	{ MODKEY|ControlMask,   44,    pushdown,       {0} },                      // j
	{ MODKEY|ControlMask,   45,    pushup,         {0} },                      // k
	{ MODKEY|ShiftMask,     27,    spawn,          {.v = rangercmd} },         // r
	{ MODKEY|ShiftMask,     41,    spawn,          {.v = firefoxcmd} },        // f
	{ 0,                    121,   spawn,          {.v = mutecmd } },          // Mute
	{ 0,                    122,   spawn,          {.v = voldowncmd } },       // Audio lower
	{ 0,                    123,   spawn,          {.v = volupcmd } },         // Audio higher
	{ 0,                    198,   spawn,          {.v = miccmd} },            // Mic toggle
	{ 0,                    233,   spawn,          {.v = brupcmd} },           // Brightness up
	{ 0,                    232,   spawn,          {.v = brdowncmd} },         // Brightness down
	TAGKEYS(                10,                    0)                          // 1
	TAGKEYS(                11,                    1)                          // 2
	TAGKEYS(                12,                    2)                          // 3
	TAGKEYS(                13,                    3)                          // 4
	TAGKEYS(                14,                    4)                          // 5
	TAGKEYS(                15,                    5)                          // 6
	TAGKEYS(                16,                    6)                          // 7
	TAGKEYS(                17,                    7)                          // 8
	TAGKEYS(                18,                    8)                          // 9
	{ MODKEY|ShiftMask,             24,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, 24,      quit,           {1} }, 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

