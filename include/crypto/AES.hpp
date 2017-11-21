#ifndef CRYPTO_AES_H
#define CRYPTO_AES_H

namespace Crypto
{

class AES
{
	public:
		AES(const uint8_t*, std::size_t);
		~AES(void);

		void encrypt(const uint8_t*, uint8_t*);
		void decrypt(const uint8_t*, uint8_t*);

		static const std::size_t BLOCK_SIZE = 16;
	protected:
		std::size_t nr;

		uint32_t *rk_enc;
		uint32_t buf_enc[68];

		uint32_t *rk_dec;
		uint32_t buf_dec[68];

		void set_keyenc(const uint8_t*, std::size_t);
		void set_keydec(const uint8_t*, std::size_t);

		static const uint8_t  FSb[256];
		static const uint32_t FT0[256], FT1[256], FT2[256], FT3[256];
		static const uint8_t  RSb[256];
		static const uint32_t RT0[256], RT1[256], RT2[256], RT3[256];

		static const uint32_t RCON[10];
};

class AESException : public std::runtime_error
{
	public:
		AESException(const char *what_arg) : std::runtime_error(what_arg) {}
};

}

#endif
