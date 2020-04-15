#include "sound/SoundSource.hpp"
#include "sound/SoundDevice.hpp"
#include "sound/Sound.hpp"

int main(int argc, char** argv)
{
	magmatic::sound::SoundDevice device;
	auto buffer = magmatic::sound::Sound::open("VORBIS", "examples/resources/bit-sound.ogg");
	magmatic::sound::SoundSource source({200000000, 2000000000, -200000000});
	source.setSound(buffer);
	source.play();
	source.play();
	source.wait();
}