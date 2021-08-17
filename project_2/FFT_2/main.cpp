/*#include<SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<graphics.h>
#include <iostream>
#include <complex>
#include <valarray>
#include <math.h>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include"FFT.h"

using namespace std ;
using namespace sf ;

int main()
{
	RenderWindow window(VideoMode(1366,768,32),"Window");

	string path ;
	int bufferSize ;
	cout<<"Put an audio file in the Ressources folder (.wav will work, mp3 wont)"<<endl;
	cout<<"Enter the file name (example.wav) : " ;
	cin>>path ;
	cout<<"Enter the buffer size treated by the fft (powers of two works best like 16384)"<<endl;
	cin>>bufferSize ;

	FFT fft("Ressources/"+path,bufferSize) ;

	Event event ;

	while(window.isOpen())
	{
		while(window.pollEvent(event)) {}

		fft.update() ;

		window.clear() ;
		fft.draw(window) ;
		window.display() ;
	}

	return 0;
}*/
/*
#include "FFT.h"
using namespace std;



int main()
{

    sf::RenderWindow window(sf::VideoMode(1366, 768), "SFML works!");
//origin
float x=-200,y=-250;
//vector<float> wave;
//here time is angle in the formula x=r*cos(angle)
float time=0;
int i=390;




    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

//render window
float radius=100;
//big circle(mother circle)
sf::CircleShape circle,scircle;
circle.setOrigin(-200,-250);
circle.setRadius(radius);
circle.setOutlineColor(sf::Color::White);
circle.setOutlineThickness(-3);
circle.setFillColor(sf::Color::Transparent);
circle.setPointCount(1000);
//small circle
float m=radius*cos(time);
float n=radius*sin(time);
//path is circumference of big circle
scircle.setOrigin(m-292,n-343);
scircle.setPosition(circle.getPosition());
scircle.setRadius(7);
scircle.setFillColor(sf::Color::Blue);
scircle.setPointCount(1000);
sf::Vertex line[]{
sf::Vertex(sf::Vector2f(300,350)),
sf::Vertex(sf::Vector2f(-m+295,-n+345))
};
line[0].color=sf::Color::White;
line[1].color=sf::Color::White;
wave w(-n+343);

//wave.push_back(-n+343);

//sf::Vertex vertices[10000];
//vertices[i].position=sf::Vector2f(i,-n+343);
//vertices[i].color=sf::Color::White;








window.clear();
window.draw(line,2,sf::Lines);
window.draw(circle);
window.draw(scircle);
//window.draw(&vertices[i],1366,sf::Points);
w.tw();
w.draw(window);
window.display();

time+=0.03;
//delay the movement
sf::Time t1=sf::milliseconds(5);
sf::sleep(t1);
if(i<1300){
i++;}
else
    i=390;
 i++;
    }
    return 0;
}

*/

#include "main.h"
#include "Audio.h"
#include "Visualizer.h"

const int WINDOW_X = 1280;
const int WINDOW_Y = 720;

const int TEXTURE_X = 0.8*WINDOW_X;
const int TEXTURE_Y = 0.35*WINDOW_Y;

int main() {

	srand(time(NULL)); // creates a new seed for rand()

	sf::Image icon;
	icon.loadFromFile("./images/icon.png"); // File/Image/Pixel
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Audio-Visualizer", sf::Style::Default, settings);
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	window.setFramerateLimit(60);

int dur;
	Audio audio = Audio();
	//int dur = audio.duration.asSeconds();
//audio.playSong();
	std::thread frequencyAnalyzationThread(&Audio::getSampleOverFrequency, &audio);
	Visualizer visualizer = Visualizer();

	//Timeline components
    sf::RectangleShape timeline(sf::Vector2f(TEXTURE_X, 1));
    timeline.setFillColor(sf::Color::Black);
    timeline.setPosition((WINDOW_X-TEXTURE_X)/2, 0.9*WINDOW_Y);
    //Seek
    sf::CircleShape seek(3);
    seek.setPointCount(64);
    seek.setOrigin(3,3);
    seek.setFillColor(sf::Color::Black);

	// Window Loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);

		if (audio.getfrequencyVisualizationVector().size() > 120) {

			visualizer.setAmplitudeVisualizationVector(audio.getAmplitudeVisualizationVector());
			visualizer.update(audio.getfrequencyVisualizationVector(), audio.getSongPlayingOffset());

			// Draws particles
			std::vector <sf::CircleShape> amplitudeParticles = visualizer.getAmplitudeParticles();
			for (int i = 0; i < amplitudeParticles.size(); i++) {
				window.draw(amplitudeParticles[i]);
			}

			// Draws background for freq rect
			window.draw(visualizer.getBackgroundRect());

			// Draws freq visualizer
			std::vector<sf::RectangleShape> freqRangeRects = visualizer.getFreqRangeRects();
			for (int i = 0; i < freqRangeRects.size(); i++) {
				window.draw(freqRangeRects[i]);
			}

			if (!audio.songPlayed()) {
				audio.playSong();

			}
int dur = audio.duration.asSeconds();
		}

		 int now_sec = audio.song.getPlayingOffset().asSeconds();
		 //sf::Vector3f v1 = audio.song.getPosition();
		// int dur = audio.duration.asSeconds();
        int pos = (WINDOW_X-TEXTURE_X)/2+now_sec*TEXTURE_X/dur;
int q=pos;

        //seek.setPosition(v1.x,v1.y);
        //std::cout<<v1.x<<std::endl;
        //std::cout<<v1.y<<std::endl;
//window.clear();

audio.hammingWindow();
audio.draw(window);
 window.draw(timeline);


        seek.setPosition(q,WINDOW_Y*0.9);


        window.draw(seek);
		window.display();
q++;
	}

	return 0;
}

/*
#include "analyser.h"

const int WINDOW_X = 600;
const int WINDOW_Y = 600;
const int WINDOW_FPS = 60;
const int TEXTURE_X = 0.8*WINDOW_X;
const int TEXTURE_Y = 0.35*WINDOW_Y;
const std::string FILENAME = "Naruto.wav";


int main(){
    analyser ana("sounds/" + FILENAME);
    ana.mapBuffer(TEXTURE_Y/2, -TEXTURE_Y/2);
    int dur = ana.duration.asSeconds();


    //Main Window Components
    sf::RenderWindow win(sf::VideoMode(WINDOW_X, WINDOW_Y), FILENAME);
    win.setFramerateLimit(WINDOW_FPS);

    sf::Event ev;                                               //Event Handler


    //Timeline components
    sf::RectangleShape timeline(sf::Vector2f(TEXTURE_X, 1));
    timeline.setFillColor(sf::Color::Black);
    timeline.setPosition((WINDOW_X-TEXTURE_X)/2, 0.9*WINDOW_Y);
    //Seek
    sf::CircleShape seek(3);
    seek.setPointCount(64);
    seek.setOrigin(3,3);
    seek.setFillColor(sf::Color::Black);

    //Graph components
    sf::Vertex vertices[TEXTURE_X];
    for (int i=0; i<TEXTURE_X; i++){
        vertices[i].position.y = TEXTURE_Y/2;
        vertices[i].color = sf::Color(0,0,0,i*255/TEXTURE_X);
    }

    sf::VertexBuffer vb(sf::LineStrip);
    vb.create(TEXTURE_X);


    //Render texture components
    sf::RenderTexture rendergraph;
    rendergraph.create(TEXTURE_X, TEXTURE_Y);
    sf::Sprite graph;
    graph.setTexture(rendergraph.getTexture());
    graph.setPosition((WINDOW_X-TEXTURE_X)/2, (WINDOW_Y-TEXTURE_Y)*0.2);


    ana.sound.play();

    while(win.isOpen()){

        while(win.pollEvent(ev)){
            if (ev.type == 0 || (ev.type == 5 && ev.key.code == 36))
                win.close();
            else if(ev.type == 5 && ev.key.code == 57){
                if(ana.sound.getStatus()==2)
                    ana.sound.pause();
                else
                    ana.sound.play();
            }
        }



        //Update vertex buffer
        for(int i=0; i<TEXTURE_X; i++)
            vertices[i].position = sf::Vector2f(i, vertices[i+1].position.y);

        vertices[TEXTURE_X-1].position = sf::Vector2f(TEXTURE_X-1, TEXTURE_Y/2+ana.getHeight());

        vb.update(vertices);




        int now_sec = ana.sound.getPlayingOffset().asSeconds();
        int pos = (WINDOW_X-TEXTURE_X)/2+now_sec*TEXTURE_X/dur;
        seek.setPosition(pos,WINDOW_Y*0.9);

        rendergraph.clear(sf::Color::White);
        rendergraph.draw(vb);


        win.clear(sf::Color::White);
        win.draw(graph);
        win.draw(timeline);
        win.draw(seek);
        win.display();
    }
}
*/
