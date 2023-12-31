#include "Vessel.h"
#include <random>

CVessel::CVessel()
{
	std::random_device rd;
	std::uniform_int_distribution<uint64_t> dist;
	m_nId = dist(rd);
}

CVessel::~CVessel()
{
}
