// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "map.h"
#include <stdlib.h>
#include <time.h>
#include <random>
#include "entity.h"
#include "fire.h"
#include "player.h"
#include "virus.h"
#include "virus_spawner.h"
#include "NPC.h"
#include "projectile.h"
#include "boulder.h"
#include "lightning.h"
#include "boss.h"

bool played = PlaySound(TEXT(""), NULL, SND_ASYNC); // plays background music
bool toiletpaperbuff = false;
double  g_dElapsedTime;
double  g_dDeltaTime;
double  g_dbufftime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;
int maplevel = 1;
// Game specific variables here
EGAMESTATES g_eGameState = S_MAIN; // initial state
int MAPSIZEX = 200;
int MAPSIZEY = 200;
map g_map = map(MAPSIZEX, MAPSIZEY, position(0,0), position(80, 25));
map bg_map = map(MAPSIZEX, MAPSIZEY, position(0, 0), position(80, 25)); //background only
map bgc_map = map(MAPSIZEX, MAPSIZEY, position(0, 0), position(80, 25)); //background characters
map fg_map = map(MAPSIZEX, MAPSIZEY, position(0, 0), position(80, 25)); //foreground map
image wallskin = image(NULL, 16); //image/skin of the walls
float virusspawntime;
float virusspawntimer;
float updatetime = 0.05f;
float updatetimer = 0;
float lightningtimer = 0;
const int MAXENTITY = 500;
image nonbuffplayerskin = image(1, 0);


entity* entities[MAXENTITY]; //stores all entities that move
image previmg; //the img under the player
WORD solids[] = {240, 2+80, 8}; //list of solid objects that will stop movement, add the colour here
/*current list: 240  =   white  = walls
                2+80 =  bg:purple fg:green = virus_spawner
                8    =  bg:NULL   fg:grey  = boulder (barricade from protesting)
*/
bool mouse_tooltip_enabled;
player* g_player;
// Console object
Console g_Console(80, 25, "Mask of Yendor");

const int MAXTOOLS = 5;
string tools[MAXTOOLS];
int currenttool = 0;
//background random generation
//nature bgc
image bgc_images_nature[] = { image(NULL, 2), image(-17, 2), image('*', 15), image('*', 12), image('*', 14), image('\\', 6) };
int bgc_weightage_nature[] = { 80,        80,              1,              1,                1      ,         1 };
//nature bg
image bg_images_nature[] = { image(NULL, 160) };
int bg_weightage_nature[] = { 1 };
//outer-space bgc
image bgc_images_space[] = {image(NULL, 0), image('+', 15), image('*', 15)};
int bgc_weightage_space[] = {80    ,              1  ,              1 };
//outer-space bg
image bg_images_space[] = { image(NULL, 0) };
int bg_weightage_space[] = { 1 };
//white bg
image bg_images_white[] = { image(NULL, 240) };
int bg_weightage_white[] = { 1 };
//yellow bg
image bg_images_yellow[] = { image(NULL, 224) };
int bg_weightage_yellow[] = { 1 };
//black bg
image bg_images_black[] = { image(NULL, 0) };
int bg_weightage_black[] = { 1 };
//brown bg
image bg_images_brown[] = { image(NULL, 96) };
int bg_weightage_brown[] = { 1 };
//debugging things
float debugtext; //will be rendered at mousepos

//others
int idx[MAXENTITY]; //used for collision detection

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init(void)
{
    //init variables
    srand(time(NULL));
    
    virusspawntime = 1;
    mouse_tooltip_enabled = true;
    for (int i = 0; i < MAXENTITY; i++)
    {
        entities[i] = NULL;
    }
    entities[0] = new player(position(190, 30), 5, 0.05f, image(1, 0));
    g_player = static_cast<player*>(entities[0]);
    mapchange(maplevel);
    //init maps
    renderWall(); //creating the border walls
    // Setting attributes of player
    previmg = image(NULL, 0);
    for (int i = 0; i < MAXTOOLS; i++)
    {
        tools[i] = "none";
    }
    tools[0] = "fists";
    // Set precision for floating point output
    //g_dElapsedTime = 3600.0;    // Susceptible to change

    // sets the initial state for the game
    g_eGameState = S_MAIN;

    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler);

    //debugging things
    for (int i = 0; i < MAXENTITY; i++)
    {
        if (entities[i] == NULL)
        {
            break;
        }
    }
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Get all the console input events
//            This function sets up the keyboard and mouse input from the console.
//            We will need to reset all the keyboard status, because no events will be sent if no keys are pressed.
//
//            Remember to set the handlers for keyboard and mouse events.
//            The function prototype of the handler is a function that takes in a const reference to the event struct
//            and returns nothing. 
//            void pfKeyboardHandler(const KEY_EVENT_RECORD&);
//            void pfMouseHandlerconst MOUSE_EVENT_RECORD&);
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{
    // resets all the keyboard events
    //memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    // then call the console to detect input from user
    g_Console.readConsoleInput();    
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the keyboard input. Whenever there is a keyboard event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            The KEY_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any keyboard event in the Splashscreen state
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent - reference to a key event struct
// Output   : void
//--------------------------------------------------------------
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: // don't handle anything for the splash screen
        break;
    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    case S_PAUSE:pausekeyboardHandler(keyboardEvent);
        break;
    case S_MAIN: // nothing
        break;
    case S_OVER:gameplayKBHandler(keyboardEvent);
        break;
    case S_TUTORIAL:gameplayKBHandler(keyboardEvent);
        break;
    case S_MAPT:gameplayKBHandler(keyboardEvent);
        break;
    case S_WIN:gameplayKBHandler(keyboardEvent);
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the mouse input. Whenever there is a mouse event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            For the mouse event, if the mouse is not moved, no event will be sent, hence you should not reset the mouse status.
//            However, if the mouse goes out of the window, you would not be able to know either. 
//
//            The MOUSE_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any mouse event in the Splashscreen state
//            
// Input    : const MOUSE_EVENT_RECORD& mouseEvent - reference to a mouse event struct
// Output   : void
//--------------------------------------------------------------
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: // don't handle anything for the splash screen
        break;
    case S_GAME: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    case S_PAUSE:gameplayMouseHandler(mouseEvent);
        break;
    case S_RESTART:Restart();
        break;
    case S_MAIN:gameplayMouseHandler(mouseEvent);
        break;
    case S_OVER: gameplayMouseHandler(mouseEvent);
        break;
    case S_TUTORIAL: gameplayMouseHandler(mouseEvent);
        break;
    case S_MAPT: gameplayMouseHandler(mouseEvent);
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the keyboard handler in the game state. Whenever there is a keyboard event in the game state, this function will be called.
//            
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case VK_UP: key = K_UP; break;
    case VK_DOWN: key = K_DOWN; break;
    case VK_LEFT: key = K_LEFT; break;
    case VK_RIGHT: key = K_RIGHT; break;
    case VK_SPACE: key = K_SPACE; break;
    case VK_ESCAPE: key = K_ESCAPE; break;
    case VK_RETURN: key = K_ENTER; break;
        //WASD cases
    case 0x57: key = K_W; break;
    case 0x41: key = K_A; break;
    case 0x53: key = K_S; break;
    case 0x44: key = K_D; break;
        //others
    case 0x54: key = K_T; break;
    case 0x45: key = K_E; break;
    case 0x51: key = K_Q; break;
        //numbers
    case 0x30: key = K_0; break;
    case 0x31: key = K_1; break;
    case 0x32: key = K_2; break;
    case 0x33: key = K_3; break;
    case 0x34: key = K_4; break;
    case 0x35: key = K_5; break;
    }
    // a key pressed event would be one with bKeyDown == true
    // a key released event would be one with bKeyDown == false
    // if no key is pressed, no event would be fired.
    // so we are tracking if a key is either pressed, or released
    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the game state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

void pausekeyboardHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case VK_ESCAPE: key = K_ESCAPE; break;
        //WASD cases
    case 0x57: key = K_W; break;
    case 0x41: key = K_A; break;
    case 0x53: key = K_S; break;
    case 0x44: key = K_D; break;
    }
    // a key pressed event would be one with bKeyDown == true
    // a key released event would be one with bKeyDown == false
    // if no key is pressed, no event would be fired.
    // so we are tracking if a key is either pressed, or released
    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }
}


//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{

    g_dDeltaTime = dt;
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: splashScreenWait(); // game logic for the splash screen
        break;
    case S_GAME:
        updateGame(dt); // gameplay logic when we are in the game
        break;
    case S_PAUSE:
        updatePause();
        break;
    case S_RESTART:Restart();
        break;
    case S_MAIN: mainMenu();
        break;
    case S_OVER: renderOver();
        break;
    }

    if (g_dElapsedTime <= 0)
    {
        g_eGameState = S_OVER; //Once timer hits 0, Game Over
    }
}


void splashScreenWait()    // waits for time to pass in splash screen
{
    g_eGameState = S_GAME;
}

void updateGame(double dt)       // gameplay logic
{
    //timer related things
    //increasing spawn timer for virus
    //virusspawntimer += dt; uncomment out for random virus spawning instead of using spawners
    // get the delta time
    g_dElapsedTime -= dt;
    updatetimer += dt;

    if (lightningtimer < 1)
    {
        lightningtimer += dt;
    }
    if (g_dbufftime < 5.0)
    {
        g_dbufftime += dt;
    }
    if (g_dbufftime >= 5.0)
    {
        g_player->setimage(nonbuffplayerskin);
        toiletpaperbuff = false;
    }
    // increasing timers for entities

    for (int i = 0; i < MAXENTITY; i++)
    {
        if (entities[i] != NULL)
        {
            entities[i]->updatetimers(dt);
        }
    }

    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    //updating things based on delta time for entities
    if (updatetimer >= updatetime)
    {
       
        updatetimer = 0;
        for (int i = 0; i < MAXENTITY; i++)
        {
            if (entities[i] != NULL)
            {
                entities[i]->update(g_map, bg_map, bgc_map, fg_map, g_player);
            }
        }
    }
    //movement for entities
    for (int i = 0; i < MAXENTITY; i++)
    {
        if (entities[i] != NULL)
        {
            if (entities[i]->getspdtimer() >= entities[i]->getspd())
            {
                //entities[0] will always be the player
                if (i == 0)
                {
                    entities[i]->setspdtimer(0);
                    moveCharacter();
                }
                else
                {
                    entities[i]->setspdtimer(0);
                    entities[i]->move(g_map, bg_map, bgc_map, solids, size(solids), entities, MAXENTITY);
                }
            }
        }
    }
    //checking for deletion
    for (int i = 1; i < MAXENTITY; i++) //player is not part of this
    {
        if (entities[i] != NULL && entities[i]->gethp() <= 0)
        {
            entities[i]->die(g_map, bg_map, bgc_map);
            entities[i] = NULL;
        }
    }
    //spawning viruses
    if (virusspawntimer >= virusspawntime)
    {
        virusspawntimer = 0;
        virusspawntime = rand() % 2;
        spawnvirus();
    }
}

void moveCharacter()
{
   
    // Updating the location of the character based on the key down
    position futurloc = position(entities[0]->getpos().get('x'), entities[0]->getpos().get('y'));
    if (g_skKeyEvent[K_W].keyDown)
    {
        futurloc.set('y', futurloc.get('y') - 1);
    }
    if (g_skKeyEvent[K_A].keyDown)
    {
        futurloc.set('x', futurloc.get('x') - 1);
    }
    if (g_skKeyEvent[K_S].keyDown)
    {
        futurloc.set('y', futurloc.get('y') + 1);
    }
    if (g_skKeyEvent[K_D].keyDown)
    {
        futurloc.set('x', futurloc.get('x') + 1);  
    }
    //collision detection for solids
    for (int i = 0; i < size(solids); i++)
    {
        if (static_cast<WORD>(g_map.getmapposition(futurloc).getcolour()) == static_cast<WORD>(solids[i]))
        {
            if (g_map.getmapposition(futurloc).getcolour() == static_cast<WORD>(8))
            {
                getentityfrompos(idx, futurloc, g_map);
                entities[idx[0]]->move(g_map, bg_map, bgc_map, solids, size(solids), entities, MAXENTITY);
            }
            futurloc = position(entities[0]->getpos().get('x'), entities[0]->getpos().get('y'));
            break;
        }
    }
    //trigger detection for g_map
    WORD g_mapcolour = static_cast<WORD>(g_map.getmapposition(futurloc).getcolour());
    if (g_mapcolour == static_cast<WORD>(0x0C)) //mask
    {
        maplevel++;
        maskrenderout();
        mapchange(maplevel);
        futurloc = entities[0]->getpos();
    }
    else if (g_mapcolour == static_cast<WORD>(0x0D)) //TP
    {
        toiletpaperbuff = true;
        g_dbufftime = 0.0;
        g_player->setimage(image(1, 14 + 128));
        maskrenderout();
    }
    else if (g_mapcolour == static_cast<WORD>(213)) //virus
    {
        getentityfrompos(idx, futurloc, g_map);
        if (idx[0] != -1)
        {
            for (int i = 0; i < MAXENTITY; i++)
            {
                if (idx == NULL)
                {
                    //do nothing
                }
                else
                {
                    if (entities[idx[i]]->getname() == "virus")
                    {
                        entities[idx[i]]->sethp(0);
                        if (toiletpaperbuff == true)
                        {
                            g_player->takedmg(0);
                        }
                        else
                        {
                            g_player->takedmg(1);
                        }

                        if (g_player->gethp() < 1)
                        {
                            g_eGameState = S_OVER;
                        }
                        //TODO other negative effects
                    }
                }
            }
        }
    }
    else if (g_mapcolour == static_cast<WORD>(241)) //Water Gun
    {
        for (int i = 0; i < MAXTOOLS; i++)
        {
            if (tools[i] == "none")
            {
                tools[i] = "Water Gun";
                currenttool = i;
                break;
            }
        }
    }
    //trigger detection for bgc_map
    char bgc_map_char = bgc_map.getmapposition(futurloc).gettext();
    if (bgc_map_char == -21) //fire
    {
        g_player->takedmg(1);
    }
    //"rendering"
    position prevloc = entities[0]->getpos();
    g_map.setmapposition(prevloc, previmg);
    previmg = image(NULL, 0);
    //previmg = g_map.getmapposition(futurloc);
    entities[0]->setpos(futurloc, g_map);

    //changing symbol of player
    if (g_skKeyEvent[K_SPACE].keyReleased)
    {
        entities[0]->setimage(image(entities[0]->getimage().gettext() + 1, entities[0]->getimage().getcolour()));
        debugtext = entities[0]->getimage().gettext();
        // resets all the keyboard events(add this to all buttons meant to be triggered from releasing and instead of being held down)
        g_skKeyEvent[K_SPACE].keyDown = 0;
        g_skKeyEvent[K_SPACE].keyReleased = 0;
    }
    //toggling of mouse over tooltips
    if (g_skKeyEvent[K_T].keyReleased)
    {
        mouse_tooltip_enabled = !mouse_tooltip_enabled;
        g_skKeyEvent[K_T].keyDown = 0;
        g_skKeyEvent[K_T].keyReleased = 0;
    }
    //changing tools
    if (g_skKeyEvent[K_Q].keyReleased)
    {
        currenttool -= 1;
        if (currenttool == -1)
        {
            currenttool = MAXTOOLS-1;
        }
        if (tools[currenttool] == "none")
        {
            currenttool += 1;
        }
        if (currenttool > MAXTOOLS-1)
        {
            currenttool = 0;
        }
        g_skKeyEvent[K_Q].keyReleased = 0;
        g_skKeyEvent[K_Q].keyDown = 0;
    }
    if (g_skKeyEvent[K_E].keyReleased)
    {
        currenttool += 1;
        if (currenttool > MAXTOOLS-1)
        {
            currenttool = 0;
        }
        if (tools[currenttool] == "none")
        {
            currenttool -= 1;
        }
        if (currenttool == -1)
        {
            currenttool = MAXTOOLS-1;
        }
        g_skKeyEvent[K_E].keyReleased = 0;
        g_skKeyEvent[K_E].keyDown = 0;
    }
    //using current tool
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        //clicking on NPCs make them not talk/talk
        getentityfrompos(idx, position(g_mouseEvent.mousePosition.X + g_map.getcampos().get('x'), g_mouseEvent.mousePosition.Y + g_map.getcampos().get('y')), g_map);
        if (idx[0] != -1)
        {
            for (int i = 0; i < MAXENTITY; i++)
            {
                if (entities[idx[i]]->getimage().gettext() == static_cast<char>(2))
                {
                    static_cast<NPC*>(entities[idx[i]])->toggletext();
                }
            }
            g_mouseEvent.buttonState = 0;
        }
        if (tools[currenttool] == "Water Gun")
        {
            if (g_player->getfireratetimer() >= g_player->getfirerate())
            {
                g_player->setfireratetimer(0);
                for (int i = 0; i < MAXENTITY; i++)
                {
                    if (entities[i] == NULL)
                    {
                        entities[i] = new projectile(g_player->getpos(), position(g_mouseEvent.mousePosition.X + g_map.getcampos().get('x'), g_mouseEvent.mousePosition.Y + g_map.getcampos().get('y')), image(7, 11), 0.1f, "water balloon", g_map, "fire", 3, image(NULL, 144));
                        break;
                    }
                }
            }
        }
    }
}

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
    {
        memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
        g_eGameState = S_PAUSE;
    }
}

//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN:renderSplashScreen();
        break;
    case S_MAIN:mainMenu();
        break;
    case S_GAME: renderGame();
        break;
    case S_PAUSE:renderPause();
        break;
    case S_OVER:renderOver();
        break;
    case S_TUTORIAL:rendertutorialscreen();
        break;
    case S_MAPT:renderTrans();
        break;
    case S_WIN:renderWin();
        break;
    }
    renderFramerate();//also renders hud      // renders debug information, frame rate, elapsed time, etc
    renderInputEvents();    // renders status of input events
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(32);
}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void renderSplashScreen()  // renders the splash screen
{
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 9;
    g_Console.writeToBuffer(c, "A game in 3 seconds", 0x03);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(c, "Press <Space> to change character colour", 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x09);
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderMask();
    renderTP();
}

void renderMap()
{
    //delete this later, keep for reference on how the code works
    // Set up sample colours, and output shadings
    //const WORD colors[] = {
    //    0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
    //    0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    //};

    //COORD c;
    //for (int i = 0; i < 12; ++i)
    //{
    //    c.X = 5 * i;
    //    c.Y = i + 1;
    //    colour(colors[i]);
    //    g_Console.writeToBuffer(c, " °±²Û", colors[i]);
    //}
    
    //rendering the maps
    COORD c;
    if (maplevel != 5)
    {
        g_map.centerOnPlayer(entities[0]->getpos());
    }
    else
    {
        g_map.centerOnPlayer(position(39, 12));
    }
    int camposx = g_map.getcampos().get('x');
    int camsizex = g_map.getcamsize().get('x');
    int camposy = g_map.getcampos().get('y');
    int camsizey = g_map.getcamsize().get('y');
    WORD g_image_colour = 0;
    WORD bg_image_colour = 0;
    WORD bgc_image_colour = 0;
    for (int x = camposx, x0 = 0; x < camposx + camsizex; x++, x0++)
    {
        for (int y = camposy, y0 = 0; y < camposy + camsizey; y++, y0++)
        {
            if (x >= 0 && y >= 0 && x < g_map.getmapsize('x') && y <= g_map.getmapsize('y'))
            {
                c.X = x0;
                c.Y = y0;
                bg_image_colour = bg_map.getmapposition(position(x, y)).getcolour();
                
                
                //rendering fg_map
                if (fg_map.getmapposition(position(x, y)).gettext() != NULL || fg_map.getmapposition(position(x, y)).getcolour() != 0)
                {
                    g_Console.writeToBuffer(c, fg_map.getmapposition(position(x, y)).gettext(), fg_map.getmapposition(position(x, y)).getcolour());
                }
                else
                {
                    if (g_map.getmapposition(position(x, y)).gettext() != NULL || g_map.getmapposition(position(x, y)).getcolour() != 0)
                    {
                        //rendering wallskin map
                        g_image_colour = g_map.getmapposition(position(x, y)).getcolour();
                        if (g_image_colour == 240)
                        {
                            g_Console.writeToBuffer(c, wallskin.gettext(), wallskin.getcolour());
                        }
                        //rendering g_map
                        else if (g_image_colour > 15)
                        {
                            g_Console.writeToBuffer(c, g_map.getmapposition(position(x, y)).gettext(), g_image_colour);
                        }
                        else
                        {
                            g_Console.writeToBuffer(c, g_map.getmapposition(position(x, y)).gettext(), g_image_colour + bg_image_colour);
                        }
                    }
                    //rendering bgc_map
                    else
                    {
                        bgc_image_colour = bgc_map.getmapposition(position(x, y)).getcolour();
                        if (bgc_image_colour > 15)
                        {
                            g_Console.writeToBuffer(c, bgc_map.getmapposition(position(x, y)).gettext(), bgc_image_colour);
                        }
                        else
                        {
                            g_Console.writeToBuffer(c, bgc_map.getmapposition(position(x, y)).gettext(), bgc_image_colour + bg_image_colour);
                        }
                    }
                }
            }
        }
    }
    renderWall();
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = (g_Console.getConsoleSize().X - ss.tellp())/2;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 96);

    //displays player hp
    ss.str("");
    ss << "lives:";
    for (int i = 0; i < g_player->gethp(); i++)
    {
        ss << " " << char(3);
    }
    c.X = 0;
    c.Y = 0;
    if (toiletpaperbuff == true)
    {
        g_Console.writeToBuffer(c, ss.str(), 14 + 96);
    }
    else
    {
        g_Console.writeToBuffer(c, ss.str(), 4 + 96);
    }
    //displays current tool
    ss.str("");
    ss << "current tool: ";
    ss << tools[currenttool];
    c.X = 0;
    c.Y = g_Console.getConsoleSize().Y-1;
    g_Console.writeToBuffer(c, ss.str(), 96);

}

// this is an example of how you would use the input events
void renderInputEvents()
{
    // keyboard events
    /*COORD startPos = {50, 2};*/
    std::ostringstream ss;
    /*std::string key;
    for (int i = 0; i < K_COUNT; ++i)
    {
        ss.str("");
        switch (i)
        {
        case K_UP: key = "UP";
            break;
        case K_DOWN: key = "DOWN";
            break;
        case K_LEFT: key = "LEFT";
            break;
        case K_RIGHT: key = "RIGHT";
            break;
        case K_SPACE: key = "SPACE";
            break;
        default: continue;
        }
        if (g_skKeyEvent[i].keyDown)
            ss << key << " pressed";
        else if (g_skKeyEvent[i].keyReleased)
            ss << key << " released";
        else
            ss << key << " not pressed";

        COORD c = { startPos.X, startPos.Y + i };
        g_Console.writeToBuffer(c, ss.str(), 0x17);
    }*/

    // mouse events    
    //debugging
    ss.str("");

    //debugtext = g_player->gethp();
    ss << "x: " << g_mouseEvent.mousePosition.X + g_map.getcampos().get('x') << "y: " << g_mouseEvent.mousePosition.Y + g_map.getcampos().get('y');

    debugtext = g_player->gethp();
    //ss << "debug text: " << debugtext;
    //ss << "x: " << g_mouseEvent.mousePosition.X + g_map.getcampos().get('x') << "y: " << g_mouseEvent.mousePosition.Y + g_map.getcampos().get('y'); //position debug

    g_Console.writeToBuffer(g_mouseEvent.mousePosition, ss.str(), 0x49);
    ss.str("");

    //tooltip
    if (mouse_tooltip_enabled)
    {
        getentityfrompos(&idx[0], position(g_mouseEvent.mousePosition.X + g_map.getcampos().get('x'), g_mouseEvent.mousePosition.Y + g_map.getcampos().get('y')), g_map);
        if (idx[0] != -1)
        {
            ss << entities[idx[0]]->getname();
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X - (ss.tellp()/2), g_mouseEvent.mousePosition.Y + 1, ss.str(), 0x49);
        }
    }
//    switch (g_mouseEvent.eventFlags)
//    {
//    case 0:
///*        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
//        {
//            ss.str("Left Button Pressed");
//            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, ss.str(), 0x59);
//        }
//        else */if (g_mouseEvent.buttonState == RIGHTMOST_BUTTON_PRESSED)
//        {
//            ss.str("Right Button Pressed");
//            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, ss.str(), 0x59);
//        }
//        else
//        {
//            ss.str("Some Button Pressed");
//            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 3, ss.str(), 0x59);
//        }
//        break;
//    case DOUBLE_CLICK:
//        ss.str("Double Clicked");
//        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 4, ss.str(), 0x59);
//        break;        
//    case MOUSE_WHEELED:
//        if (g_mouseEvent.buttonState & 0xFF000000)
//            ss.str("Mouse wheeled down");
//        else
//            ss.str("Mouse wheeled up");
//        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 5, ss.str(), 0x59);
//        break;
//    default:        
//        break;
//    }
    
}

void spawnvirus() {
    if (virus::gettotal() < 20)
    {
        for (int i = 0; i < size(entities); i++)
        {
            if (entities[i] == NULL)
            {
                entities[i] = new virus(0.5f, g_map);
                break;
            }
        }
    }
}

//returns the entity that is in pos of g_map
void getentityfrompos(int* ptr, position pos, map& g_map) {
    for (int i = 0; i < MAXENTITY; i++)
    {
        ptr[i] = NULL;
    }
    for (int i = 0; i < MAXENTITY; i++)
    {
        if (entities[i] != NULL && entities[i]->getpos().get('x') == pos.get('x') && entities[i]->getpos().get('y') == pos.get('y'))
        {
            for (int i2 = 0; i2 < MAXENTITY; i2++)
            {
                if (ptr[i2] == NULL)
                {
                    ptr[i2] = i;
                    break;
                }
            }
        }
    }
    if (ptr[0] == NULL)
    {
        ptr[0] = -1; //ptr[0] == -1 when no entity is in that position
    }
}

//deletes all entities except for the player
void clearentities() {
    for (int i = 1; i < MAXENTITY; i++)
    {
        if (entities[i] != NULL)
        {
            entities[i]->die(g_map, bg_map, bgc_map);
            entities[i] = NULL;
        }
    }
}

void renderMask()
{
    if (maplevel == 1)
    {
        WORD charColor = 0x0C;
        g_map.setmapposition(position(190, 24), image('M', charColor));
    }
    else if (maplevel == 2)
    {

        WORD charColor = 0x0C;
        g_map.setmapposition(position(126, 55), image('M', charColor));

    }
    else if (maplevel == 3)
    {

        WORD charColor = 0x0C;
        g_map.setmapposition(position(117, 198), image('M', charColor));
    }
    else if (maplevel == 4)
    {

        WORD charColor = 0x0C;
        g_map.setmapposition(position(79, 72), image('M', charColor));

    }
    else if (maplevel == 6)
    {
        g_eGameState = S_WIN;
    }
}

void renderTP()
{
    
}

//render border walls
void renderWall()
{
    for (int i = 0; i < g_map.getmapsize('x'); i++)
    {
        WORD charColor = 240; //bg white
        g_map.setmapposition(position(i, g_map.getmapsize('y')), image(' ', charColor)); //bottom wall border
        g_map.setmapposition(position(i, 0), image(' ', charColor));//top wall border
        for (int i = 0; i < g_map.getmapsize('y'); i++)
        {
            g_map.setmapposition(position(0, i), image(' ', charColor)); //left wall border
            g_map.setmapposition(position(g_map.getmapsize('x') - 1, i), image(' ', charColor)); //right wall border
        }
    }
}

void maskrenderout()
{
    WORD charColor = 0x00;
    g_map.setmapposition(position(152, 15), image('T', charColor));
}

void mapchange(int x)
{
    clearentities();
    g_map.clearmap();
    bg_map.clearmap();
    bgc_map.clearmap();
    fg_map.clearmap();
    //background char map
    bgc_map.fill(bgc_images_nature, size(bgc_images_nature), bgc_weightage_nature);
    //background colour only map
    bg_map.fill(bg_images_nature, size(bg_images_nature), bg_weightage_nature);
    WORD charColor = 240;
    nonbuffplayerskin = image(1, 0);
    g_player->setimage(nonbuffplayerskin);
    if (maplevel < 6) {
        g_eGameState = S_MAPT;
    }
    if (maplevel == 1)
    {
        g_dElapsedTime = 300.0; // susceptible to changes for level 1
        nonbuffplayerskin = image(1, 3);
        g_player->setimage(nonbuffplayerskin);
        wallskin = image(NULL,240);
        backgroundchange("space", "space", "clear");
        entities[1] = new virus_spawner(position(107, 35), 0.1f, g_map);
        entities[2] = new virus_spawner(position(136, 11), 0.1f, g_map);
        entities[3] = new NPC(position(155, 14), 99, 0.25f, image(2, 15), "SELLING TOILET PAPER!`HEY YOU WANT A FREE TRIAL?`GUARENTEED TO PROTECT YOU FROM VIRUS for 4.9+ seconds(no refunds)", "TP Man", g_map);
        entities[4] = new NPC(position(165, 43), 99, 0.25f, image(2, 4), "THE MESSIAH IS HERE!`Retrieve the 4 Masks of Yendor and save the world`I will guide you along the journey", "Worshipper", g_map);
     
        for (int i = 0; i < 40; i++)
        {
            g_map.setmapposition(position(180, 5 + i), image(' ', charColor));

        }
        for (int i = 0; i < 20; i++)
        {
            g_map.setmapposition(position(180 + i, 25), image(' ', charColor));
            g_map.setmapposition(position(180 + i, 29), image(' ', charColor));
        }
        for (int i = 0; i < 82; i++)
        {
            g_map.setmapposition(position(98 + i, 5), image(' ', charColor));
            g_map.setmapposition(position(98 + i, 6), image(' ', charColor));
        }
        for (int i = 0; i < 7; i++)
        {

            g_map.setmapposition(position(148, 6 + i), image(' ', charColor));

        }
        for (int i = 0; i < 34; i++)
        {

            g_map.setmapposition(position(146 + i, 13), image(' ', charColor));

        }
        for (int i = 0; i < 28; i++)
        {

            g_map.setmapposition(position(91 + i, 9), image(' ', charColor));

        }
        for (int i = 0; i < 12; i++)
        {

            g_map.setmapposition(position(91, 9 + i), image(' ', charColor));
            g_map.setmapposition(position(137, 31 + i), image(' ', charColor));

        }
        for (int i = 0; i < 2; i++)
        {

            g_map.setmapposition(position(118, 10 + i), image(' ', charColor));


        }
        for (int i = 0; i < 8; i++)
        {

            g_map.setmapposition(position(118 + i, 11), image(' ', charColor));
            g_map.setmapposition(position(138 , 31+i), image(' ', charColor));
            g_map.setmapposition(position(139, 31 + i), image(' ', charColor));
            g_map.setmapposition(position(140, 31 + i), image(' ', charColor));
            g_map.setmapposition(position(141, 31 + i), image(' ', charColor));
            g_map.setmapposition(position(142, 31 + i), image(' ', charColor));
            g_map.setmapposition(position(143, 31 + i), image(' ', charColor));
            g_map.setmapposition(position(144, 31 + i), image(' ', charColor));

        }
        for (int i = 0; i < 6; i++)
        {

            g_map.setmapposition(position(125, 11 + i), image(' ', charColor));

        }
        for (int i = 0; i < 36; i++)
        {

            g_map.setmapposition(position(125 + i, 16), image(' ', charColor));

        }
        for (int i = 0; i < 15; i++)
        {

            g_map.setmapposition(position(160, 16 + i), image(' ', charColor));

        }

        for (int i = 0; i < 11; i++)
        {

            g_map.setmapposition(position(150, 20 + i), image(' ', charColor));

        }
        for (int i = 0; i < 10; i++)
        {

            g_map.setmapposition(position(150 + i, 30), image(' ', charColor));

        }
        for (int i = 0; i < 10; i++)
        {

            g_map.setmapposition(position(175 + i, 44), image(' ', charColor));

        }
        for (int i = 0; i < 30; i++)
        {

            g_map.setmapposition(position(175, 14 + i), image(' ', charColor));
            g_map.setmapposition(position(145 + i, 34), image(' ', charColor));
            g_map.setmapposition(position(137 + i, 34), image(' ', charColor));
            g_map.setmapposition(position(115 + i, 27), image(' ', charColor));

        }
        for (int i = 0; i < 7; i++)
        {

            g_map.setmapposition(position(145, 27 + i), image(' ', charColor));

        }
        for (int i = 0; i < 5; i++)
        {

            g_map.setmapposition(position(115, 27 + i), image(' ', charColor));

        }
        for (int i = 0; i < 22; i++)
        {

            g_map.setmapposition(position(115 + i, 31), image(' ', charColor));

        }
        for (int i = 0; i < 38; i++)
        {

            g_map.setmapposition(position(137 + i, 42), image(' ', charColor));

        }
        for (int i = 0; i < 60; i++)
        {
            g_map.setmapposition(position(91 + i, 20), image(' ', charColor));
        }
        for (int i = 0; i < 46; i++)
        {
            g_map.setmapposition(position(91, i), image(' ', charColor));
        }
        for (int i = 0; i < 108; i++)
        {
            g_map.setmapposition(position(91 + i, 46), image(' ', charColor));
        }


        //FILLING
        for (int i = 0; i < 37; i++)
        {
            g_map.setmapposition(position(138 + i, 35), image(' ', charColor));
            g_map.setmapposition(position(138 + i, 36), image(' ', charColor));
            g_map.setmapposition(position(138 + i, 37), image(' ', charColor));
            g_map.setmapposition(position(138 + i, 38), image(' ', charColor));
            g_map.setmapposition(position(138 + i, 39), image(' ', charColor));
            g_map.setmapposition(position(138 + i, 40), image(' ', charColor));
            g_map.setmapposition(position(138 + i, 41), image(' ', charColor));
        }
        for (int i = 0; i < 26; i++)
        {
            g_map.setmapposition(position(92 + i, 10), image(' ', charColor));
            g_map.setmapposition(position(92 + i, 11), image(' ', charColor));
        }
        for (int i = 0; i < 10; i++)
        {
            g_map.setmapposition(position(151, 20 + i), image(' ', charColor));
            g_map.setmapposition(position(152, 20 + i), image(' ', charColor));
            g_map.setmapposition(position(153, 20 + i), image(' ', charColor));
            g_map.setmapposition(position(154, 20 + i), image(' ', charColor));
            g_map.setmapposition(position(155, 20 + i), image(' ', charColor));
            g_map.setmapposition(position(156, 20 + i), image(' ', charColor));
            g_map.setmapposition(position(157, 20 + i), image(' ', charColor));
            g_map.setmapposition(position(158, 20 + i), image(' ', charColor));
            g_map.setmapposition(position(159, 20 + i), image(' ', charColor));
           
        }
        for (int i = 0; i < 29; i++)
        {
            g_map.setmapposition(position(116+i, 28 ), image(' ', charColor));
            g_map.setmapposition(position(116+i, 29 ), image(' ', charColor));
            g_map.setmapposition(position(116+i, 30 ), image(' ', charColor));
        }
        for (int i = 0; i < 33; i++)
        {
            g_map.setmapposition(position(92 + i, 12 ), image(' ', charColor));
            g_map.setmapposition(position(92 + i, 13 ), image(' ', charColor));
            g_map.setmapposition(position(92 + i, 14 ), image(' ', charColor));
            g_map.setmapposition(position(92 + i, 15 ), image(' ', charColor));
            g_map.setmapposition(position(92 + i, 16 ), image(' ', charColor));
        }
        for (int i = 0; i < 68; i++)
        {
            g_map.setmapposition(position(92 + i, 17), image(' ', charColor));
            g_map.setmapposition(position(92 + i, 18), image(' ', charColor));
            g_map.setmapposition(position(92 + i, 19), image(' ', charColor));
        }
        for (int i = 0; i < 31; i++)
        {
            g_map.setmapposition(position(149+i, 7 ), image(' ', charColor));
            g_map.setmapposition(position(149 + i, 8 ), image(' ', charColor));
            g_map.setmapposition(position(149 + i, 9 ), image(' ', charColor));
            g_map.setmapposition(position(149 + i, 10 ), image(' ', charColor));
            g_map.setmapposition(position(149 + i, 11 ), image(' ', charColor));
            g_map.setmapposition(position(149 + i, 12 ), image(' ', charColor));
        }
        for (int i = 0; i < 30; i++)
        {
            g_map.setmapposition(position(176 , 14 + i), image(' ', charColor));
            g_map.setmapposition(position(177 , 14 + i), image(' ', charColor));
            g_map.setmapposition(position(178 , 14 + i), image(' ', charColor));
            g_map.setmapposition(position(179 , 14 +i), image(' ', charColor));
        }
        for (int i = 0; i < 18; i++)
        {
            g_map.setmapposition(position(181 + i, 26 ), image(' ', charColor));
            g_map.setmapposition(position(181 + i, 27 ), image(' ', charColor));
            g_map.setmapposition(position(181 + i, 28 ), image(' ', charColor));
        }
        //END OF FILLING
        WORD charColor = 0x0D;
        g_map.setmapposition(position(152, 15), image('T', charColor));
        // END OF MAP 1 DESIGN
    }
    else if (maplevel == 2)
    {
        g_dElapsedTime = 75.0; // susceptible to changes for level 2
        wallskin = image(NULL, 80);
        backgroundchange("yellow", "clear", "clear");
        entities[1] = new virus_spawner(position(151, 25), 0.1f, g_map);
        entities[2] = new virus_spawner(position(51, 25), 0.1f, g_map);
        entities[3] = new virus_spawner(position(81, 50), 0.1f, g_map);
        entities[4] = new virus_spawner(position(52, 75), 0.1f, g_map);
        entities[5] = new virus_spawner(position(151, 75), 0.1f, g_map);

        for (int i = 0; i < 11; i++) //Map 2's Skeleton
        {
            g_map.setmapposition(position(71, i), image(' ', charColor));
            g_map.setmapposition(position(131, i), image(' ', charColor));
            g_map.setmapposition(position(51, 10 + i), image(' ', charColor));
            g_map.setmapposition(position(151, 10 + i), image(' ', charColor));
            g_map.setmapposition(position(41, 20 + i), image(' ', charColor));
            g_map.setmapposition(position(161, 20 + i), image(' ', charColor));
            g_map.setmapposition(position(51, 30 + i), image(' ', charColor));
            g_map.setmapposition(position(151, 30 + i), image(' ', charColor));
            g_map.setmapposition(position(71, 40 + i), image(' ', charColor));
            g_map.setmapposition(position(131, 40 + i), image(' ', charColor));
            g_map.setmapposition(position(71, 50 + i), image(' ', charColor));
            g_map.setmapposition(position(131, 50 + i), image(' ', charColor));
            g_map.setmapposition(position(51, 60 + i), image(' ', charColor));
            g_map.setmapposition(position(151, 60 + i), image(' ', charColor));
            g_map.setmapposition(position(41, 70 + i), image(' ', charColor));
            g_map.setmapposition(position(161, 70 + i), image(' ', charColor));
            g_map.setmapposition(position(51, 80 + i), image(' ', charColor));
            g_map.setmapposition(position(151, 80 + i), image(' ', charColor));
            g_map.setmapposition(position(71, 90 + i), image(' ', charColor));
            g_map.setmapposition(position(131, 90 + i), image(' ', charColor));
            g_map.setmapposition(position(51 + i, 10), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 10), image(' ', charColor));
            g_map.setmapposition(position(131 + i, 10), image(' ', charColor));
            g_map.setmapposition(position(141 + i, 10), image(' ', charColor));
            g_map.setmapposition(position(41 + i, 20), image(' ', charColor));
            g_map.setmapposition(position(151 + i, 20), image(' ', charColor));
            g_map.setmapposition(position(41 + i, 30), image(' ', charColor));
            g_map.setmapposition(position(151 + i, 30), image(' ', charColor));
            g_map.setmapposition(position(51 + i, 40), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 40), image(' ', charColor));
            g_map.setmapposition(position(131 + i, 40), image(' ', charColor));
            g_map.setmapposition(position(141 + i, 40), image(' ', charColor));
            g_map.setmapposition(position(51 + i, 60), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 60), image(' ', charColor));
            g_map.setmapposition(position(131 + i, 60), image(' ', charColor));
            g_map.setmapposition(position(141 + i, 60), image(' ', charColor));
            g_map.setmapposition(position(41 + i, 70), image(' ', charColor));
            g_map.setmapposition(position(151 + i, 70), image(' ', charColor));
            g_map.setmapposition(position(41 + i, 80), image(' ', charColor));
            g_map.setmapposition(position(151 + i, 80), image(' ', charColor));
            g_map.setmapposition(position(51 + i, 90), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 90), image(' ', charColor));
            g_map.setmapposition(position(131 + i, 90), image(' ', charColor));
            g_map.setmapposition(position(141 + i, 90), image(' ', charColor));
            g_map.setmapposition(position(71 + i, 100), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 100), image(' ', charColor));
            g_map.setmapposition(position(91 + i, 100), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 100), image(' ', charColor));
            g_map.setmapposition(position(111 + i, 100), image(' ', charColor));
            g_map.setmapposition(position(121 + i, 100), image(' ', charColor));
        }
        for (int i = 0; i < 41; i++) //Map 2's Juicy Insides 
        {
            g_map.setmapposition(position(81 + i, 6), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 7), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 8), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 9), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 10), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 11), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 12), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 13), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 14), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 15), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 16), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 16), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 17), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 17), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 18), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 18), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 19), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 19), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 20), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 20), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 21), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 21), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 22), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 22), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 23), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 23), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 24), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 24), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 25), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 25), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 26), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 26), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 27), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 27), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 28), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 28), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 29), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 29), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 30), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 30), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 31), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 31), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 32), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 32), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 33), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 33), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 34), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 34), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 35), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 35), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 36), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 37), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 38), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 39), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 40), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 41), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 42), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 43), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 44), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 55), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 56), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 57), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 58), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 59), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 60), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 61), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 62), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 63), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 64), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 64), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 65), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 65), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 66), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 66), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 67), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 67), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 68), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 68), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 69), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 69), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 70), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 70), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 71), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 71), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 72), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 72), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 73), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 73), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 74), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 74), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 75), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 75), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 76), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 76), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 77), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 77), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 78), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 78), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 79), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 79), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 80), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 80), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 81), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 81), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 82), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 82), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 83), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 83), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 84), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 84), image(' ', charColor));
            g_map.setmapposition(position(61 + i, 85), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 85), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 86), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 87), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 88), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 89), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 90), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 91), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 92), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 93), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 94), image(' ', charColor));
            g_map.setmapposition(position(81 + i, 95), image(' ', charColor));
        }
        for (int i = 0; i < 31; i++) // Map 3 Mid-Section
        {
            g_map.setmapposition(position(101 + i, 45), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 46), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 47), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 48), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 49), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 50), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 51), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 52), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 53), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 54), image(' ', charColor));
        }
        entities[0]->setpos(position(126, 44), g_map);
        WORD charColor = 0x0D;
        g_map.setmapposition(position(43, 75), image('T', charColor));

    }
    else if (maplevel == 3)
    {
        wallskin = image(NULL, 0);
        backgroundchange("white", "clear", "clear");
        entities[1] = new virus_spawner(position(98, 161), 0.1f, g_map);
        entities[2] = new virus_spawner(position(73, 135), 0.1f, g_map);
        entities[3] = new virus_spawner(position(98, 109), 0.1f, g_map);
        entities[4] = new virus_spawner(position(117, 169), 0.1f, g_map);
        entities[5] = new virus_spawner(position(118, 183), 0.1f, g_map);
        g_dElapsedTime = 75.0; // susceptible to changes for level 3
        for (int i = 0; i < 43; i++) //Map 3's Wrist
        {
            g_map.setmapposition(position(77, 200 - i), image(' ', charColor));
            g_map.setmapposition(position(105, 200 - i), image(' ', charColor));
            g_map.setmapposition(position(106, 200 - i), image(' ', charColor));
            g_map.setmapposition(position(107, 200 - i), image(' ', charColor));
            g_map.setmapposition(position(108, 200 - i), image(' ', charColor));
            g_map.setmapposition(position(109, 200 - i), image(' ', charColor));
            g_map.setmapposition(position(110, 200 - i), image(' ', charColor));
            g_map.setmapposition(position(111, 200 - i), image(' ', charColor));
            g_map.setmapposition(position(124, 200 - i), image(' ', charColor));
        }
        for (int i = 0; i < 24; i++)
        {
            g_map.setmapposition(position(92, 181 - i), image(' ', charColor));
            g_map.setmapposition(position(91, 181 - i), image(' ', charColor));
            g_map.setmapposition(position(90, 181 - i), image(' ', charColor));
            g_map.setmapposition(position(89, 181 - i), image(' ', charColor));
            g_map.setmapposition(position(88, 181 - i), image(' ', charColor));
        }
        for (int i = 0; i < 20; i++)
        {
            g_map.setmapposition(position(78 + i, 194), image(' ', charColor));
            g_map.setmapposition(position(78 + i, 193), image(' ', charColor));
            g_map.setmapposition(position(78 + i, 192), image(' ', charColor));
            g_map.setmapposition(position(78 + i, 191), image(' ', charColor));
            g_map.setmapposition(position(78 + i, 190), image(' ', charColor));
        }
        for (int i = 0; i < 5; i++)
        {
            g_map.setmapposition(position(88 + i, 189), image(' ', charColor));
            g_map.setmapposition(position(88 + i, 188), image(' ', charColor));
            g_map.setmapposition(position(88 + i, 187), image(' ', charColor));
            g_map.setmapposition(position(88 + i, 186), image(' ', charColor));
            g_map.setmapposition(position(88 + i, 185), image(' ', charColor));
        }
        for (int i = 0; i < 3; i++)
        {
            g_map.setmapposition(position(93 + i, 175), image(' ', charColor));
            g_map.setmapposition(position(93 + i, 174), image(' ', charColor));
            g_map.setmapposition(position(93 + i, 173), image(' ', charColor));
            g_map.setmapposition(position(93 + i, 166), image(' ', charColor));
            g_map.setmapposition(position(96 + i, 166), image(' ', charColor));
            g_map.setmapposition(position(93 + i, 165), image(' ', charColor));
            g_map.setmapposition(position(96 + i, 165), image(' ', charColor));
            g_map.setmapposition(position(93 + i, 164), image(' ', charColor));
            g_map.setmapposition(position(96 + i, 164), image(' ', charColor));
            g_map.setmapposition(position(93 + i, 158), image(' ', charColor));
            g_map.setmapposition(position(96 + i, 158), image(' ', charColor));
            g_map.setmapposition(position(99 + i, 158), image(' ', charColor));
            g_map.setmapposition(position(102 + i, 158), image(' ', charColor));
            g_map.setmapposition(position(77, 157 - i), image(' ', charColor));
            g_map.setmapposition(position(77, 154 - i), image(' ', charColor));
        }
        for (int i = 0; i < 2; i++) // Map 3's Fist
        {
            g_map.setmapposition(position(72, 151 - i), image(' ', charColor));
            g_map.setmapposition(position(67, 149 - i), image(' ', charColor));
            g_map.setmapposition(position(62, 147 - i), image(' ', charColor));
            g_map.setmapposition(position(57, 145 - i), image(' ', charColor));
            g_map.setmapposition(position(52, 143 - i), image(' ', charColor));
            g_map.setmapposition(position(47, 141 - i), image(' ', charColor));
            g_map.setmapposition(position(42, 139 - i), image(' ', charColor));
            g_map.setmapposition(position(37, 137 - i), image(' ', charColor));
            g_map.setmapposition(position(36, 130 - i), image(' ', charColor));
            g_map.setmapposition(position(41, 128 - i), image(' ', charColor));
            g_map.setmapposition(position(46, 126 - i), image(' ', charColor));
            g_map.setmapposition(position(51, 124 - i), image(' ', charColor));
            g_map.setmapposition(position(56, 122 - i), image(' ', charColor));
            g_map.setmapposition(position(61, 120 - i), image(' ', charColor));
            g_map.setmapposition(position(67, 119 - i), image(' ', charColor));
            g_map.setmapposition(position(81, 115 - i), image(' ', charColor));
            g_map.setmapposition(position(81, 112 - i), image(' ', charColor));
            g_map.setmapposition(position(86, 110 - i), image(' ', charColor));
            g_map.setmapposition(position(91, 108 - i), image(' ', charColor));
            g_map.setmapposition(position(96, 106 - i), image(' ', charColor));
            g_map.setmapposition(position(102, 106 - i), image(' ', charColor));
            g_map.setmapposition(position(107, 108 - i), image(' ', charColor));
            g_map.setmapposition(position(117, 115 - i), image(' ', charColor));
            g_map.setmapposition(position(122, 117 - i), image(' ', charColor));
            g_map.setmapposition(position(126, 120 - i), image(' ', charColor));
            g_map.setmapposition(position(132, 123 - i), image(' ', charColor));
            g_map.setmapposition(position(137, 125 - i), image(' ', charColor));
            g_map.setmapposition(position(141, 128 - i), image(' ', charColor));
            g_map.setmapposition(position(147, 130 - i), image(' ', charColor));
            g_map.setmapposition(position(152, 132 - i), image(' ', charColor));
            g_map.setmapposition(position(152, 139 - i), image(' ', charColor));
            g_map.setmapposition(position(147, 141 - i), image(' ', charColor));
            g_map.setmapposition(position(142, 143 - i), image(' ', charColor));
            g_map.setmapposition(position(110 + i, 152), image(' ', charColor));
            g_map.setmapposition(position(110 + i, 151), image(' ', charColor));
            g_map.setmapposition(position(108, 148 - i), image(' ', charColor));
            g_map.setmapposition(position(107, 148 - i), image(' ', charColor));
            g_map.setmapposition(position(105, 144 - i), image(' ', charColor));
            g_map.setmapposition(position(104, 144 - i), image(' ', charColor));
            g_map.setmapposition(position(102, 140 - i), image(' ', charColor));
            g_map.setmapposition(position(101, 140 - i), image(' ', charColor));
        }
        for (int i = 0; i < 5; i++)
        {
            g_map.setmapposition(position(76 - i, 152), image(' ', charColor));
            g_map.setmapposition(position(71 - i, 150), image(' ', charColor));
            g_map.setmapposition(position(66 - i, 148), image(' ', charColor));
            g_map.setmapposition(position(61 - i, 146), image(' ', charColor));
            g_map.setmapposition(position(56 - i, 144), image(' ', charColor));
            g_map.setmapposition(position(51 - i, 142), image(' ', charColor));
            g_map.setmapposition(position(46 - i, 140), image(' ', charColor));
            g_map.setmapposition(position(41 - i, 138), image(' ', charColor));
            g_map.setmapposition(position(36 - i, 136), image(' ', charColor));
            g_map.setmapposition(position(32, 135 - i), image(' ', charColor));
            g_map.setmapposition(position(32 + i, 131), image(' ', charColor));
            g_map.setmapposition(position(37 + i, 129), image(' ', charColor));
            g_map.setmapposition(position(42 + i, 127), image(' ', charColor));
            g_map.setmapposition(position(47 + i, 125), image(' ', charColor));
            g_map.setmapposition(position(52 + i, 123), image(' ', charColor));
            g_map.setmapposition(position(57 + i, 121), image(' ', charColor));
            g_map.setmapposition(position(62 + i, 119), image(' ', charColor));
            g_map.setmapposition(position(67 + i, 117), image(' ', charColor));
            g_map.setmapposition(position(72 + i, 117), image(' ', charColor));
            g_map.setmapposition(position(77 + i, 117), image(' ', charColor));
            g_map.setmapposition(position(67 + i, 116), image(' ', charColor));
            g_map.setmapposition(position(72 + i, 116), image(' ', charColor));
            g_map.setmapposition(position(77 + i, 116), image(' ', charColor));
            g_map.setmapposition(position(77 + i, 113), image(' ', charColor));
            g_map.setmapposition(position(77 + i, 114), image(' ', charColor));
            g_map.setmapposition(position(77 + i, 115), image(' ', charColor));
            g_map.setmapposition(position(82 + i, 111), image(' ', charColor));
            g_map.setmapposition(position(87 + i, 109), image(' ', charColor));
            g_map.setmapposition(position(92 + i, 107), image(' ', charColor));
            g_map.setmapposition(position(97 + i, 105), image(' ', charColor));
            g_map.setmapposition(position(102 + i, 107), image(' ', charColor));
            g_map.setmapposition(position(107 + i, 109), image(' ', charColor));
            g_map.setmapposition(position(107 + i, 110), image(' ', charColor));
            g_map.setmapposition(position(107 + i, 111), image(' ', charColor));
            g_map.setmapposition(position(107 + i, 112), image(' ', charColor));
            g_map.setmapposition(position(107 + i, 113), image(' ', charColor));
            g_map.setmapposition(position(107 + i, 114), image(' ', charColor));
            g_map.setmapposition(position(107 + i, 115), image(' ', charColor));
            g_map.setmapposition(position(112 + i, 114), image(' ', charColor));
            g_map.setmapposition(position(117 + i, 116), image(' ', charColor));
            g_map.setmapposition(position(122 + i, 118), image(' ', charColor));
            g_map.setmapposition(position(122 + i, 120), image(' ', charColor));
            g_map.setmapposition(position(122 + i, 121), image(' ', charColor));
            g_map.setmapposition(position(122 + i, 122), image(' ', charColor));
            g_map.setmapposition(position(122 + i, 123), image(' ', charColor));
            g_map.setmapposition(position(122 + i, 124), image(' ', charColor));
            g_map.setmapposition(position(122 + i, 125), image(' ', charColor));
            g_map.setmapposition(position(122 + i, 126), image(' ', charColor));
            g_map.setmapposition(position(122 + i, 127), image(' ', charColor));
            g_map.setmapposition(position(122 + i, 128), image(' ', charColor));
            g_map.setmapposition(position(122 + i, 129), image(' ', charColor));
            g_map.setmapposition(position(122 + i, 130), image(' ', charColor));
            g_map.setmapposition(position(122 + i, 131), image(' ', charColor));
            g_map.setmapposition(position(127 + i, 122), image(' ', charColor));
            g_map.setmapposition(position(132 + i, 124), image(' ', charColor));
            g_map.setmapposition(position(137 + i, 126), image(' ', charColor));
            g_map.setmapposition(position(137 + i, 128), image(' ', charColor));
            g_map.setmapposition(position(137 + i, 129), image(' ', charColor));
            g_map.setmapposition(position(142 + i, 129), image(' ', charColor));
            g_map.setmapposition(position(147 + i, 131), image(' ', charColor));
            g_map.setmapposition(position(152 + i, 133), image(' ', charColor));
            g_map.setmapposition(position(156, 133 + i), image(' ', charColor));
            g_map.setmapposition(position(156 - i, 137), image(' ', charColor));
            g_map.setmapposition(position(151 - i, 139), image(' ', charColor));
            g_map.setmapposition(position(146 - i, 141), image(' ', charColor));
            g_map.setmapposition(position(141 - i, 143), image(' ', charColor));
            g_map.setmapposition(position(137, 143 + i), image(' ', charColor));
            g_map.setmapposition(position(137, 148 + i), image(' ', charColor));
            g_map.setmapposition(position(137, 153 + i), image(' ', charColor));
            g_map.setmapposition(position(124 + i, 158), image(' ', charColor));
            g_map.setmapposition(position(129 + i, 158), image(' ', charColor));
            g_map.setmapposition(position(137 - i, 158), image(' ', charColor));
            g_map.setmapposition(position(112 + i, 157), image(' ', charColor));
            g_map.setmapposition(position(107 + i, 157), image(' ', charColor));
            g_map.setmapposition(position(102 + i, 157), image(' ', charColor));
            g_map.setmapposition(position(112 + i, 156), image(' ', charColor));
            g_map.setmapposition(position(107 + i, 156), image(' ', charColor));
            g_map.setmapposition(position(102 + i, 156), image(' ', charColor));
            g_map.setmapposition(position(107 + i, 155), image(' ', charColor));
            g_map.setmapposition(position(107 + i, 154), image(' ', charColor));
            g_map.setmapposition(position(107 + i, 153), image(' ', charColor));
            g_map.setmapposition(position(111 - i, 150), image(' ', charColor));
            g_map.setmapposition(position(111 - i, 149), image(' ', charColor));
            g_map.setmapposition(position(108 - i, 146), image(' ', charColor));
            g_map.setmapposition(position(108 - i, 145), image(' ', charColor));
            g_map.setmapposition(position(105 - i, 142), image(' ', charColor));
            g_map.setmapposition(position(105 - i, 141), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 138), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 137), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 136), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 136), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 135), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 135), image(' ', charColor));
            g_map.setmapposition(position(82 - i, 149), image(' ', charColor));
            g_map.setmapposition(position(82 - i, 148), image(' ', charColor));
            g_map.setmapposition(position(82 - i, 147), image(' ', charColor));
            g_map.setmapposition(position(77 - i, 147), image(' ', charColor));
            g_map.setmapposition(position(82 - i, 146), image(' ', charColor));
            g_map.setmapposition(position(77 - i, 146), image(' ', charColor));
            g_map.setmapposition(position(77 - i, 145), image(' ', charColor));
            g_map.setmapposition(position(72 - i, 145), image(' ', charColor));
            g_map.setmapposition(position(77 - i, 144), image(' ', charColor));
            g_map.setmapposition(position(72 - i, 144), image(' ', charColor));
            g_map.setmapposition(position(72 - i, 143), image(' ', charColor));
            g_map.setmapposition(position(67 - i, 143), image(' ', charColor));
            g_map.setmapposition(position(72 - i, 142), image(' ', charColor));
            g_map.setmapposition(position(67 - i, 142), image(' ', charColor));
            g_map.setmapposition(position(67 - i, 141), image(' ', charColor));
            g_map.setmapposition(position(62 - i, 141), image(' ', charColor));
            g_map.setmapposition(position(67 - i, 140), image(' ', charColor));
            g_map.setmapposition(position(62 - i, 140), image(' ', charColor));
            g_map.setmapposition(position(62 - i, 139), image(' ', charColor));
            g_map.setmapposition(position(57 - i, 139), image(' ', charColor));
            g_map.setmapposition(position(62 - i, 138), image(' ', charColor));
            g_map.setmapposition(position(57 - i, 138), image(' ', charColor));
            g_map.setmapposition(position(57 - i, 137), image(' ', charColor));
            g_map.setmapposition(position(52 - i, 137), image(' ', charColor));
            g_map.setmapposition(position(57 - i, 136), image(' ', charColor));
            g_map.setmapposition(position(52 - i, 136), image(' ', charColor));
            g_map.setmapposition(position(52 - i, 135), image(' ', charColor));
            g_map.setmapposition(position(47 - i, 135), image(' ', charColor));
            g_map.setmapposition(position(52 - i, 134), image(' ', charColor));
            g_map.setmapposition(position(47 - i, 134), image(' ', charColor));
            g_map.setmapposition(position(52 - i, 133), image(' ', charColor));
            g_map.setmapposition(position(47 - i, 133), image(' ', charColor));
            g_map.setmapposition(position(52 - i, 132), image(' ', charColor));
            g_map.setmapposition(position(47 - i, 132), image(' ', charColor));
            g_map.setmapposition(position(52 - i, 131), image(' ', charColor));
            g_map.setmapposition(position(57 - i, 131), image(' ', charColor));
            g_map.setmapposition(position(52 - i, 130), image(' ', charColor));
            g_map.setmapposition(position(57 - i, 130), image(' ', charColor));
            g_map.setmapposition(position(57 - i, 129), image(' ', charColor));
            g_map.setmapposition(position(62 - i, 129), image(' ', charColor));
            g_map.setmapposition(position(57 - i, 128), image(' ', charColor));
            g_map.setmapposition(position(62 - i, 128), image(' ', charColor));
            g_map.setmapposition(position(62 - i, 127), image(' ', charColor));
            g_map.setmapposition(position(67 - i, 127), image(' ', charColor));
            g_map.setmapposition(position(62 - i, 126), image(' ', charColor));
            g_map.setmapposition(position(67 - i, 126), image(' ', charColor));
            g_map.setmapposition(position(72 - i, 127), image(' ', charColor));
            g_map.setmapposition(position(77 - i, 127), image(' ', charColor));
            g_map.setmapposition(position(72 - i, 126), image(' ', charColor));
            g_map.setmapposition(position(77 - i, 126), image(' ', charColor));
            g_map.setmapposition(position(82 - i, 127), image(' ', charColor));
            g_map.setmapposition(position(87 - i, 127), image(' ', charColor));
            g_map.setmapposition(position(82 - i, 126), image(' ', charColor));
            g_map.setmapposition(position(87 - i, 126), image(' ', charColor));
            g_map.setmapposition(position(92 - i, 127), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 127), image(' ', charColor));
            g_map.setmapposition(position(92 - i, 126), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 126), image(' ', charColor));
            g_map.setmapposition(position(67 - i, 129), image(' ', charColor));
            g_map.setmapposition(position(72 - i, 129), image(' ', charColor));
            g_map.setmapposition(position(67 - i, 128), image(' ', charColor));
            g_map.setmapposition(position(72 - i, 128), image(' ', charColor));
            g_map.setmapposition(position(77 - i, 129), image(' ', charColor));
            g_map.setmapposition(position(82 - i, 129), image(' ', charColor));
            g_map.setmapposition(position(77 - i, 128), image(' ', charColor));
            g_map.setmapposition(position(82 - i, 128), image(' ', charColor));
            g_map.setmapposition(position(87 - i, 129), image(' ', charColor));
            g_map.setmapposition(position(92 - i, 129), image(' ', charColor));
            g_map.setmapposition(position(87 - i, 128), image(' ', charColor));
            g_map.setmapposition(position(92 - i, 128), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 129), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 128), image(' ', charColor));
            g_map.setmapposition(position(62 - i, 131), image(' ', charColor));
            g_map.setmapposition(position(67 - i, 131), image(' ', charColor));
            g_map.setmapposition(position(62 - i, 130), image(' ', charColor));
            g_map.setmapposition(position(67 - i, 130), image(' ', charColor));
            g_map.setmapposition(position(72 - i, 131), image(' ', charColor));
            g_map.setmapposition(position(77 - i, 131), image(' ', charColor));
            g_map.setmapposition(position(72 - i, 130), image(' ', charColor));
            g_map.setmapposition(position(77 - i, 130), image(' ', charColor));
            g_map.setmapposition(position(82 - i, 131), image(' ', charColor));
            g_map.setmapposition(position(87 - i, 131), image(' ', charColor));
            g_map.setmapposition(position(82 - i, 130), image(' ', charColor));
            g_map.setmapposition(position(87 - i, 130), image(' ', charColor));
            g_map.setmapposition(position(92 - i, 131), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 131), image(' ', charColor));
            g_map.setmapposition(position(92 - i, 130), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 130), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 134), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 134), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 133), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 133), image(' ', charColor));
            g_map.setmapposition(position(92 - i, 134), image(' ', charColor));
            g_map.setmapposition(position(92 - i, 133), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 132), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 132), image(' ', charColor));
            g_map.setmapposition(position(92 - i, 132), image(' ', charColor));
            g_map.setmapposition(position(87 - i, 132), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 131), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 130), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 129), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 128), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 127), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 126), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 125), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 125), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 124), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 124), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 123), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 123), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 122), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 122), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 121), image(' ', charColor));
            g_map.setmapposition(position(97 - i, 121), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 120), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 119), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 118), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 117), image(' ', charColor));
            g_map.setmapposition(position(102 - i, 116), image(' ', charColor));
            g_map.setmapposition(position(136 - i, 146), image(' ', charColor));
            g_map.setmapposition(position(131 - i, 146), image(' ', charColor));
            g_map.setmapposition(position(126 - i, 146), image(' ', charColor));
            g_map.setmapposition(position(121 - i, 146), image(' ', charColor));
            g_map.setmapposition(position(116 - i, 146), image(' ', charColor));
            g_map.setmapposition(position(136 - i, 147), image(' ', charColor));
            g_map.setmapposition(position(131 - i, 147), image(' ', charColor));
            g_map.setmapposition(position(126 - i, 147), image(' ', charColor));
            g_map.setmapposition(position(121 - i, 147), image(' ', charColor));
            g_map.setmapposition(position(116 - i, 147), image(' ', charColor));
            g_map.setmapposition(position(136 - i, 148), image(' ', charColor));
            g_map.setmapposition(position(131 - i, 148), image(' ', charColor));
            g_map.setmapposition(position(126 - i, 148), image(' ', charColor));
            g_map.setmapposition(position(121 - i, 148), image(' ', charColor));
            g_map.setmapposition(position(136 - i, 149), image(' ', charColor));
            g_map.setmapposition(position(131 - i, 149), image(' ', charColor));
            g_map.setmapposition(position(126 - i, 149), image(' ', charColor));
            g_map.setmapposition(position(121 - i, 149), image(' ', charColor));
            g_map.setmapposition(position(136 - i, 150), image(' ', charColor));
            g_map.setmapposition(position(131 - i, 150), image(' ', charColor));
            g_map.setmapposition(position(126 - i, 150), image(' ', charColor));
            g_map.setmapposition(position(121 - i, 150), image(' ', charColor));
            g_map.setmapposition(position(131 - i, 152), image(' ', charColor));
            g_map.setmapposition(position(126 - i, 152), image(' ', charColor));
            g_map.setmapposition(position(121 - i, 152), image(' ', charColor));
            g_map.setmapposition(position(131 - i, 153), image(' ', charColor));
            g_map.setmapposition(position(126 - i, 153), image(' ', charColor));
            g_map.setmapposition(position(121 - i, 153), image(' ', charColor));
            g_map.setmapposition(position(131 - i, 154), image(' ', charColor));
            g_map.setmapposition(position(126 - i, 154), image(' ', charColor));
            g_map.setmapposition(position(121 - i, 154), image(' ', charColor));
            g_map.setmapposition(position(126 - i, 155), image(' ', charColor));
            g_map.setmapposition(position(126 - i, 156), image(' ', charColor));
            g_map.setmapposition(position(126 - i, 157), image(' ', charColor));
            g_map.setmapposition(position(137 - i, 137), image(' ', charColor));
            g_map.setmapposition(position(137 - i, 136), image(' ', charColor));
            g_map.setmapposition(position(137 - i, 135), image(' ', charColor));
            g_map.setmapposition(position(137 - i, 134), image(' ', charColor));
            g_map.setmapposition(position(137 - i, 133), image(' ', charColor));
            g_map.setmapposition(position(137 - i, 132), image(' ', charColor));
            g_map.setmapposition(position(121 - i, 131), image(' ', charColor));
            g_map.setmapposition(position(121 - i, 130), image(' ', charColor));
            g_map.setmapposition(position(121 - i, 129), image(' ', charColor));
            g_map.setmapposition(position(121 - i, 128), image(' ', charColor));
            g_map.setmapposition(position(121 - i, 127), image(' ', charColor));
            g_map.setmapposition(position(116 - i, 131), image(' ', charColor));
            g_map.setmapposition(position(116 - i, 130), image(' ', charColor));
            g_map.setmapposition(position(116 - i, 129), image(' ', charColor));
            g_map.setmapposition(position(108 - i, 131), image(' ', charColor));
        }
        for (int i = 0; i < 35; i++)
        {
            g_map.setmapposition(position(103 + i, 138), image(' ', charColor));
            g_map.setmapposition(position(103 + i, 139), image(' ', charColor));
            g_map.setmapposition(position(103 + i, 140), image(' ', charColor));
        }
        entities[0]->setpos(position(79, 197), g_map);
        WORD charColor = 0x0D;
        g_map.setmapposition(position(131, 156), image('T', charColor));
    }
    else if (maplevel == 4)
    {
        wallskin = image(NULL, 96);
        backgroundchange("nature", "nature", "clear");
        entities[1] = new virus_spawner(position(75, 145), 0.1f, g_map);
        entities[2] = new virus_spawner(position(122, 105), 0.1f, g_map);
        entities[3] = new virus_spawner(position(78, 137), 0.1f, g_map);
        entities[4] = new virus_spawner(position(62, 152), 0.1f, g_map);
        entities[5] = new virus_spawner(position(64, 98), 0.1f, g_map);
        entities[6] = new virus_spawner(position(63, 75), 0.1f, g_map);
        entities[7] = new virus_spawner(position(63, 75), 0.1f, g_map);
        entities[8] = new virus_spawner(position(150, 108), 0.1f, g_map);
        entities[9] = new virus_spawner(position(83, 141), 0.1f, g_map);
        entities[10] = new virus_spawner(position(114, 88), 0.1f, g_map);
        entities[11] = new virus_spawner(position(85, 79), 0.1f, g_map);
        entities[12] = new lightning(position(91, 170), fg_map);
        entities[13] = new NPC(position(95, 180), 99, 0.5f, image(2, 15), "It's dangerous to pass`with all these lighning and fire`Take this water gun(click to shoot)", "Walter", g_map);
        entities[14] = new NPC(position(96, 193), 99, 0.25f, image(2, 4), "There's only one left!`You can do it, we are so close!`Cure this planet of its plague", "Worshipper", g_map);
        for (int i = 0; i < 9; i++)
        {
            bg_map.setmapposition(position(86 + i, 175), image(NULL, 144)); //water
        }
        g_map.setmapposition(position(94, 180), image(-87, 241)); //Water Gun
        g_dElapsedTime = 1000.0; // susceptible to changes for level 4
        for (int i = 0; i < 6; i++)
        {
            g_map.setmapposition(position(99, 194+i), image(' ', charColor));
            g_map.setmapposition(position(65, 131 + i), image(' ', charColor));
            g_map.setmapposition(position(72 , 126+i), image(' ', charColor));
            g_map.setmapposition(position(80, 121 + i), image(' ', charColor));
            g_map.setmapposition(position(86, 116 + i), image(' ', charColor));
        }
        for (int i = 0; i < 4; i++)
        {
            g_map.setmapposition(position(95+i, 194), image(' ', charColor));
            g_map.setmapposition(position(91 + i, 161), image(' ', charColor));
            g_map.setmapposition(position(95 + i, 175), image(' ', charColor));
        }
        for (int i = 0; i < 12; i++)
        {
            g_map.setmapposition(position(95, 182+i), image(' ', charColor));
            g_map.setmapposition(position(151, 88 + i), image(' ', charColor));
        }
        for (int i = 0; i < 10; i++)
        {
            g_map.setmapposition(position(85+i, 182), image(' ', charColor));
            g_map.setmapposition(position(91 + i, 130), image(' ', charColor));
            g_map.setmapposition(position(101, 190 + i), image(' ', charColor));
            
        }
        for (int i = 0; i < 19; i++)
        {
            g_map.setmapposition(position(85 , 163+i), image(' ', charColor));
            g_map.setmapposition(position(174, 100+i), image(' ', charColor));
        }
        for (int i = 0; i < 14; i++)
        {
            g_map.setmapposition(position(71+i, 163 ), image(' ', charColor));
            g_map.setmapposition(position(95, 161+i), image(' ', charColor));
        }
        for (int i = 0; i < 9; i++)
        {
            g_map.setmapposition(position(71 , 154+i), image(' ', charColor));
            g_map.setmapposition(position(107 , 119+i), image(' ', charColor));
            
        }
        for (int i = 0; i < 13; i++)
        {
            g_map.setmapposition(position(58+i, 154 ), image(' ', charColor));
            g_map.setmapposition(position(17, 96 + i), image(' ', charColor));
        }
        for (int i = 0; i < 18; i++)
        {
            g_map.setmapposition(position(58 , 136+i), image(' ', charColor));
            g_map.setmapposition(position(113+i, 80 ), image(' ', charColor));
        }
        for (int i = 0; i < 7; i++)
        {
            g_map.setmapposition(position(58+i, 136 ), image(' ', charColor));
            g_map.setmapposition(position(65 + i, 131), image(' ', charColor));
            g_map.setmapposition(position(6 , 109+i), image(' ', charColor));
            
            
        }
        for (int i = 0; i < 6; i++)
        {
            g_map.setmapposition(position(72 + i, 126), image(' ', charColor));
            g_map.setmapposition(position(80+i, 121 ), image(' ', charColor));
            g_map.setmapposition(position(17+i, 96 ), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 128), image(' ', charColor));
        }
        for (int i = 0; i < 80; i++)
        {
            g_map.setmapposition(position(6 + i, 116), image(' ', charColor));
        }
        for (int i = 0; i < 11; i++)
        {
            g_map.setmapposition(position(6+i, 109 ), image(' ', charColor));
            g_map.setmapposition(position(53, 69 + i), image(' ', charColor));
            g_map.setmapposition(position(113 , 69+i), image(' ', charColor));
        }
        for (int i = 0; i < 16; i++)
        {
            g_map.setmapposition(position(23, 80 + i), image(' ', charColor));
        }
        for (int i = 0; i < 30; i++)
        {
            g_map.setmapposition(position(23+i, 80 ), image(' ', charColor));
        }
        for (int i = 0; i < 60; i++)
        {
            g_map.setmapposition(position(53+i, 69 ), image(' ', charColor));
        }
        for (int i = 0; i < 8; i++)
        {
            g_map.setmapposition(position(131 , 80+i), image(' ', charColor));
            g_map.setmapposition(position(72 + i, 126), image(' ', charColor));
        }
        for (int i = 0; i < 20; i++)
        {
            g_map.setmapposition(position(131+i, 88 ), image(' ', charColor));
        }
        for (int i = 0; i < 23; i++)
        {
            g_map.setmapposition(position(151+i, 100 ), image(' ', charColor));
        }
        for (int i = 0; i < 67; i++)
        {

            g_map.setmapposition(position(107 + i, 119), image(' ', charColor));
        }
        for (int i = 0; i < 2; i++)
        {
            g_map.setmapposition(position(101, 128+i), image(' ', charColor));
            g_map.setmapposition(position(99+i, 190 ), image(' ', charColor));
        }
        for (int i = 0; i < 31; i++)
        {
            g_map.setmapposition(position(91, 130 + i), image(' ', charColor));
        }
        for (int i = 0; i < 15; i++)
        {
            g_map.setmapposition(position(99, 175 + i), image(' ', charColor));
        }


        //MAZE

        for (int i = 0; i < 5; i++)
        {
            g_map.setmapposition(position(107, 114 + i), image(' ', charColor));
            g_map.setmapposition(position(120 , 114+i), image(' ', charColor));
            g_map.setmapposition(position(86, 111 + i), image(' ', charColor));
            g_map.setmapposition(position(74, 111 + i), image(' ', charColor));
            g_map.setmapposition(position(37, 111 + i), image(' ', charColor));
            g_map.setmapposition(position(26, 111 + i), image(' ', charColor));
            g_map.setmapposition(position(130, 108 + i), image(' ', charColor));
            g_map.setmapposition(position(135+i, 104), image(' ', charColor));
            g_map.setmapposition(position(60 , 79+i), image(' ', charColor));
            
        }
        for (int i = 0; i < 13; i++)
        {
            g_map.setmapposition(position(107+i, 114 ), image(' ', charColor));
            g_map.setmapposition(position(74+i, 111 ), image(' ', charColor));
            g_map.setmapposition(position(56+i, 71 ), image(' ', charColor));
        }
        for (int i = 0; i < 11; i++)
        {
            g_map.setmapposition(position(26 + i, 111), image(' ', charColor));
            g_map.setmapposition(position(163 , 103+i), image(' ', charColor));
            g_map.setmapposition(position(108, 71 + i), image(' ', charColor));
        }
        for (int i = 0; i < 10; i++)
        {
            g_map.setmapposition(position(21 , 98+i), image(' ', charColor));
            
            g_map.setmapposition(position(128 , 82+i), image(' ', charColor));
            g_map.setmapposition(position(75, 74 + i), image(' ', charColor));
            g_map.setmapposition(position(60+i, 84 ), image(' ', charColor));
            g_map.setmapposition(position(60 + i, 79), image(' ', charColor));
        }
        for (int i = 0; i < 6; i++)
        {
            g_map.setmapposition(position(72, 85 + i), image(' ', charColor));
            g_map.setmapposition(position(95, 85 + i), image(' ', charColor));
            g_map.setmapposition(position(73+i, 90 ), image(' ', charColor));
            g_map.setmapposition(position(89 + i, 90), image(' ', charColor));
            g_map.setmapposition(position(70, 79 + i), image(' ', charColor));
            g_map.setmapposition(position(96+i, 85), image(' ', charColor));
        }
        for (int i = 0; i < 7; i++)
        {
            g_map.setmapposition(position(21+i, 98 ), image(' ', charColor));
            g_map.setmapposition(position(69 + i, 74), image(' ', charColor));
            g_map.setmapposition(position(84 + i, 74), image(' ', charColor));
            g_map.setmapposition(position(36, 94+i), image(' ', charColor));
            g_map.setmapposition(position(43, 94 + i), image(' ', charColor));
            g_map.setmapposition(position(37+i, 94 ), image(' ', charColor));
            g_map.setmapposition(position(33, 85+i), image(' ', charColor));
            g_map.setmapposition(position(65, 85 + i), image(' ', charColor));
        }
        for (int i = 0; i < 15; i++)
        {
            g_map.setmapposition(position(27 , 83+i), image(' ', charColor));
            g_map.setmapposition(position(90, 93 + i), image(' ', charColor));
            g_map.setmapposition(position(135, 95 + i), image(' ', charColor));
            g_map.setmapposition(position(140, 95 + i), image(' ', charColor));
            g_map.setmapposition(position(96, 77 + i), image(' ', charColor));
            g_map.setmapposition(position(101, 77 + i), image(' ', charColor));
        }
        
        for (int i = 0; i < 29; i++)
        {
            g_map.setmapposition(position(27+i, 83 ), image(' ', charColor));
        }
        for (int i = 0; i < 12; i++)
        {
            g_map.setmapposition(position(56, 71+i), image(' ', charColor));
            g_map.setmapposition(position(146, 91 + i), image(' ', charColor));
        }
        for (int i = 0; i < 3; i++)
        {
            g_map.setmapposition(position(69, 71 + i), image(' ', charColor));
            g_map.setmapposition(position(75, 71 + i), image(' ', charColor));
            g_map.setmapposition(position(84 , 71 + i), image(' ', charColor));
            g_map.setmapposition(position(90, 71 + i), image(' ', charColor));
        }
        for (int i = 0; i < 9; i++)
        {
            g_map.setmapposition(position(75+i, 71), image(' ', charColor));
            g_map.setmapposition(position(96 , 95+i), image(' ', charColor));
            g_map.setmapposition(position(127, 95 + i), image(' ', charColor));
        }
        
        for (int i = 0; i < 18; i++)
        {
            g_map.setmapposition(position(90+i, 71 ), image(' ', charColor));
            g_map.setmapposition(position(128 + i, 91), image(' ', charColor));
        }
        for (int i = 0; i < 20; i++)
        {
            g_map.setmapposition(position(109+i , 82), image(' ', charColor));
        }
        for (int i = 0; i < 17; i++)
        {
            g_map.setmapposition(position(146 + i, 103), image(' ', charColor));
            
        }
        for (int i = 0; i < 24; i++)
        {
            g_map.setmapposition(position(72 + i, 84), image(' ', charColor));
        }
        for (int i = 0; i < 33; i++)
        {
            g_map.setmapposition(position(130 + i, 113), image(' ', charColor));
        }
        for (int i = 0; i < 40; i++)
        {
            g_map.setmapposition(position(90 + i, 108), image(' ', charColor));
        }
        for (int i = 0; i < 4; i++)
        {
            g_map.setmapposition(position(90 + i, 108), image(' ', charColor));
            g_map.setmapposition(position(86 , 89+i), image(' ', charColor));
            g_map.setmapposition(position(68, 89 + i), image(' ', charColor));
            g_map.setmapposition(position(32, 102 + i), image(' ', charColor));
            g_map.setmapposition(position(53, 102 + i), image(' ', charColor));
        }
        
        for (int i = 0; i < 22; i++)
        {
            g_map.setmapposition(position(68 + i, 93), image(' ', charColor));
            g_map.setmapposition(position(62 + i, 104), image(' ', charColor));
            g_map.setmapposition(position(62 + i, 105), image(' ', charColor));
            g_map.setmapposition(position(32 + i, 102), image(' ', charColor));
            g_map.setmapposition(position(32 + i, 105), image(' ', charColor));
        }

        for (int i = 0; i < 31; i++)
        {
            g_map.setmapposition(position(96 + i, 95), image(' ', charColor));
            g_map.setmapposition(position(96 + i, 103), image(' ', charColor));
        }
        

        
        for (int i = 0; i < 52; i++)
        {
            g_map.setmapposition(position(22 + i, 107), image(' ', charColor));
        }

        for (int i = 0; i < 33; i++)
        {
            g_map.setmapposition(position(33 + i, 85), image(' ', charColor));
            g_map.setmapposition(position(33 + i, 92), image(' ', charColor));
        }
        for (int i = 0; i < 8; i++)
        {
            g_map.setmapposition(position(36, 94 + i), image(' ', charColor));
            g_map.setmapposition(position(43, 94 + i), image(' ', charColor));
        }
        for (int i = 0; i < 26; i++)
        {
            g_map.setmapposition(position(101 + i, 84 ), image(' ', charColor));
            g_map.setmapposition(position(101 + i, 93), image(' ', charColor));
        }
        g_map.setmapposition(position(108, 82), image(' ', charColor));
        g_map.setmapposition(position(101, 130), image(' ', charColor));
        g_map.setmapposition(position(107, 128), image(' ', charColor));
        g_map.setmapposition(position(174, 119), image(' ', charColor));
        g_map.setmapposition(position(53, 80), image(' ', charColor));
        g_map.setmapposition(position(56, 83), image(' ', charColor));
        g_map.setmapposition(position(23, 96), image(' ', charColor));
        g_map.setmapposition(position(17, 109), image(' ', charColor));
        WORD charColor = 0x0D;
        g_map.setmapposition(position(85, 87), image('T', charColor));

        entities[0]->setpos(position(100, 199), g_map);
    }
    else if (maplevel == 5)
    {
        g_dElapsedTime = 999;
        g_player->setpos(position(39, 18), g_map);
        bgc_map.fill(bgc_images_space, size(bgc_images_space), bgc_weightage_space);
        bg_map.fill(bg_images_space, size(bg_images_space), bg_weightage_space);
        for (int i = 0; i < 25; i++)
        {
            g_map.setmapposition(position(79, i), image(NULL, 240));
        }
        for (int i = 0; i < 79; i++)
        {
            g_map.setmapposition(position(i, 24), image(NULL, 240));
        }
        entities[1] = new boss(position(39, 3), 1, g_map);
        //entities[2] = new NPC(position(96, 193), 99, 0.25f, image(2, 4), "Finally, we can save the world`curing it of the plague...` THAT IS HUMANITY", "Cult Worshipper", g_map);
    }
}

void renderOver() // render game over screen
{
    std::ostringstream ss;
    std::string key;
    ss.str("");
    ss << "GAME OVER!";
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - ss.tellp();
    g_Console.writeToBuffer(c, ss.str(), 0x03);
    if (g_skKeyEvent[K_ENTER].keyReleased)
    {
        Restart();
        memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    }
    ss.str("");
    ss << "Press <Enter> to restart";
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(c, ss.str(), 0x03);
}

void renderTrans() // render map transition screen (level 1 .. level 2 ..)
{
    std::ostringstream ss;
    std::string key;
    ss.str("");
    COORD c = g_Console.getConsoleSize();
    if (maplevel == 1)
    {
        ss << "Level " << maplevel << ": Tutorial Toad";
        c.Y /= 3;
        c.X = c.X / 2 - ss.tellp() + 12;
        g_Console.writeToBuffer(c, ss.str(), 0x03);
    }
    else if (maplevel == 2)
    {
        ss << "Level " << maplevel << ": Kobe Bryant";
        c.Y /= 3;
        c.X = c.X / 2 - ss.tellp() + 10;
        g_Console.writeToBuffer(c, ss.str(), 0x03);
    }
    else if (maplevel == 3)
    {
        ss << "Level " << maplevel << ": Black Lives Matter";
        c.Y /= 3;
        c.X = c.X / 2 - ss.tellp() + 17;
        g_Console.writeToBuffer(c, ss.str(), 0x03);
    }
    else if (maplevel == 4)
    {
        ss << "Level " << maplevel << ": Team Trees";
        c.Y /= 3;
        c.X = c.X / 2 - ss.tellp() + 9;
        g_Console.writeToBuffer(c, ss.str(), 0x03);
    }
    else if (maplevel == 5)
    {
        ss << "L3^el " << maplevel << "= ???";
        c.Y /= 3;
        c.X = (c.X - ss.tellp()) / 2;
        g_Console.writeToBuffer(c, ss.str(), 0x03);
    }

    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        g_eGameState = S_GAME;
        memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    }
    ss.str("");
    ss << "Click to continue";
    c.Y += 1;
    c.X = (g_Console.getConsoleSize().X - ss.tellp() )/ 2;
    g_Console.writeToBuffer(c, ss.str(), 0x03);
}

void renderWin() // render win screen
{
    
    std::ostringstream ss;
    std::string key;
    ss.str("");
    ss << "YOU WIN!";
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - ss.tellp();
    g_Console.writeToBuffer(c, ss.str(), 0x03);
    if (g_skKeyEvent[K_ENTER].keyReleased)
    {
        Restart();
        g_eGameState = S_MAIN;
        memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
        
    }
    ss.str("");
    ss << "Press <Enter> to go to main menu";
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(c, ss.str(), 0x03);
}

void renderPause() // render pause screen
{
    std::ostringstream ss;
    std::string key;
    ss.str("");
    ss << "Resume";
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - ss.tellp();
    g_Console.writeToBuffer(c, ss.str(), 0x03);
    if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && (g_mouseEvent.mousePosition.X >= c.X) && (g_mouseEvent.mousePosition.X <= c.X + ss.tellp() - 1) && (g_mouseEvent.mousePosition.Y == c.Y))
    {
        g_eGameState = S_GAME;
        g_mouseEvent.buttonState = 0;
    }
    ss.str("");
    ss << "Restart";
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - ss.tellp() + 1;
    g_Console.writeToBuffer(c, ss.str(), 0x03);
    if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && (g_mouseEvent.mousePosition.X >= c.X) && (g_mouseEvent.mousePosition.X <= c.X + ss.tellp() - 1) && (g_mouseEvent.mousePosition.Y == c.Y))
    {
        g_eGameState = S_RESTART;
        g_mouseEvent.buttonState = 0;
    }
    ss.str("");
    ss << "Exit to main menu";
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - ss.tellp() + 11;
    g_Console.writeToBuffer(c, ss.str(), 0x03);
    if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && (g_mouseEvent.mousePosition.X >= c.X) && (g_mouseEvent.mousePosition.X <= c.X + ss.tellp() - 1) && (g_mouseEvent.mousePosition.Y == c.Y))
    {
        Restart();
        g_eGameState = S_MAIN;
        g_mouseEvent.buttonState = 0;
    }
}

void Restart() // restarts the game back to level 1
{
    maplevel = 1;
    mapchange(maplevel);
    shutdown();
    init();
    g_eGameState = S_GAME;
}

void mainMenu()
{
    std::ostringstream ss;
    std::string key;
    ss.str("");
    ss << "Mask of Yendor";
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - ss.tellp() +5;
    g_Console.writeToBuffer(c, ss.str(), 0x04);
    ss.str("");
    ss << "Start";
    c.Y += 3;
    c.X = g_Console.getConsoleSize().X / 2 - ss.tellp();
    g_Console.writeToBuffer(c, ss.str(), 0x03);
    if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && (g_mouseEvent.mousePosition.X >= c.X) && (g_mouseEvent.mousePosition.X <= c.X + ss.tellp() - 1) && (g_mouseEvent.mousePosition.Y == c.Y))
    {
        g_eGameState = S_TUTORIAL;
        g_mouseEvent.buttonState = 0;
    }
    ss.str("");
    ss << "Exit";
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - ss.tellp() - 1;
    g_Console.writeToBuffer(c, ss.str(), 0x03);
    if ((g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) && (g_mouseEvent.mousePosition.X >= c.X) && (g_mouseEvent.mousePosition.X <= c.X + ss.tellp() - 1) && (g_mouseEvent.mousePosition.Y == c.Y))
    {
        g_bQuitGame = true;
    }
}

void rendertutorialscreen() // prints out instructions
{
    std::ostringstream ss;
    std::string key;
    ss.str("");
    ss << "Instructions:";
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - ss.tellp();
    g_Console.writeToBuffer(c, ss.str(), 0x03);
    ss.str("");
    ss << "W-A-S-D to move up/left/down/right";
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, ss.str(), 0x03);
    ss.str("");
    ss << "Goal is to collect the mask and avoid the virus!";
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, ss.str(), 0x03);
    ss.str("");
    ss << "Press <Enter> to continue";
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, ss.str(), 0x03);
    if (g_skKeyEvent[K_ENTER].keyReleased)
    {
        g_eGameState = S_MAPT;
        memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    }
}

void updatePause() // unpause 
{
    
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
    {
        g_eGameState = S_GAME;
        memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    }
}

void backgroundchange(string bg, string bgc, string fg) {
    if (bg == "nature")
    {
        bg_map.fill(bg_images_nature, size(bg_images_nature), bg_weightage_nature);
    }
    else if (bg == "space")
    {
        bg_map.fill(bg_images_space, size(bg_images_space), bg_weightage_space);
    }
    else if (bg == "white")
    {
        bg_map.fill(bg_images_white, size(bg_images_white), bg_weightage_white);
    }
    else if (bg == "clear")
    {
        bg_map.clearmap();
    }
    else if (bg == "yellow")
    {
        bg_map.fill(bg_images_yellow, size(bg_images_yellow), bg_weightage_yellow);
    }
    else if (bg == "black")
    {
        bg_map.fill(bg_images_black, size(bg_images_black), bg_weightage_black);
    }
    else if (bg == "brown")
    {
        bg_map.fill(bg_images_brown, size(bg_images_brown), bg_weightage_brown);
    }

    if (bgc == "nature")
    {
        bgc_map.fill(bgc_images_nature, size(bgc_images_nature), bgc_weightage_nature);
    }
    else if (bgc == "space")
    {
        bgc_map.fill(bgc_images_space, size(bgc_images_space), bgc_weightage_space);
    }
    else if (bgc == "clear")
    {
        bgc_map.clearmap();
    }

    if (fg == "white")
    {
        fg_map.fill(bg_images_white, size(bg_images_white), bg_weightage_white);
    }
    else if (fg == "clear")
    {
        fg_map.clearmap();
    }
}

/*list of colours used:
g_map
240  -> walls (fg: NULL    bg: white    text: NULL)
213  -> virus (fg: purple  bg: magenta  text: 15)
  0  -> player(fg: light_green bg: NULL text: 1)
0x0B -> mask  (fg: white   bg: NULL     text: 'M')
  0  -> nothing(fg: NULL   bg: NULL     text: NULL)
  8  -> boulder(fg: grey   bg: NULL     text: '#')
241  -> Water Gun(fg: blue bg: NULL     text: -87)
bgc_map
reds -> fire  (fg: reds    bg: reds     text: -21)
bg_map
144  -> water (fg: NULL    bg: blue     text: NULL)
*/