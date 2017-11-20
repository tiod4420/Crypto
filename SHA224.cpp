#include "crypto/Utils.hpp"
#include "crypto/SHA224.hpp"

namespace Crypto
{

SHA224::SHA224(void)
{
	total[0] = 0;
	total[1] = 0;

	state[0] = 0xC1059ED8;
	state[1] = 0x367CD507;
	state[2] = 0x3070DD17;
	state[3] = 0xF70E5939;
	state[4] = 0xFFC00B31;
	state[5] = 0x68581511;
	state[6] = 0x64F98FA7;
	state[7] = 0xBEFA4FA4;
}

void 
SHA224::finish(uint8_t *output)
{
	uint8_t tmp[32];

	SHA256::finish(tmp);
	memcpy(output, tmp, 28);
}

}
