#include <Game.h>

#include <iostream>

int main(int argc, char** argv)
try
{
    pangu::Game game;
    game.run();

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << "ERROR: " << e.what() << std::endl;
    return -1;
}
catch (...)
{
    std::cerr << "Unknown error!" << std::endl;
    return -2;
}