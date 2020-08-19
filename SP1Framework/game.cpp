// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>

double  g_dElapsedTime;
double  g_dDeltaTime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state
<<<<<<< Updated upstream
=======
map g_map = map(200, 200, position(0,0), position(80, 25));
float virusspawntime;
float virusspawntimer;
const int MAXENTITY = 50;
entity* entities[MAXENTITY]; //stores all entities that move
image previmg; //the img under the player
WORD solids[] = {240}; //list of solid objects that will stop movement, add the colour here
//current list: 240 =   white  = walls
//WORD triggers[] = {}; TODO onTriggersEnter
>>>>>>> Stashed changes

// Console object
Console g_Console(80, 25, "SP1 Framework");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;    

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;
    g_sChar.m_bActive = true;
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler); 
    renderWall();
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
    memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
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
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
        case S_GAME: updateGame(); // gameplay logic when we are in the game
            break;
    }
}


void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 3.0) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
}

void updateGame()       // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
}

void moveCharacter()
{    
    // Updating the location of the character based on the key release
    // providing a beep sound whenver we shift the character
    if (g_skKeyEvent[K_UP].keyReleased && g_sChar.m_cLocation.Y > 0)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.Y--;       
    }
    if (g_skKeyEvent[K_LEFT].keyReleased && g_sChar.m_cLocation.X > 0)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.X--;        
    }
    if (g_skKeyEvent[K_DOWN].keyReleased && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.Y++;        
    }
<<<<<<< Updated upstream
    if (g_skKeyEvent[K_RIGHT].keyReleased && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
        //Beep(1440, 30);
        g_sChar.m_cLocation.X++;        
    }
=======
    if (static_cast<WORD>(g_map.getmapposition(futurloc).getcolour()) == static_cast<WORD>(0x0B)) // mask collision
    {
        maplevel++;
        maskrenderout();
        
        mapchange(maplevel);
        
    }

    //rendering
    position prevloc = entities[0]->getpos();
    g_map.setmapposition(prevloc, previmg);
    previmg = image(NULL, 0);
    //previmg = g_map.getmapposition(futurloc);
    entities[0]->setpos(futurloc, g_map);

    //changing symbol of player
>>>>>>> Stashed changes
    if (g_skKeyEvent[K_SPACE].keyReleased)
    {
        g_sChar.m_bActive = !g_sChar.m_bActive;        
    }

   
}
void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
        g_bQuitGame = true;    
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
    case S_SPLASHSCREEN: renderSplashScreen();
        break;
    case S_GAME: renderGame();
        break;
    }
    renderFramerate();      // renders debug information, frame rate, elapsed time, etc
    renderInputEvents();    // renders status of input events
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game

}



void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x1F);
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
<<<<<<< Updated upstream
    renderCharacter();  // renders the character into the buffer
=======
    renderMask();
   
>>>>>>> Stashed changes
}



void renderMap()
{
    // Set up sample colours, and output shadings
<<<<<<< Updated upstream
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    COORD c;
    for (int i = 0; i < 12; ++i)
    {
        c.X = 5 * i;
        c.Y = i + 1;
        colour(colors[i]);
        g_Console.writeToBuffer(c, " °±²Û", colors[i]);
    }
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0C;
    if (g_sChar.m_bActive)
    {
        charColor = 0x0A;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, charColor);
=======
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
    
    //rendering the map
    
    
        COORD c;
        g_map.centerOnPlayer(entities[0]->getpos());
        for (int x = g_map.getcampos().get('x'), x0 = 0; x < g_map.getcampos().get('x') + g_map.getcamsize().get('x'); x++, x0++)
        {
            for (int y = g_map.getcampos().get('y'), y0 = 0; y < g_map.getcampos().get('y') + g_map.getcamsize().get('y'); y++, y0++)
            {
                if (x >= 0 && y >= 0 && x < g_map.getmapsize('x') && y <= g_map.getmapsize('y'))
                {
                    c.X = x0;
                    c.Y = y0;
                    g_Console.writeToBuffer(c, g_map.getmapposition(position(x, y)).gettext(), g_map.getmapposition(position(x, y)).getcolour());
                }
            }
        }

        
    
    
    
    
    
>>>>>>> Stashed changes
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
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x59);
}

// this is an example of how you would use the input events
void renderInputEvents()
{
    // keyboard events
    COORD startPos = {50, 2};
    std::ostringstream ss;
    std::string key;
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
    }

    // mouse events    
    ss.str("");
<<<<<<< Updated upstream
    ss << "Mouse position (" << g_mouseEvent.mousePosition.X << ", " << g_mouseEvent.mousePosition.Y << ")";
    g_Console.writeToBuffer(g_mouseEvent.mousePosition, ss.str(), 0x59);
=======
    ss << "x= "<<g_mouseEvent.mousePosition.X + g_map.getcampos().get('x')<<" y= "<< g_mouseEvent.mousePosition.Y + g_map.getcampos().get('y');
    g_Console.writeToBuffer(g_mouseEvent.mousePosition, ss.str(), 0x49);
>>>>>>> Stashed changes
    ss.str("");
    switch (g_mouseEvent.eventFlags)
    {
    case 0:
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            ss.str("Left Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, ss.str(), 0x59);
        }
        else if (g_mouseEvent.buttonState == RIGHTMOST_BUTTON_PRESSED)
        {
            ss.str("Right Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, ss.str(), 0x59);
        }
        else
        {
            ss.str("Some Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 3, ss.str(), 0x59);
        }
        break;
    case DOUBLE_CLICK:
        ss.str("Double Clicked");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 4, ss.str(), 0x59);
        break;        
    case MOUSE_WHEELED:
        if (g_mouseEvent.buttonState & 0xFF000000)
            ss.str("Mouse wheeled down");
        else
            ss.str("Mouse wheeled up");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 5, ss.str(), 0x59);
        break;
    default:        
        break;
    }
    
}

<<<<<<< Updated upstream
=======
void renderMask()
{
    if (maplevel == 1)
    {
        WORD charColor = 0x0B;
        g_map.setmapposition(position(10, 10), image('M', charColor));
    }
    else if (maplevel == 2)
    {
        
        WORD charColor = 0x0B;
        g_map.setmapposition(position(20, 10), image('M', charColor));
        
    }
    else if (maplevel == 3)
    {

        WORD charColor = 0x0B;
        g_map.setmapposition(position(30, 10), image('M', charColor));
    }
    else if (maplevel == 4)
    {

        WORD charColor = 0x0B;
        g_map.setmapposition(position(40, 10), image('M', charColor));
    }
}

//render border walls
void renderWall()
{
    for (int i = 0; i < g_map.getmapsize('x'); i++)
    {
        WORD charColor = 240; //bg white
        g_map.setmapposition(position(i, g_map.getmapsize('y')), image(' ', charColor)); //bottom wall border
        g_map.setmapposition(position(i, 0), image(' ', charColor));//top wall border
        for (int i = 0; i < 40; i++)
        {
            g_map.setmapposition(position(180, 5+i), image(' ', charColor));
            
        }
        for (int i = 0; i < 20; i++)
        {
            g_map.setmapposition(position(180+i, 25), image(' ', charColor));
            g_map.setmapposition(position(180 + i, 29), image(' ', charColor));
        }
        for (int i = 0; i < 82; i++)
        {
            g_map.setmapposition(position(98+i, 5), image(' ', charColor));
            g_map.setmapposition(position(98 + i, 6), image(' ', charColor));
        }
        for (int i = 0; i < 7; i++)
        { 

            g_map.setmapposition(position(148 , 6+i), image(' ', charColor));

        }
        for (int i = 0; i < 34; i++)
        {

            g_map.setmapposition(position(146+i, 13), image(' ', charColor));

        }
        for (int i = 0; i < 28; i++)
        {

            g_map.setmapposition(position(91 + i, 9), image(' ', charColor));

        }
        for (int i = 0; i < 12; i++)
        {

            g_map.setmapposition(position(91 , 9+i), image(' ', charColor));

        }
        for (int i = 0; i < 2; i++)
        {

            g_map.setmapposition(position(118, 10 +i), image(' ', charColor));

        }
        for (int i = 0; i < 8; i++)
        {

            g_map.setmapposition(position(118+i, 11), image(' ', charColor));

        }
        for (int i = 0; i < 6; i++)
        {

            g_map.setmapposition(position(125, 11+i), image(' ', charColor));

        }
        for (int i = 0; i < 36; i++)
        {

            g_map.setmapposition(position(125+i, 16), image(' ', charColor));

        }
        for (int i = 0; i < 15; i++)
        {

            g_map.setmapposition(position(160, 16+i), image(' ', charColor));

        }

        for (int i = 0; i < 11; i++)
        {

            g_map.setmapposition(position(150, 20 + i), image(' ', charColor));

        }
        for (int i = 0; i < 10; i++)
        {

            g_map.setmapposition(position(150+i, 30), image(' ', charColor));

        }
        for (int i = 0; i < 10; i++)
        {

            g_map.setmapposition(position(175 + i, 44), image(' ', charColor));

        }
        for (int i = 0; i < 27; i++)
        {

            g_map.setmapposition(position(175 , 17+i), image(' ', charColor));

        }
        for (int i = 0; i < 60; i++)
        {
            g_map.setmapposition(position(91+i, 20), image(' ', charColor));
        }

        for (int i = 0; i < g_map.getmapsize('y'); i++)
        {
            g_map.setmapposition(position(0, i), image(' ', charColor)); //left wall border
            g_map.setmapposition(position(g_map.getmapsize('x')-1, i), image(' ', charColor)); //right wall border
        }
    }
}



void maskrenderout()
{
    WORD charColor = 0x00;
    g_map.setmapposition(position(10, 10), image('M', charColor));
}

void mapchange(int x)
{
    g_map.clearmap();
    if (maplevel == 2)
    {
        
        
        for (int i = 0; i < 5; i++)
        {

            g_map.setmapposition(position(i, 1), image(' ', 240));
        }
        
        entities[0]->setpos(position(9, 9), g_map);
        
    }
    if (maplevel == 3)
    {
        g_map.setmapposition(position(20, 1), image(' ', 240));

    }
    if (maplevel == 4)
    {
        g_map.setmapposition(position(30, 1), image(' ', 240));

    }
}

void spawnvirus() {
    for (int i = 0; i < size(entities); i++)
    {
        if (entities[i] == NULL)
        {
            entities[i] = new virus(0.5f, g_map);
            break;
        }
    }
}

/*list of colours used:
240 -> walls (fg: NULL    bg: white)
  5 -> virus (fg: purple  bg: NULL)
 10 -> player(fg: light_green bg: NULL)

>>>>>>> Stashed changes


