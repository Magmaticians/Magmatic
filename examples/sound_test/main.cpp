#include "sound/SoundSource.hpp"
#include "sound/SoundDevice.hpp"
#include "sound/Sound.hpp"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
	magmatic::sound::SoundDevice device;
	auto buffer = magmatic::sound::Sound::open("VORBIS", "examples/resources/bit-sound-mono.ogg");
	auto buffer2 = magmatic::sound::Sound::open("OPUS", "examples/resources/bit-sound.opus");
	magmatic::sound::SoundSource source({0, 0, 0});
	//https://freesound.org/people/hmmm101/sounds/335521/
	source.setSound(buffer);
	source.play();
	float pos = 0;
//	while(source.state() == magmatic::sound::SoundSource::Playing)
//	{
//		source.setPosition({pos, pos, pos});
//		pos += 0.3;
//		std::this_thread::sleep_for(200ms);
//	}
	source.setPosition({0,0, 0});
	source.play();
	while(source.state() == magmatic::sound::SoundSource::Playing)
	{
		source.setPosition({20*std::sin(pos), 20*std::cos(pos), 0});
		std::this_thread::sleep_for(200ms);
		pos += 0.3;
	}

	source.setSound(buffer2);
	source.play();
	source.wait();
}