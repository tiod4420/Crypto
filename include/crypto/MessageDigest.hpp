#ifndef CRYPTO_MESSAGEDIGEST_H
#define CRYPTO_MESSAGEDIGEST_H

#include <cstddef>
#include <cstdint>

namespace Crypto
{

class MessageDigest
{
	public:
		virtual void update(const uint8_t*, std::size_t) = 0;
		virtual void finish(uint8_t*) = 0;
};

template <class MD>
static void MessageDigest_get(const uint8_t *input, std::size_t input_sz, uint8_t *output)
{
	MD ctx;
	ctx.update(input, input_sz);
	ctx.finish(output);
}

}

#endif