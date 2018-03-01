#ifndef CRYPTO_GCM_H
#define CRYPTO_GCM_H

#include "crypto/CipherMode.hpp"
#include "crypto/SymmetricCipher.hpp"

#include <cstring>

#if defined(_MSC_VER) || defined(__WATCOMC__)
	#define UL64(x) x##ui64
#else
	#define UL64(x) x##ULL
#endif

#define GET_UINT64(n,b,i)                  \
{                                          \
    (n) = ((uint64_t)(b)[(i)    ] << 56 )  \
        | ((uint64_t)(b)[(i) + 1] << 48 )  \
        | ((uint64_t)(b)[(i) + 2] << 40 )  \
        | ((uint64_t)(b)[(i) + 3] << 32 )  \
        | ((uint64_t)(b)[(i) + 4] << 24 )  \
        | ((uint64_t)(b)[(i) + 5] << 16 )  \
        | ((uint64_t)(b)[(i) + 6] <<  8 )  \
        | ((uint64_t)(b)[(i) + 7]       ); \
}

#define PUT_UINT64(n,b,i)                  \
{                                          \
    (b)[(i)    ] = (uint8_t)((n) >> 56);   \
    (b)[(i) + 1] = (uint8_t)((n) >> 48);   \
    (b)[(i) + 2] = (uint8_t)((n) >> 40);   \
    (b)[(i) + 3] = (uint8_t)((n) >> 32);   \
    (b)[(i) + 4] = (uint8_t)((n) >> 24);   \
    (b)[(i) + 5] = (uint8_t)((n) >> 16);   \
    (b)[(i) + 6] = (uint8_t)((n) >>  8);   \
    (b)[(i) + 7] = (uint8_t)((n)      );   \
}

namespace Crypto
{

template <class SC>
class GCM : public CipherMode
{
	public:
		GCM(const uint8_t *key, std::size_t key_sz, const uint8_t *iv, std::size_t iv_sz,
				const uint8_t *add, std::size_t add_sz, bool is_encrypt)
			: sc_ctx(key, key_sz), add_sz(add_sz), buffer_sz(0), stream_sz(0), total_sz(0), is_encrypt(is_encrypt)
		{
			if ( 16 != SC::BLOCK_SIZE ) {
				throw CipherMode::Exception("Cipher block size not supported");
			}

			if ( (0 == iv_sz) || (0 != (iv_sz >> 61)) ) {
				throw CipherMode::Exception("IV length does not meet length requirements");
			}

			if ( 0 != (add_sz >> 61) ) {
				throw CipherMode::Exception("Additional Input length does not meet length requirements");
			}

			memset(H, 0x00, sizeof(H));
			sc_ctx.encrypt(H, H);

			// Create counter block
			memset(CB, 0x00, sizeof(S));
			if ( 12 == iv_sz ) {
				// J0 = IV || 0...0 || 1
				memcpy(CB, iv, iv_sz);
				CB[15] = 0x01;
			} else {
				// J0 = GHASH_H(IV || 0...0 || [len(IV)])
				memset(buffer, 0x00, sizeof(buffer));
				PUT_UINT64(iv_sz * 8, buffer, 8);

				while ( iv_sz > 0 ) {
					std::size_t read_sz = (iv_sz < 16) ? iv_sz : 16;

					gcm_hash(H, CB, iv, read_sz);

					iv    += read_sz;
					iv_sz -= read_sz;
				}

				gcm_hash(H, CB, buffer, 16);
			}

			// Keep first counter block for Tag
			sc_ctx.encrypt(CB, base);

			// Prepare counter block for next encryption
			inc_cb(CB);
			sc_ctx.encrypt(CB, stream);

			// Hash additional input
			memset(S, 0x00, sizeof(S));
			while ( add_sz > 0 ) {
				std::size_t read_sz = (add_sz < 16) ? add_sz : 16;

				gcm_hash(H, S, add, read_sz);

				add    += read_sz;
				add_sz -= read_sz;
			}
		}

		~GCM(void)
		{
			zeroize(H,          sizeof(H));
			zeroize(CB,         sizeof(CB));
			zeroize(S,          sizeof(S));
			zeroize(base,       sizeof(base));
			zeroize(buffer,     sizeof(buffer));
			zeroize(stream,     sizeof(stream));

			zeroize(&add_sz,    sizeof(add_sz));
			zeroize(&buffer_sz, sizeof(buffer_sz));
			zeroize(&stream_sz, sizeof(stream_sz));
			zeroize(&total_sz,  sizeof(total_sz));
		}

		int update(const uint8_t *input, std::size_t input_sz, uint8_t *output, std::size_t &output_sz)
		{
			// Check that output is large enough
			if ( output_sz < input_sz ) {
				output_sz = input_sz;
				return CRYPTO_CIPHER_MODE_INVALID_LENGTH;
			}

			// Restrict plaintext size to 2^39-256 bits
			if ( total_sz + input_sz < total_sz || total_sz + input_sz > UL64(0x0000000FFFFFFFE0) ) {
				return CRYPTO_CIPHER_MODE_LENGTH_LIMIT;
			}

			total_sz += input_sz;

			output_sz = 0;
			for ( std::size_t i = 0 ; i < input_sz ; ++i ) {
				output[i]         = input[i] ^ stream[stream_sz];
				buffer[buffer_sz] = is_encrypt ? output[i] : input[i];
				++output_sz;

				++stream_sz;
				if ( 16 == stream_sz ) {
					inc_cb(CB);
					sc_ctx.encrypt(CB, stream);
					stream_sz = 0;
				}

				++buffer_sz;
				if ( 16 == buffer_sz ) {
					gcm_hash(H, S, buffer, 16);
					buffer_sz = 0;
				}
			}

			return CRYPTO_CIPHER_MODE_SUCCESS;
		}

		int finish(std::size_t &pad_sz)
		{
			if ( buffer_sz > 0 ) {
				gcm_hash(H, S, buffer, buffer_sz);
			}

			PUT_UINT64(add_sz   * 8, buffer, 0);
			PUT_UINT64(total_sz * 8, buffer, 8);

			gcm_hash(H, S, buffer, 16);

			for ( std::size_t i = 0 ; i < 16 ; ++i ) {
				S[i] ^= base[i];
			}

			pad_sz = 0;

			return CRYPTO_CIPHER_MODE_SUCCESS;
		}

		int get_tag(uint8_t *tag, std::size_t &tag_sz)
		{
			if ( tag_sz < 4 || tag_sz > 16 ) {
				return CRYPTO_CIPHER_MODE_INVALID_LENGTH;
			}

			memcpy(tag, S, tag_sz);

			return CRYPTO_CIPHER_MODE_SUCCESS;
		}

		int check_tag(uint8_t *tag, std::size_t &tag_sz, bool &auth)
		{
			if ( tag_sz < 4 || tag_sz > 16 ) {
				return CRYPTO_CIPHER_MODE_INVALID_LENGTH;
			}

			// Check tag in constant time
			uint8_t diff = 0;
			for ( std::size_t i = 0 ; i < tag_sz ; ++i ) {
				diff |= (S[i] ^ tag[i]);
			}

			auth = (0 == diff);

			return CRYPTO_CIPHER_MODE_SUCCESS;
		}

		static const std::size_t BLOCK_SIZE = SC::BLOCK_SIZE;
	protected:
		static inline void gcm_hash(const uint8_t H[16], uint8_t S[16], const uint8_t data[16], std::size_t data_sz)
		{
			for ( std::size_t i = 0 ; i < data_sz ; ++i ) {
				S[i] ^= data[i];
			}

			gcm_mult(H, S);
		}

		// TODO: optimization through precompute table
		static inline void gcm_mult(const uint8_t H[16], uint8_t S[16])
		{
			// S = S * H
			uint64_t xl, xh;
			uint64_t vl, vh;
			uint64_t zl, zh;
			uint64_t rl, rh;

			GET_UINT64(xh, S, 0);
			GET_UINT64(xl, S, 8);

			GET_UINT64(vh, H, 0);
			GET_UINT64(vl, H, 8);

			zl = UL64(0x0000000000000000);
			zh = UL64(0x0000000000000000);

			rl = UL64(0x0000000000000000);
			rh = UL64(0xE100000000000000);

			for ( std::size_t i = 0 ; i < 128 ; ++i ) {
				if ( 0 != ((xh >> 63) & 0x01) ) {
					zl = zl ^ vl;
					zh = zh ^ vh;
				}

				if ( 0 != (vl & 0x01) ) {
					vl = ((vh & 0x01) << 63) | (vl >> 1);
					vh = vh >> 1;

					vl ^= rl;
					vh ^= rh;
				} else {
					vl = ((vh & 0x01) << 63) | (vl >> 1);
					vh = vh >> 1;
				}

				xh = (xh << 1) | (xl >> 63);
				xl =  xl << 1;
			}

			PUT_UINT64(zh, S, 0);
			PUT_UINT64(zl, S, 8);
		}

		static void inc_cb(uint8_t CB[16])
		{
			for ( std::size_t i = 15 ; i > 11 ; --i ) {
				if ( ++CB[i] != 0 ) {
					break;
				}
			}
		}

		SC sc_ctx;

		uint8_t H[16];
		uint8_t CB[16];
		uint8_t S[16];
		uint8_t base[16];
		uint8_t buffer[16];
		uint8_t stream[16];
		std::size_t add_sz;
		std::size_t buffer_sz;
		std::size_t stream_sz;
		std::size_t total_sz;
		bool is_encrypt;
};

}

#endif
