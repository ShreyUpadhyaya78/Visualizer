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
#include <filesystem>
#include<direct.h>
#include<dirent.h>
#include<cstring>
#include<string>
using namespace std;
namespace fs = std::filesystem;


const int WINDOW_X = 1280;
const int WINDOW_Y = 720;

const int TEXTURE_X = 0.8*WINDOW_X;
const int TEXTURE_Y = 0.35*WINDOW_Y;
void noman();
int main() {

      //string path = "./audio";


   // for (auto & p : fs::directory_iterator(path)){
     //   cout << p << endl;

   /// }
   DIR* dir = opendir("./audio");
    if (dir == NULL) {
        return 1;
    }
    struct dirent* entity;
    entity = readdir(dir);
    string path[10000];
    int u=0;
    int inputNum;
    cout<<"Enter the number of song you want to listen!!"<<endl;
    while (entity != NULL) {
if(strcmp(entity->d_name,".")!=0&&strcmp(entity->d_name,"..")!=0){
        path[u]=entity->d_name;
        cout<<u-1<<". "<<path[u]<<endl;}
        u++;
        entity=readdir(dir);

    }
cin>>inputNum;
inputNum+=1;
cout<<"Selected- "<<path[inputNum]<<endl;
string songAddress=path[inputNum];




	srand(time(NULL)); // creates a new seed for rand()

	sf::Image icon;
	icon.loadFromFile("./images/icon.png"); // File/Image/Pixel
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Audio-Visualizer", sf::Style::Default, settings);
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	window.setFramerateLimit(60);

int dur;

	Audio audio = Audio(songAddress);
audio.returnPath(songAddress);



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
sf::Font font;

if(!font.loadFromFile("./Fonts/SF-Pro-Text-Medium.otf")){
    cerr<<"Error loading file"<<endl;
}
int characterSize=32;
sf::Text text1;
sf::Text text2;
sf::Text text3;
text1.setFont(font);
text1.setCharacterSize(characterSize);
text1.setColor(sf::Color::Black);
text1.setString("Log Base Bars");
float text1Width=text1.getGlobalBounds().width;
text1.setPosition((window.getSize().x-text1Width)/2,290.f);
text2.setFont(font);
text2.setCharacterSize(characterSize);
text2.setColor(sf::Color::Black);
text2.setString("Wave");
float text2Width=text2.getGlobalBounds().width;
float text2X=(window.getSize().x-text2Width)/2;
text2.setPosition(text2X,550.f);
text3.setFont(font);
text3.setCharacterSize(characterSize);
text3.setColor(sf::Color::Black);
string songName=audio.getPath();
int len=songName.length();
songName.erase(len-4,songName.length());
text3.setString("Playing: "+songName);
float text3Width=text3.getGlobalBounds().width;
text3.setPosition((window.getSize().x-text3Width)/2,10.f);



	// Window Loopt
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);
if(event.type==sf::Event::KeyPressed){
        //Up and Down to control volume
    if(event.key.code==sf::Keyboard::Key::Down)
        audio.song.setVolume(audio.song.getVolume()-10);
    if(event.key.code==sf::Keyboard::Key::Up)
        audio.song.setVolume(audio.song.getVolume()+10);
        //Left and Right to control tracking position
        if(event.key.code==sf::Keyboard::Key::Right){
            auto newPos=audio.song.getPlayingOffset()+sf::seconds(5);
            audio.song.setPlayingOffset(newPos);}
        if(event.key.code==sf::Keyboard::Key::Left){
            auto newPos=audio.song.getPlayingOffset()-sf::seconds(5);
            audio.song.setPlayingOffset(newPos);}

}
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
audio.hammingWindow();
audio.draw(window);
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


 window.draw(timeline);


        seek.setPosition(q,WINDOW_Y*0.9);


        window.draw(seek);
        window.draw(text1);
        window.draw(text2);
        window.draw(text3);
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
