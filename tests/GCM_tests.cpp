#include <memory>
#include <vector>

#include "TestOptions.hpp"
#include "TestVectors.hpp"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "crypto/AES.hpp"
#include "crypto/DES.hpp"
#include "crypto/GCM.hpp"
#include "crypto/Utils.hpp"

#if defined(_MSC_VER) || defined(__WATCOMC__)
	#define UL64(x) x##ui64
#else
	#define UL64(x) x##ULL
#endif

TEST(GCM, constructor)
{
	// Normal case encrypt
	{
		uint8_t key[16], iv[16], add[16];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);
		std::size_t add_sz = sizeof(add);

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);
		memset(add, 0x00, add_sz);

		Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);
	}

	// Normal case decrypt
	{
		uint8_t key[16], iv[16], add[16];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);
		std::size_t add_sz = sizeof(add);

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);
		memset(add, 0x00, add_sz);

		Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, false);
	}
	
	// Normal case no additional data
	{
		uint8_t key[16], iv[16];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);

		Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, NULL, 0, true);
	}

	// Construction with a block size != 16
	{
		std::string exception, expected("Cipher block size not supported");
		uint8_t key[8], iv[16], add[16];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);
		std::size_t add_sz = sizeof(add);

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);
		memset(add, 0x00, add_sz);

		try {
			Crypto::GCM<Crypto::DES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);
		} catch ( const Crypto::CipherMode::Exception &cme ) {
			exception = cme.what();
		}

		EXPECT_EQ(exception, expected);
	}

	// IV size = 0
	{
		std::string exception, expected("IV length does not meet length requirements");
		uint8_t key[16], iv[16], add[16];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = 0;
		std::size_t add_sz = sizeof(add);

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);
		memset(add, 0x00, add_sz);

		try {
			Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);
		} catch ( const Crypto::CipherMode::Exception &cme ) {
			exception = cme.what();
		}

		EXPECT_EQ(exception, expected);
	}

	// IV size > 2^64
	if ( sizeof(std::size_t) > 8 ) {
		std::string exception, expected("IV length does not meet length requirements");
		uint8_t key[16], iv[16], add[16];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = -1;
		std::size_t add_sz = sizeof(add);

		memset(key, 0x00, key_sz);
		memset(add, 0x00, add_sz);

		try {
			Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);
		} catch ( const Crypto::CipherMode::Exception &cme ) {
			exception = cme.what();
		}

		EXPECT_EQ(exception, expected);
	}

	// Add size > 2^64
	if ( sizeof(std::size_t) > 8 ) {
		std::string exception, expected("Additional Input length does not meet length requirements");
		uint8_t key[16], iv[16], add[16];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);
		std::size_t add_sz = -1;

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);

		try {
			Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);
		} catch ( const Crypto::CipherMode::Exception &cme ) {
			exception = cme.what();
		}

		EXPECT_EQ(exception, expected);
	}
}

TEST(GCM, update_sz)
{
	// Buffer empty, provide < BLOCK_SIZE, space 0
	{
		int ret;
		uint8_t key[16], iv[16], add[16], input[32], output[32];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);
		std::size_t add_sz = sizeof(add);
		std::size_t input_sz = sizeof(input);
		std::size_t output_sz = sizeof(output);

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);
		memset(add, 0x00, add_sz);
		memset(input, 0x00, input_sz);
		memset(output, 0x00, output_sz);

		Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);

		output_sz = 0;
		ret = ctx.update(input, 8, output, output_sz);
		EXPECT_EQ(ret, 1);
		EXPECT_EQ(output_sz, (std::size_t)8);
	}

	// Buffer empty, provide = BLOCK_SIZE, space 0
	{
		int ret;
		uint8_t key[16], iv[16], add[16], input[32], output[32];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);
		std::size_t add_sz = sizeof(add);
		std::size_t input_sz = sizeof(input);
		std::size_t output_sz = sizeof(output);

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);
		memset(add, 0x00, add_sz);
		memset(input, 0x00, input_sz);
		memset(output, 0x00, output_sz);

		Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);

		output_sz = 8;
		ret = ctx.update(input, 8, output, output_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(output_sz, (std::size_t)8);
	}

	// Total input size overflows
	{
		int ret;
		uint8_t key[16], iv[16], add[16], input[32], output[32];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);
		std::size_t add_sz = sizeof(add);
		std::size_t input_sz = sizeof(input);
		std::size_t output_sz = sizeof(output);

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);
		memset(add, 0x00, add_sz);
		memset(input, 0x00, input_sz);
		memset(output, 0x00, output_sz);

		Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);

		input_sz = output_sz = 1;
		ret = ctx.update(input, input_sz, output, output_sz);
		EXPECT_EQ(ret, 0);

		input_sz = output_sz = -1;
		ret = ctx.update(input, input_sz, output, output_sz);
		EXPECT_EQ(ret, 3);
	}

	// Total input size is bigger than what GCM can produce
	{
		int ret;
		uint8_t key[16], iv[16], add[16], input[32], output[32];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);
		std::size_t add_sz = sizeof(add);
		std::size_t input_sz = sizeof(input);
		std::size_t output_sz = sizeof(output);

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);
		memset(add, 0x00, add_sz);
		memset(input, 0x00, input_sz);
		memset(output, 0x00, output_sz);

		Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);

		input_sz = output_sz = 1;
		ret = ctx.update(input, input_sz, output, output_sz);
		EXPECT_EQ(ret, 0);

		input_sz = output_sz = UL64(0x0000000FFFFFFFE0);
		ret = ctx.update(input, input_sz, output, output_sz);
		EXPECT_EQ(ret, 3);
	}
}

TEST(GCM, finish_sz)
{
	int ret;
	uint8_t key[16], iv[16], add[16], input[32], output[32];
	std::size_t key_sz = sizeof(key);
	std::size_t iv_sz = sizeof(iv);
	std::size_t add_sz = sizeof(add);
	std::size_t input_sz = sizeof(input);
	std::size_t output_sz = sizeof(output);

	memset(key, 0x00, key_sz);
	memset(iv, 0x00, iv_sz);
	memset(add, 0x00, add_sz);
	memset(input, 0x00, input_sz);
	memset(output, 0x00, output_sz);

	// Buffer empty, not finished
	{
		Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);

		output_sz = 16;
		ret = ctx.finish(output_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(output_sz, (std::size_t)0);
	}
}

TEST(GCM, get_tag)
{
	// Normal case
	{
		int ret;
		uint8_t key[16], iv[16], add[16], tag[16];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);
		std::size_t add_sz = sizeof(add);
		std::size_t tag_sz = sizeof(tag);
		std::size_t pad_sz = 0;

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);
		memset(add, 0x00, add_sz);
		memset(tag, 0x00, tag_sz);

		Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);

		ret = ctx.finish(pad_sz);
		EXPECT_EQ(ret, 0);

		ret = ctx.get_tag(tag, tag_sz);
		EXPECT_EQ(ret, 0);
	}

	// Requested tag is too small
	{
		int ret;
		uint8_t key[16], iv[16], add[16], tag[3];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);
		std::size_t add_sz = sizeof(add);
		std::size_t tag_sz = sizeof(tag);
		std::size_t pad_sz = 0;

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);
		memset(add, 0x00, add_sz);
		memset(tag, 0x00, tag_sz);

		Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);

		ret = ctx.finish(pad_sz);
		EXPECT_EQ(ret, 0);

		ret = ctx.get_tag(tag, tag_sz);
		EXPECT_EQ(ret, 1);
	}

	// Requested tag is too big
	{
		int ret;
		uint8_t key[16], iv[16], add[16], tag[17];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);
		std::size_t add_sz = sizeof(add);
		std::size_t tag_sz = sizeof(tag);
		std::size_t pad_sz = 0;

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);
		memset(add, 0x00, add_sz);
		memset(tag, 0x00, tag_sz);

		Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);

		ret = ctx.finish(pad_sz);
		EXPECT_EQ(ret, 0);

		ret = ctx.get_tag(tag, tag_sz);
		EXPECT_EQ(ret, 1);
	}
}

TEST(GCM, check_tag)
{
	// Requested tag is too small
	{
		int ret;
		uint8_t key[16], iv[16], add[16], tag[3];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);
		std::size_t add_sz = sizeof(add);
		std::size_t tag_sz = sizeof(tag);
		std::size_t pad_sz = 0;
		bool is_auth;

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);
		memset(add, 0x00, add_sz);
		memset(tag, 0x00, tag_sz);

		Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);

		ret = ctx.finish(pad_sz);
		EXPECT_EQ(ret, 0);

		tag_sz = 3;
		ret = ctx.check_tag(tag, tag_sz, is_auth);
		EXPECT_EQ(ret, 1);
	}

	// Requested tag is too big
	{
		int ret;
		uint8_t key[16], iv[16], add[16], tag[3];
		std::size_t key_sz = sizeof(key);
		std::size_t iv_sz = sizeof(iv);
		std::size_t add_sz = sizeof(add);
		std::size_t tag_sz = sizeof(tag);
		std::size_t pad_sz = 0;
		bool is_auth;

		memset(key, 0x00, key_sz);
		memset(iv, 0x00, iv_sz);
		memset(add, 0x00, add_sz);
		memset(tag, 0x00, tag_sz);

		Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv, iv_sz, add, add_sz, true);

		ret = ctx.finish(pad_sz);
		EXPECT_EQ(ret, 0);

		tag_sz = 17;
		ret = ctx.check_tag(tag, tag_sz, is_auth);
		EXPECT_EQ(ret, 1);
	}
}

TEST(GCM, KAT_enc)
{
	std::vector<std::string> files = {
		"gcmEncryptExtIV128.rsp", "gcmEncryptExtIV192.rsp", "gcmEncryptExtIV256.rsp"
	};

	for ( auto file : files ) {
		std::string file_path = TestOptions::get().vect_dir + "GCM/" + file;

		auto test_vectors = TestVectors::NISTCAVPParser(file_path);
		EXPECT_FALSE(test_vectors.empty());

		for ( auto tests : test_vectors ) {
			for ( auto test : tests ) {
				int res;
				uint8_t key[32];
				std::size_t key_sz = test["Key"].length() / 2;
				std::size_t iv_sz = test["IV"].length() / 2;
				std::size_t add_sz = test["AAD"].length() / 2;
				std::size_t input_sz = test["PT"].length() / 2;
				std::size_t output_sz = test["CT"].length() / 2;
				std::size_t tag_sz = test["Tag"].length() / 2;
				std::unique_ptr<uint8_t[]> iv(new uint8_t[iv_sz]);
				std::unique_ptr<uint8_t[]> add(new uint8_t[add_sz]);
				std::unique_ptr<uint8_t[]> input(new uint8_t[input_sz]);
				std::unique_ptr<uint8_t[]> output(new uint8_t[output_sz]);
				std::unique_ptr<uint8_t[]> tag(new uint8_t[tag_sz]);
				std::size_t total_sz, current_sz, pad_sz = 0;
				std::string output_str, tag_str;

				res = Crypto::Utils::from_hex(test["Key"], key, key_sz);
				EXPECT_EQ(res, 0);

				res = Crypto::Utils::from_hex(test["IV"], iv.get(), iv_sz);
				EXPECT_EQ(res, 0);

				res = Crypto::Utils::from_hex(test["AAD"], add.get(), add_sz);
				EXPECT_EQ(res, 0);

				res = Crypto::Utils::from_hex(test["PT"], input.get(), input_sz);
				EXPECT_EQ(res, 0);

				Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv.get(), iv_sz, add.get(), add_sz, true);

				total_sz = output_sz;
				output_sz = 0;
				for ( std::size_t i = 0 ; i < input_sz ; ++i ) {
					current_sz = total_sz - output_sz;

					res = ctx.update(input.get() + i, 1, output.get() + output_sz, current_sz);
					EXPECT_EQ(res, 0);

					output_sz += current_sz;
					EXPECT_EQ(res, 0);
				}

				res = ctx.finish(pad_sz);
				EXPECT_EQ(res, 0);
				EXPECT_EQ(pad_sz, 0);

				res = ctx.get_tag(tag.get(), tag_sz);
				EXPECT_EQ(res, 0);

				res = Crypto::Utils::to_hex(output.get(), output_sz, output_str, false);
				EXPECT_EQ(res, 0);

				EXPECT_EQ(output_str, test["CT"]);

				if ( test["Tag"].length() > 0 ) {
					res = Crypto::Utils::to_hex(tag.get(), tag_sz, tag_str, false);
					EXPECT_EQ(res, 0);

					EXPECT_EQ(tag_str, test["Tag"]);
				}
			}
		}
	}
}

TEST(GCM, KAT_dec)
{
	std::vector<std::string> files = {
		"gcmDecrypt128.rsp", "gcmDecrypt192.rsp", "gcmDecrypt256.rsp"
	};

	for ( auto file : files ) {
		std::string file_path = TestOptions::get().vect_dir + "GCM/" + file;

		auto test_vectors = TestVectors::NISTCAVPParser(file_path);
		EXPECT_FALSE(test_vectors.empty());

		for ( auto tests : test_vectors ) {
			for ( auto test : tests ) {
				int res;
				uint8_t key[32];
				std::size_t key_sz = test["Key"].length() / 2;
				std::size_t iv_sz = test["IV"].length() / 2;
				std::size_t add_sz = test["AAD"].length() / 2;
				std::size_t input_sz = test["CT"].length() / 2;
				std::size_t output_sz = test["CT"].length() / 2;
				std::size_t tag_sz = test["Tag"].length() / 2;
				std::unique_ptr<uint8_t[]> iv(new uint8_t[iv_sz]);
				std::unique_ptr<uint8_t[]> add(new uint8_t[add_sz]);
				std::unique_ptr<uint8_t[]> input(new uint8_t[input_sz]);
				std::unique_ptr<uint8_t[]> output(new uint8_t[output_sz]);
				std::unique_ptr<uint8_t[]> tag(new uint8_t[tag_sz]);
				std::size_t total_sz, current_sz, pad_sz = 0;
				std::string output_str, tag_str;
				bool is_auth = false;

				res = Crypto::Utils::from_hex(test["Key"], key, key_sz);
				EXPECT_EQ(res, 0);

				res = Crypto::Utils::from_hex(test["IV"], iv.get(), iv_sz);
				EXPECT_EQ(res, 0);

				res = Crypto::Utils::from_hex(test["AAD"], add.get(), add_sz);
				EXPECT_EQ(res, 0);

				res = Crypto::Utils::from_hex(test["CT"], input.get(), input_sz);
				EXPECT_EQ(res, 0);

				res = Crypto::Utils::from_hex(test["Tag"], tag.get(), tag_sz);
				EXPECT_EQ(res, 0);

				Crypto::GCM<Crypto::AES> ctx(key, key_sz, iv.get(), iv_sz, add.get(), add_sz, false);

				total_sz = output_sz;
				output_sz = 0;
				for ( std::size_t i = 0 ; i < input_sz ; ++i ) {
					current_sz = total_sz - output_sz;

					res = ctx.update(input.get() + i, 1, output.get() + output_sz, current_sz);
					EXPECT_EQ(res, 0);

					output_sz += current_sz;
					EXPECT_EQ(res, 0);
				}

				res = ctx.finish(pad_sz);
				EXPECT_EQ(res, 0);
				EXPECT_EQ(pad_sz, 0);

				res = ctx.check_tag(tag.get(), tag_sz, is_auth);
				EXPECT_EQ(res, 0);

				EXPECT_EQ(is_auth, ! test.has("FAIL"));

				if ( ! test.has("FAIL") ) {
					res = Crypto::Utils::to_hex(output.get(), output_sz, output_str, false);
					EXPECT_EQ(res, 0);

					EXPECT_EQ(output_str, test["PT"]);
				}
			}
		}
	}
}
