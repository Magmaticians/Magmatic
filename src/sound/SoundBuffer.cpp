#include "sound/SoundBuffer.hpp"
#include <iostream>

magmatic::sound::SoundBuffer::~SoundBuffer()
{
	std::cout <<"eluwina\n"<<std::endl;
	alDeleteBuffers(1, &buffer_ID);
}