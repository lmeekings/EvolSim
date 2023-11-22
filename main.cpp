/* This is the program EvolSim, short for Evolutionary Simulator.
* This program has been created for my university project entitled evolving artificial life.
* Author: Lewis James Meekings
*/
#include <iostream>
#include <sstream>
#include "environment.h"
#include "Button.h"
#include <cstdlib>
#include <time.h>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>

using namespace EvolSim;
using std::cout;

int main()
{
    //Simulation settings
    const int width = 10;
    const int height = 10;
    int food = 10;
    srand(time(NULL));
    vector<EvolSim::Creature> speciesList;
    EvolSim::Creature c{ EvolSim::Creature::Eater::herbivore,
                                    "Diplodocus", 3, width, height };
    EvolSim::Creature c2{ EvolSim::Creature::Eater::carnivore,
                                    "Allosaur", 3, width, height };
    speciesList = { c, c2 };
    Environment a{ width, height, 0.3, food, 20, speciesList };

    //Graphical stuff
    float displayWidth = 500.f;
    sf::RenderWindow window(sf::VideoMode(displayWidth * 1.4, displayWidth), "evolsim");
    window.setFramerateLimit(120);
    //Create view
    sf::View view;
    view.setSize(displayWidth, displayWidth);
    view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
    window.setView(view);
    //Tile dimensions
    float tileSizeX = displayWidth / width;
    float tileSizeY = displayWidth / height;

    //Creating the sidebar to display information
    sf::RectangleShape sideBar;
    sideBar.setSize(sf::Vector2f(0.4 * displayWidth, displayWidth));
    sideBar.setPosition(displayWidth, 0);
    sideBar.setFillColor(sf::Color::White);

    //Arrays handling the graphics of tiles and creatures
    sf::RectangleShape tilemap[width][height];
    sf::CircleShape creaturemap[width][height];
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            tilemap[x][y].setSize(sf::Vector2f(tileSizeX, tileSizeY));
            tilemap[x][y].setOutlineThickness(1.f);
            tilemap[x][y].setOutlineColor(sf::Color::White);
            tilemap[x][y].setPosition(x * tileSizeX, y * tileSizeY);

            creaturemap[x][y].setRadius(tileSizeX / 4);
            creaturemap[x][y].setPosition((x+.25) * tileSizeX, (y+.25) * tileSizeY);
        }
    }

    //Creates text to be displayed.
    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
    sf::Text speciesNames{ a.getSpeciesNames(), font, 16};
    sf::Text creatureCount{ "", font, 16 };
    sf::Text deadCreatures{ "", font, 16 };
    sf::Text bornCreatures{ "", font, 16 };
    sf::Text temperature{ "", font, 16 };
    speciesNames.setPosition(displayWidth * 1.05, displayWidth * 0.05);
    speciesNames.setFillColor(sf::Color::Black);
    creatureCount.setPosition(displayWidth * 1.05, displayWidth * (0.05 * (1 + a.getNumSpecies())));
    creatureCount.setFillColor(sf::Color::Black);
    deadCreatures.setPosition(displayWidth * 1.05, displayWidth * (0.05 * (2 + a.getNumSpecies())));
    deadCreatures.setFillColor(sf::Color::Black);
    bornCreatures.setPosition(displayWidth * 1.05, displayWidth * (0.05 * (3 + a.getNumSpecies())));
    bornCreatures.setFillColor(sf::Color::Black);
    temperature.setPosition(displayWidth * 1.05, displayWidth * (0.05 * (4 + a.getNumSpecies())));
    temperature.setFillColor(sf::Color::Black);

    //Creates buttons
    Button desertification{ "Desertification", {110, 40} };
    desertification.setFont(font);
    desertification.setPosition(sf::Vector2f(displayWidth * 1.1, displayWidth * (0.05 * (5 + a.getNumSpecies()))));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (desertification.isMouseOver(window)) {
                    //Do thing
                }
            }
        }

        window.clear();
        window.setView(view);
        window.setView(window.getDefaultView());
        window.draw(sideBar);
        vector<vector<int>> temporaryVector = a.getGridGraphics();
        vector<vector<int>> creatureCounterVector = a.getCreatureGraphics();
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                tilemap[x][y].setFillColor(sf::Color::Color(0, (255/food) * temporaryVector[x][y], 0, 255));
                window.draw(tilemap[x][y]);
                creaturemap[x][y].setFillColor(sf::Color::Color(255, 0, 0, creatureCounterVector[x][y] * 40));
                window.draw(creaturemap[x][y]);
            }
        }
        creatureCount.setString("Live Creatures: " + to_string(a.getLiveCreatures()));
        deadCreatures.setString("Dead Creatures: " + to_string(a.getDeadCreatures()));
        bornCreatures.setString("Born Creatures: " + to_string(a.getBornCreatures()));
        temperature.setString("Temperature: " + to_string(a.getTemperature()));
        window.draw(speciesNames);
        window.draw(creatureCount);
        window.draw(deadCreatures);
        window.draw(bornCreatures);
        window.draw(temperature);
        desertification.drawTo(window);
        window.display();
        a.startCycle();
    }

    return 0;
}
