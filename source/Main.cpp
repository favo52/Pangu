//#include "Game.h"
#include "Application.h"

#include <stdexcept>
#include <iostream>

int main(int argc, char* argv)
try
{
    //pangu::Game game;
    //game.run();

    Pangu::Application App;
    App.Run();

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << "ERROR: " << e.what() << std::endl;
    return -1;
}