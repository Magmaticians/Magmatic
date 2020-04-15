#include "sound/SoundSource.hpp"
#include "sound/SoundDevice.hpp"
#include "sound/Sound.hpp"

int main(int argc, char** argv)
{
	magmatic::sound::SoundDevice device;
	auto buffer = magmatic::sound::Sound::open("VORBIS", "examples/resources/bit-sound.ogg");
	auto buffer2 = magmatic::sound::Sound::open("OPUS", "examples/resources/bit-sound.opus");
	magmatic::sound::SoundSource source({200000000, 2000000000, -200000000});
	//https://freesound.org/people/hmmm101/sounds/335521/
	source.setSound(buffer);
	source.play();
	source.wait();
	source.setSound(buffer2);
	source.play();
	source.wait();
}