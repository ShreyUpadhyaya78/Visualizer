#pragma once
#include<vector>
#include<string>
#include<complex>
#include <valarray>
#define PI 3.141592654
using namespace std;
using namespace sf ;

typedef complex<double> Complex;
typedef valarray<Complex> CArray;

class Audio
{
//setting up variables

    string finalPath;

    const sf::Int16* samples;
    sf::Uint64 sampleSize;
    unsigned int sampleRate;
    int singleChannelSize;

    bool playedSong = false;








public:
  
    void returnPath(string path)
    {
        finalPath=path;
    }

    string songPath=finalPath;

  
    std::string audioFilePath;
    string getPath()
    {
        return(finalPath);
    }
    int _bufferSize=512;
    sf::SoundBuffer buffer;
    sf::Sound song;
    sf::Music music;
    sf::Time duration;
   
    vector<Complex> sample ;
    vector<float> window ;
    CArray bin ;

    VertexArray VA1 ;
    VertexArray VA2 ;
    vector<Vertex> cascade ;

    int sRate ;
    int sampleCount ;
    int bufferSize ;
    int mark ;

    Audio(string path)
    {
//sddress for audio folder
        audioFilePath="./audio/"+path;
        if (!buffer.loadFromFile(audioFilePath))
        {
            std::cout << "Couldn't load buffer" << std::endl;
        }
        else
        {
            //get song data
            song.setBuffer(buffer);
            duration=buffer.getDuration();
            sampleRate = buffer.getSampleRate();
            // Original sample (unsplit: the samples holding the left and right channels)
            samples = buffer.getSamples();
            sampleSize = buffer.getSampleCount();
            singleChannelSize = sampleSize / 2;
        }

        VA1.setPrimitiveType(LineStrip) ;
        sRate = buffer.getSampleRate()*buffer.getChannelCount() ;
        sampleCount = buffer.getSampleCount() ;
        if(_bufferSize < sampleCount) bufferSize = _bufferSize ;
        else bufferSize = sampleCount ;
        mark = 0 ;

        for(int i(0) ; i < bufferSize ; i++) window.push_back(0.54-0.46*cos(2*PI*i/(float)bufferSize)) ;

        sample.resize(bufferSize) ;
        VA1.resize(bufferSize) ;
    }
//wave window
    void hammingWindow()
    {
        mark = song.getPlayingOffset().asSeconds()*sampleRate ;
        if(mark+bufferSize < sampleCount)
        {
            for(int i(mark) ; i < bufferSize+mark ; i++)
            {
                sample[i-mark] = Complex(buffer.getSamples()[i]*window[i-mark],0) ;
                VA1[i-mark] = Vertex(Vector2f(310,475)+Vector2f((i-mark)/(float)bufferSize*700,sample[i-mark].real()*0.005),sf::Color::Black) ;
            }
        }

    }
//render wave

//fft calculating recursive function
    void fft(CArray &x)
    {
        const int N = x.size();
        if(N <= 1) return;

        CArray even = x[slice(0,N/2,2)];
        CArray  odd = x[slice(1,N/2,2)];

        fft(even);
        fft(odd);

        for(int k = 0 ; k < N/2 ; k++)
        {
            Complex t = polar(1.0,-2 * PI * k / N) * odd[k];
            x[k] = even[k] + t;
            x[k+N/2] = even[k] - t;
        }
    }
//main function to call fft function 
    void updateFFT()
    {
        hammingWindow() ;

        VA2.clear() ;


        bin = CArray(sample.data(),bufferSize) ;
        fft(bin) ;
        float max = 100000000 ;


        bars(max);
    }
//function for calculating log bars
    void bars(float const& max)
    {
        VA2.setPrimitiveType(Lines) ;
        Vector2f position(170,260) ;
        for(float i(3) ; i < min(bufferSize/2.f,20000.f) ; i*=1.01)
        {
            Vector2f samplePosition(log(i)/log(min((bufferSize)/2.f,20000.f)),abs(bin[(int)i])) ;
            VA2.append(Vertex(position+Vector2f(samplePosition.x*800,-samplePosition.y/max*15000),sf::Color::Black)) ;
            //for reflection
            VA2.append(Vertex(position+Vector2f(samplePosition.x*800,0),sf::Color::Black)) ;
            VA2.append(Vertex(position+Vector2f(samplePosition.x*800,0),sf::Color(0,0,0,100))) ;
            VA2.append(Vertex(position+Vector2f(samplePosition.x*800,samplePosition.y/max*15000/2.f),sf::Color(0,0,0,0))) ;
        }
    }
//function to draw wave and bars
    void draw(sf::RenderWindow &window)
    {
        window.draw(VA1) ;
        window.draw(VA2);
    }

//check if song is playing
    void playSong()
    {

        song.play();
        music.openFromFile(audioFilePath);
        music.play();
        music.setVolume(0.f);

        song.setLoop(true);
        playedSong = true;
    }

    bool songPlayed()
    {
        return playedSong;
    }



};

