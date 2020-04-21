#include "sound/SoundSource.hpp"
#include "sound/SoundDevice.hpp"
#include "sound/Sound.hpp"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

void orbitingSound(const magmatic::sound::SoundSource& source)
{
	constexpr int orbit_size = 30;
	source.setPosition({0,0, 0});
	source.play();
	source.setGain(3);
	float pos = 0.0;
	while(source.state() == magmatic::sound::SoundSource::Playing)
	{
		source.setPosition({orbit_size*std::sin(pos), orbit_size*std::cos(pos), 0});
		std::this_thread::sleep_for(200ms);
		pos += 0.3;
	}
}

void fadingSound(const magmatic::sound::SoundSource& source)
{
	source.setPosition({0,0, 0});
	source.play();
	source.setGain(1);
	float pos = 0.0;
	while(source.state() == magmatic::sound::SoundSource::Playing)
	{
		source.setPosition({pos, pos, 0});
		std::this_thread::sleep_for(200ms);
		pos += 0.3;
	}
}

int main(int argc, char** argv)
{
	magmatic::sound::SoundDevice device;
	auto buffer = magmatic::sound::Sound::open("VORBIS", "examples/resources/bit-sound-mono.ogg");
	magmatic::sound::SoundSource source({0, 0, 0});
	//https://freesound.org/people/hmmm101/sounds/335521/
	source.setSound(buffer);

	orbitingSound(source);
	fadingSound(source);
}