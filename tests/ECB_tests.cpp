#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "crypto/AES.hpp"
#include "crypto/ECB.hpp"
#include "crypto/Padding.hpp"
#include "crypto/Utils.hpp"

TEST(ECB, encrypt_test_vector)
{
	int res;
	const std::vector<std::vector<std::string>> tests = {
		{
			"edfdb257cb37cdf182c5455b0c0efebb",
			"1695fe475421cace3557daca01f445ff",
			"7888beae6e7a426332a7eaa2f808e637"
		}, {
			"7723d87d773a8bbfe1ae5b081235b566",
			"1b0a69b7bc534c16cecffae02cc5323190ceb413f1db3e9f0f79ba654c54b60e",
			"ad5b089515e7821087c61652dc477ab1f2cc6331a70dfc59c9ffb0c723c682f6"
		}, {
			"280afe063216a10b9cad9b2095552b16",
			"6f172bb6ec364833411841a8f9ea2051735d600538a9ea5e8cd2431a432903c1"
			"d6178988b616ed76e00036c5b28ccd8b",
			"4cc2a8f13c8c7c36ed6a814db7f26900c7e04df49cbad916ce6a44d0ae4fe7ed"
			"c0b402794675b3694933ebbc356525d8"
		}, {
			"ef60fb1400c83936414a25651eb51a1b",
			"593559318cc66bf695e49feb42794bdfb66bce895ec222ca2609b133ecf66ac7"
			"344d13021e01e11a969c4684cbe20abae2b19d3ceb2cacd41419f21f1c865149",
			"3ea6f4305217bd47eebe773da4b578549cac744c00cbd8f9d596d38010304bd8"
			"50cc2f4b19a91c2e022eabf100266185ca2705127815dfd46efbe4ecd46a3058"
		}, {
			"c5805cd1c4a7b98a715badb709720bf4",
			"84f809fc5c84652376cc0df10095bc00b9f0547fa91a2d3310a0adbc9cc6191a"
			"de2aaa6fffa5e406af7223955f9277bfb06eb1dd2bbfbefe32ab342c36302bf2"
			"2bc64e1b394032bbb5f4e6744f1bcbf2",
			"a6dc096bc21b0658e416a0f679fefc6e958e9c56e3ce04fdf6e392c2db770a60"
			"d9523c255925e14a3e02a1002bf3875c2e501bac618bee1f55f9850454854eef"
			"9d693d90937cc8387b6f4c4414e2080b"
		}, {
			"4c35be028e1475278346eae531cbee5c",
			"7adcf4a494f6b09790c82c8bb97db62c5d3fa4032f06dfeceaad9ecb374b747b"
			"d1c08d07e78e351dc2eb99bfa714d23cffe31f5fb5a472e6e0252f35a20c304c"
			"4f6d0cf7d29c99444d40af3a00a92fc86c6444fcb80ce9765362ac1bdba0b10e",
			"22217953f71932ab4360d97ef495081559f1fcb09caca41fa0c65f7b1792b560"
			"eabe18f3b3b06ef80c41886f24c5d6d32d20427e83d8b5564d9ac7435a2842c1"
			"cf7c6fcc229eb7f518d3e0167d510efbaee39a0438fc800eb6acfc203c93280c"
		}, {
			"00cc73c990d376b82246e45ea3ae2e37",
			"37a1205ea929355d2e4ee52d5e1d9cda279ae01e640287ccb153276e7e0ecf2d"
			"633cf4f2b3afaecb548a2590ce0445c6a168bac3dc601813eb74591bb1ce8dfc"
			"d740cdbb6388719e8cd283d9cc7e736938240b410dd5a6a48ba49dd2066503e6"
			"3ab592ffdf3be49e7d2de74f82158b8c",
			"c88e03383ba9da6f982c057fe92c0bb3ed5b9cd18295a100e13a4e12d440b919"
			"bbb8b221abead362902ce44d30d0b80e56bee1f66a7d8de0b1e1b4dbf76c90c1"
			"807a3bc5f277e9814c82ab120f7e10217dfdf6092ce4958f8906c5e32279c653"
			"7dd1fbae20cb7a1d9f89d0490b6aefc1"
		}, {
			"0a53aa7a3e4a4f364e8c6c7224af5501",
			"eaf1760c0f25310dada6debeb966304db7a9f1b2d1c3af922623b263649031d2"
			"99b3c56146d61d55b6ebf4cf8dd04039a4d1ace3146f49ee915f806afad64cbb"
			"2d04a64120de40382e2175dcae9480d1ca8dedc38fb64e4a40112f10f03a4c35"
			"4fed01f2c5c7017dbd514b2d443a5adfd2e49c986723266cda41a69e6e459908",
			"5ece70a44da41bc7cfb9b582ea9ce0980030ec4af331e76499961f88860aa055"
			"4aba3ecbf77ca4293a3fee854a2caf3ae800343fb4521388b16b6dc599b3d60b"
			"f82777f98e1a8d04ab9cd54dd9a248095795d4dfe4858bfd9a05f54c795bb086"
			"e15f7c22228184ec66a9ca10b1cf71a6bb9303c5cd1dcc056460a86df651f053"
		}, {
			"b80bcc929052cb5450479442e2b809ce",
			"8177d79c8f239178186b4dc5f1df2ea7fee7d0db535489ef983aefb3b2029aeb"
			"a0bb2b46a2b18c94a1417a33cbeb41ca7ea9c73a677fccd2eb5470c3c500f6d3"
			"f1a6c755c944ba586f88921f6ae6c9d194e78c7233c406126633e144c3810ad2"
			"3ee1b5af4c04a22d49e99e7017f74c2309492569ff49be17d2804920f2ac5f51"
			"4d13fd3e7318cc7cf80ca5101a465428",
			"5befb3062a7a7246af1f77b0ec0ac614e28be06ac2c81b19e5a0481bf160f9f2"
			"bc43f28f6548787639e4ce3e0f1e95475f0e81ceb793004c8e46670ebd48b866"
			"d5b43d104874ead4be8a236bf90b48f862f7e252dec4475fdbb841a662efcd25"
			"ed64b2910e9baaea9466e413a4241438b31df0bd3df9a16f4641636754e25986"
			"1728aa7ddf435cc51f54f79a1db25f52"
		}, {
			"ebea9c6a82213a00ac1d22faea22116f",
			"451f45663b44fd005f3c288ae57b383883f02d9ad3dc1715f9e3d6948564257b"
			"9b06d7dd51935fee580a96bbdfefb918b4e6b1daac809847465578cb8b5356ed"
			"38556f801ff7c11ecba9cdd263039c15d05900fc228e1caf302d261d7fb56cee"
			"663595b96f192a78ff4455393a5fe8162170a066fdaeac35019469f22b347068"
			"6bced2f007a1a2e43e01b4562caaa502ed541b8205874ec1ffb1c8b255766942",
			"01043053f832ef9b911ed387ba577451e30d51d4b6b11f319d4cd539d067b7f4"
			"f9b4f41f7f3d4e920c57cbe2b5e1885aa66203ae493e93a1df63793a9563c176"
			"bc6775dd09cc9161e278a01beb8fd8a19200326bd95abc5f716768e34f90b505"
			"23d30fdabb103a3bc020afbbb0cb3bd2ad512a6fea79f8d64cef347458dec48b"
			"e89451cb0b807d73593f273d9fc521b789a77524404f43e00f20b3b77b938b1a"
		}
	};

	for ( auto test : tests ) {
		uint8_t key[16];
		std::size_t key_sz = sizeof(key);

		uint8_t plain[Crypto::AES::BLOCK_SIZE * 10];
		std::size_t plain_sz = sizeof(plain);

		uint8_t cipher[Crypto::AES::BLOCK_SIZE * 10];
		std::size_t cipher_sz = sizeof(cipher);
		std::string ciphertext;

		uint8_t pad[16];
		std::size_t pad_sz = sizeof(pad);

		Crypto::Utils::from_hex(test[0], key,   key_sz);
		Crypto::Utils::from_hex(test[1], plain, plain_sz);

		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		std::size_t current_sz, offset;
		offset = 0;
		for ( std::size_t i = 0 ; i < plain_sz ; ++i ) {
			current_sz = cipher_sz - offset;

			res = ctx.update(plain + i, 1, cipher + offset, current_sz);
			EXPECT_EQ(res, 0);

			offset += current_sz;
		}

		res = ctx.finish(pad_sz);
		EXPECT_EQ(res, 0);

		Crypto::Utils::to_hex(cipher, offset, ciphertext, false);
		EXPECT_THAT(ciphertext, test[2]);
	}
}

TEST(ECB, decrypt_test_vector)
{
	int res;
	const std::vector<std::vector<std::string>> tests = {
		{
			"54b760dd2968f079ac1d5dd20626445d",
			"065bd5a9540d22d5d7b0f75d66cb8b30",
			"46f2c98932349c338e9d67f744a1c988"
		}, {
			"bce4c486cc4a12622b9147df9bca452c",
			"76fa67e121894dbc2ad85ae7f9c5c44d14ccee5b87d5e7708950338f5c84cf58",
			"8c8e6ed8a6bdb3baabec78189b7d61010d6d168de2c08ec56f9b9c004a191e21"
		}, {
			"279161759d29e92dc8f1fafd8d45533d",
			"1ce69ab7624fed511fb42b8fb639f566d72783f9ab49801651961b048e0f5b7c"
			"c0ebb2f514f2d625f927ccc4d5a94593",
			"8838058bd98072b72fd75641edb216301fee8323ff8c4ba18ee6a4623c2fdeb3"
			"df5f8a240a8e7e6d5ae15444614f41fb"
		}, {
			"9ba852520c9fd1eb367b6ad2aed07abd",
			"6c53489f6a4bd0f7cd15d20f6ebc7e649fd95b76b107e6daba967c8a9484797f"
			"29a8a282ee31c03fae4f8e9b8930d5473c2ed695a347e88b7ccd62376d5ebb41",
			"6061df5bcd421fabdb5235fc032502650448fd8233a0237c5f6f249a63d7db3e"
			"4283ac9a8684a363ef64e760c5886564659da619466851568b32091df5516f57"
		}, {
			"cd6a33abb95d7ab92980f9c2b810c90f",
			"33cb1a98ce4be97f5b222aa861c945643f372687cbf7f8193a7d304e40baceb8"
			"77eedfbc05be47437a86bdb1f7f0171b95c6c8b50404a7897332f26fb0f09239"
			"91d3a02e926f29babfb52e63d580efff",
			"3f098ae7e7524ddb556403c5d3d0b10e5db6c324843ca54a47e34ad9bdbccc93"
			"a645e96f2e98651751f0f1a697398ea801a69079c1808b6a8e3bb02a4cf3292f"
			"1a7a05b954571339f7899f9aaa2ae1e9"
		}, {
			"ff71b9f6311a44072ddb1a77488c4870",
			"19d76b386a7715dec7aad8bb01d126c7385a4387f087faec67718f78274cf4a2"
			"d7ec3f7c56885c930bbfff1676d8dd3b0b4e1cdf418c086de6f50700b527cfcc"
			"2a81d6ba95e30a526eb4c3dee82af6ccc8c5ab55781db15c57de90995411bff6",
			"3dfe918048f4c0b0b22fbc55e5af751cf3e6ce348eb9a888795033f6dd216afb"
			"ea65fc8df41c0ce6c7316a8183d36060bf19709fe2bec6d13bbdf1a434cc5b8f"
			"10aadb368c269d2dc4474a1f8112640fe52803b1883cb7756743100a758dd862"
		}, {
			"8ec6a5a054fea2fc8dafb5939a4bd788",
			"4d6f972946a2e4ca05bfbeecd00535ece6c81ed963c4167063b1c34b6a185d55"
			"04646d81a83cd5bce2101120b2f2bc6a2fa856ae6fff4449ab6218189faa13e7"
			"3038e82c5141c3f6276a8a206b9563ca11fa76092779f4deff2e58659047ed4f"
			"1a129092095ab75f1cf4b255c69531f4",
			"6e19cec775595c2f768de3bda3682fb4026ed1889fc3de11bc45a2a3e2b22694"
			"ddaab3274c81b29204fa034eedac564a7dfe27aabb6ff420f9aa5abadf15c746"
			"76efeabd96a7ff2491813ba53cb5ab76eacd37255a28daecbbe580f5eb77f9bc"
			"0339e0da59b2b307f52974b6182cb106"
		}, {
			"b53e54b1b09f78abcd6afac1f7e7cda9",
			"447557bd1561b09a082cabdbf49b0612b46c9793db3c987aa4a843ef28b39fa8"
			"d46799b019c0566765c5c8cb53f87ff51ff0c2e65c45df84e9acbfe02163bdba"
			"52d0b847706567320bc6ede07f2eaef86b0f602364f86510eabc58bc9ad1e6f0"
			"a6f6df0b83188c01e17744a4e0053a22810e99cf5a1ed3258f203509fdc3c050",
			"398db04b45a724393e4baec86a220f22358df1bbf0cab278dcfecf7a3f8f6275"
			"ef45ed9f039bc1f5030c3a0c245fbb6ea0bff3e2da0859ac498db6624a572023"
			"de7c221bf3567f51e98947970dfc581b3ca670ab92953d3bdd97dbf5d6bef8aa"
			"2f31500d221198ed9f23b049eaeed67683083034b4a5a82a0e49319e368c6f55"
		}, {
			"9e79b681c57d7feb7db7e8a6ad41382c",
			"fa2e97bed2f0b76ed2f8c1275f6d50ecfc1131240622ba9da17e5dd0f20a7d67"
			"8f537ffc33762524b73e17ac3d8dc322affd679768c66ca430e3f6ab6e70b95f"
			"99570e87f02b46ff6b543efff16d339fcbead89b59fea65403bc7418737c47a3"
			"01806be66fd7c307d7825da8e1d6496c3921acb7419010b60350fea069f4cf3b"
			"b017bfa11a424344e3109d30778fad90",
			"a2e19c08fa204640b1bc73beac58f81a7923f3ef2a0affcf1568c9179d816cf1"
			"bd16b79418f0266f0ff904e06045f276df187ed72b8f6641b065b854a7d3f152"
			"b8e153cd77ec605e5b75454245745dc0e0dcfc616f5246f868b6ee5a1df56c49"
			"06c5c34bc42309d370733e50d9a1addc3f37cbe4b331ac7d3a920c794484a16b"
			"05599f46f42046a6dc9063b1d5a8ae1f"
		}, {
			"44f0ee626d0446e0a3924cfb078944bb",
			"931b2f5f3a5820d53a6beaaa6431083a3488f4eb03b0f5b57ef838e157962310"
			"3bd6e6800377538b2e51ef708f3c4956432e8a8ee6a34e190642b26ad8bdae6c"
			"2af9a6c7996f3b6004d2671e41f1c9f40ee03d1c4a52b0a0654a331f15f34dce"
			"4acb96bd6507815ca4347a3de11a311b7de5351c9787c4538158e28974ffa83d"
			"8296dfe9cd09cd87f7bf4f54d97d28d4788799163408323943b3e72f5eab66c1",
			"9c29eecb2de04254fafb896a994102d1da30ddb49d82728eb23dbd029901e9b7"
			"5b3d0aee03f7a05f6c852d8fada0b5c28e8c9aed334fad11829df3dfadc5c2e4"
			"71eb41af9e48a8a465e03d5ebdb0216915081f3b5a0ebb2308dfc2d28e5a8ba3"
			"f32adae4c3575921bc657b63d46ba5a618880ee9ad8af3fba5643a5026facd7d"
			"667ce599327f936cdda7e1bb742a33a019990b76be648a6ec725daed540ed9e7"
		}
	};

	for ( auto test : tests ) {
		uint8_t key[16];
		std::size_t key_sz = sizeof(key);

		uint8_t cipher[Crypto::AES::BLOCK_SIZE * 10];
		std::size_t cipher_sz = sizeof(cipher);

		uint8_t plain[Crypto::AES::BLOCK_SIZE * 10];
		std::string plaintext;

		std::size_t pad_sz;

		Crypto::Utils::from_hex(test[0], key,    key_sz);
		Crypto::Utils::from_hex(test[1], cipher, cipher_sz);

		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		std::size_t current_sz, offset;
		offset = 0;
		for ( std::size_t i = 0 ; i < cipher_sz ; ++i ) {
			current_sz = cipher_sz - offset;

			res = ctx.update(cipher + i, 1, plain + offset, current_sz);
			EXPECT_EQ(res, 0);

			offset += current_sz;
		}

		res = ctx.finish(pad_sz);
		EXPECT_EQ(res, 0);

		Crypto::Utils::to_hex(plain, offset, plaintext, false);
		EXPECT_THAT(plaintext, test[2]);
	}
}

TEST(ECB, encrypt_update)
{
	int res;
	const std::vector<std::vector<std::string>> tests = {
		{
			"00000000000000000000000000000000",
			"0000000000000000000000000000000000000000000000000000000000000000"
			"00000000000000000000000000000000000000000000000000000000000000",
			"66e94bd4ef8a2c3b884cfa59ca342b2e66e94bd4ef8a2c3b884cfa59ca342b2e"
			"66e94bd4ef8a2c3b884cfa59ca342b2e58e2fccefa7e3061367f1d57a4e7455a"
	       	}
	};

	for ( auto test : tests ) {
		uint8_t key[16];
		std::size_t key_sz = sizeof(key);

		uint8_t plain[Crypto::AES::BLOCK_SIZE * 4];
		std::size_t plain_sz = sizeof(plain);

		uint8_t cipher[Crypto::AES::BLOCK_SIZE * 4];
		std::size_t cipher_sz = sizeof(cipher);
		std::string ciphertext;

		Crypto::Utils::from_hex(test[0], key,   key_sz);
		Crypto::Utils::from_hex(test[1], plain, plain_sz);

		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		std::size_t current_sz, offset;
		offset = 0;

		// Buffer is 0
		current_sz = cipher_sz - offset;
		res = ctx.update(plain, 1, cipher + offset, current_sz);
		EXPECT_EQ(res, 0);
		EXPECT_EQ(current_sz, (std::size_t)0);
		offset += current_sz;

		// Buffer is 1
		current_sz = cipher_sz - offset;
		res = ctx.update(plain + 1, 2, cipher + offset, current_sz);
		EXPECT_EQ(res, 0);
		EXPECT_EQ(current_sz, (std::size_t)0);
		offset += current_sz;

		// Buffer is 3
		current_sz = cipher_sz - offset;
		res = ctx.update(plain + 3, 4, cipher + offset, current_sz);
		EXPECT_EQ(res, 0);
		EXPECT_EQ(current_sz, (std::size_t)0);
		offset += current_sz;

		// Buffer is 7
		current_sz = cipher_sz - offset;
		res = ctx.update(plain + 7, 8, cipher + offset, current_sz);
		EXPECT_EQ(res, 0);
		EXPECT_EQ(current_sz, (std::size_t)0);
		offset += current_sz;

		// Buffer is 15
		current_sz = cipher_sz - offset;
		res = ctx.update(plain + 15, 16, cipher + offset, current_sz);
		EXPECT_EQ(res, 0);
		EXPECT_EQ(current_sz, (std::size_t)16);
		offset += current_sz;

		// Buffer is 15
		current_sz = cipher_sz - offset;
		res = ctx.update(plain + 31, 32, cipher + offset, current_sz);
		EXPECT_EQ(res, 0);
		EXPECT_EQ(current_sz, (std::size_t)32);
		offset += current_sz;

		// Buffer is 15
		current_sz = cipher_sz - offset;
		uint8_t pad[1] = { 0x01 };
		res = ctx.update(pad, 1, cipher + offset, current_sz);
		EXPECT_EQ(res, 0);
		offset += current_sz;
		res = ctx.finish(current_sz);
		EXPECT_EQ(res, 0);

		Crypto::Utils::to_hex(cipher, offset, ciphertext, false);

		EXPECT_THAT(ciphertext, test[2]);
	}
}

TEST(ECB, decrypt_update)
{
	int res;
	const std::vector<std::vector<std::string>> tests = {
		{
			"00000000000000000000000000000000",
			"66e94bd4ef8a2c3b884cfa59ca342b2e66e94bd4ef8a2c3b884cfa59ca342b2e"
			"66e94bd4ef8a2c3b884cfa59ca342b2e58e2fccefa7e3061367f1d57a4e7455a",
			"0000000000000000000000000000000000000000000000000000000000000000"
			"00000000000000000000000000000000000000000000000000000000000000"
	       	}
	};

	for ( auto test : tests ) {
		uint8_t key[16];
		std::size_t key_sz = sizeof(key);

		uint8_t cipher[Crypto::AES::BLOCK_SIZE * 4];
		std::size_t cipher_sz = sizeof(cipher);

		uint8_t plain[Crypto::AES::BLOCK_SIZE * 4];
		std::size_t plain_sz = sizeof(plain);
		std::string plaintext;

		Crypto::Utils::from_hex(test[0], key,    key_sz);
		Crypto::Utils::from_hex(test[1], cipher, cipher_sz);

		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		std::size_t current_sz, offset;
		offset = 0;

		// Buffer is 0
		current_sz = plain_sz - offset;
		res = ctx.update(cipher, 1, plain + offset, current_sz);
		EXPECT_EQ(res, 0);
		EXPECT_EQ(current_sz, (std::size_t)0);
		offset += current_sz;

		// Buffer is 1
		current_sz = plain_sz - offset;
		res = ctx.update(cipher + 1, 2, plain + offset, current_sz);
		EXPECT_EQ(res, 0);
		EXPECT_EQ(current_sz, (std::size_t)0);
		offset += current_sz;

		// Buffer is 3
		current_sz = plain_sz - offset;
		res = ctx.update(cipher + 3, 4, plain + offset, current_sz);
		EXPECT_EQ(res, 0);
		EXPECT_EQ(current_sz, (std::size_t)0);
		offset += current_sz;

		// Buffer is 7
		current_sz = plain_sz - offset;
		res = ctx.update(cipher + 7, 8, plain + offset, current_sz);
		EXPECT_EQ(res, 0);
		EXPECT_EQ(current_sz, (std::size_t)0);
		offset += current_sz;
		
		// Buffer is 15
		current_sz = plain_sz - offset;
		res = ctx.update(cipher + 15, 16, plain + offset, current_sz);
		EXPECT_EQ(res, 0);
		EXPECT_EQ(current_sz, (std::size_t)16);
		offset += current_sz;

		// Buffer is 15
		current_sz = plain_sz - offset;
		res = ctx.update(cipher + 31, 32, plain + offset, current_sz);
		EXPECT_EQ(res, 0);
		EXPECT_EQ(current_sz, (std::size_t)32);
		offset += current_sz;

		// Buffer is 15
		current_sz = plain_sz - offset;
		res = ctx.update(cipher + 63, 1, plain + offset, current_sz);
		EXPECT_EQ(res, 0);
		EXPECT_EQ(current_sz, (std::size_t)16);
		offset += current_sz;

		// Buffer is 0
		res = ctx.finish(current_sz);
		EXPECT_EQ(res, 0);

		Crypto::ANSIX923Padding::unpad(plain, plain_sz, current_sz);
		Crypto::Utils::to_hex(plain, current_sz, plaintext, false);

		EXPECT_THAT(plaintext, test[2]);
	}
}

TEST(ECB, encrypt_update_size)
{
	int ret;

	uint8_t key[16];
	std::size_t key_sz = sizeof(key);
	memset(key, 0x00, key_sz);

	uint8_t plain[32];
	std::size_t plain_sz = sizeof(plain);
	memset(plain, 0x00, plain_sz);

	uint8_t cipher[32];
	std::size_t cipher_sz = sizeof(cipher);
	memset(cipher, 0x00, cipher_sz);

	// Buffer empty, provide < BLOCK_SIZE, space 0
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = 0;
		ret = ctx.update(plain, 8, cipher, cipher_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(cipher_sz, (std::size_t)0);
	}

	// Buffer empty, provide = BLOCK_SIZE, space 0
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = 0;
		ret = ctx.update(plain, 16, cipher, cipher_sz);
		EXPECT_EQ(ret, 1);
		EXPECT_EQ(cipher_sz, (std::size_t)16);
	}

	// Buffer empty, provide = BLOCK_SIZE, space BLOCK_SIZE
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = 16;
		ret = ctx.update(plain, 16, cipher, cipher_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(cipher_sz, (std::size_t)16);
	}

	// Buffer empty, provide = 1.5 * BLOCK_SIZE, space 0
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = 0;
		ret = ctx.update(plain, 24, cipher, cipher_sz);
		EXPECT_EQ(ret, 1);
		EXPECT_EQ(cipher_sz, (std::size_t)16);
	}

	// Buffer empty, provide = 1.5 * BLOCK_SIZE, space BLOCK_SIZE
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = 16;
		ret = ctx.update(plain, 24, cipher, cipher_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(cipher_sz, (std::size_t)16);
	}

	// Buffer empty, provide = 2 * BLOCK_SIZE, space 0
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = 0;
		ret = ctx.update(plain, 32, cipher, cipher_sz);
		EXPECT_EQ(ret, 1);
		EXPECT_EQ(cipher_sz, (std::size_t)32);
	}

	// Buffer empty, provide = 2 * BLOCK_SIZE, space 2 * BLOCK_SIZE
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = 32;
		ret = ctx.update(plain, 32, cipher, cipher_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(cipher_sz, (std::size_t)32);
	}

	// Buffer = 0.5 * BLOCK_SIZE, provide = 0.25 * BLOCK_SIZE, space = 0
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = 0;
		ret = ctx.update(plain, 8, cipher, cipher_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(cipher_sz, (std::size_t)0);

		cipher_sz = 0;
		ret = ctx.update(plain, 4, cipher, cipher_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(cipher_sz, (std::size_t)0);
	}

	// Buffer = 0.5 * BLOCK_SIZE, provide = 0.5 * BLOCK_SIZE, space = 0
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = 0;
		ret = ctx.update(plain, 8, cipher, cipher_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(cipher_sz, (std::size_t)0);

		cipher_sz = 0;
		ret = ctx.update(plain, 8, cipher, cipher_sz);
		EXPECT_EQ(ret, 1);
		EXPECT_EQ(cipher_sz, (std::size_t)16);
	}

	// Buffer = 0.5 * BLOCK_SIZE, provide = 0.5 * BLOCK_SIZE, space = BLOCK_SIZE
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = 0;
		ret = ctx.update(plain, 8, cipher, cipher_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(cipher_sz, (std::size_t)0);

		cipher_sz = 16;
		ret = ctx.update(plain, 8, cipher, cipher_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(cipher_sz, (std::size_t)16);
	}

	// Buffer = 0.5 * BLOCK_SIZE, provide = 1.5 * BLOCK_SIZE, space = 2 * BLOCK_SIZE
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = 0;
		ret = ctx.update(plain, 8, cipher, cipher_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(cipher_sz, (std::size_t)0);

		cipher_sz = 32;
		ret = ctx.update(plain, 24, cipher, cipher_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(cipher_sz, (std::size_t)32);
	}
}

TEST(ECB, encrypt_finish_size)
{
	int ret;

	uint8_t key[16];
	std::size_t key_sz = sizeof(key);
	memset(key, 0x00, key_sz);

	uint8_t plain[32];
	std::size_t plain_sz = sizeof(plain);
	memset(plain, 0x00, plain_sz);

	uint8_t cipher[32];
	std::size_t cipher_sz = sizeof(cipher);
	memset(cipher, 0x00, cipher_sz);

	// Buffer empty, not finished
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = 0;
		ret = ctx.finish(cipher_sz);
		EXPECT_EQ(ret, 0);
	}

	// Buffer not empty, not finished
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = sizeof(cipher);
		ret = ctx.update(plain, 8, cipher, cipher_sz);
		EXPECT_EQ(ret, 0);

		cipher_sz = 0;
		ret = ctx.finish(cipher_sz);
		EXPECT_EQ(ret, 2);
		EXPECT_EQ(cipher_sz, (std::size_t)8);
	}

	// Buffer empty, finished
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, true);

		cipher_sz = 0;
		ret = ctx.finish(cipher_sz);
		EXPECT_EQ(ret, 0);

		cipher_sz = 0;
		ret = ctx.finish(cipher_sz);
		EXPECT_EQ(ret, 0);
	}
}

TEST(ECB, decrypt_update_size)
{
	int ret;

	uint8_t key[16];
	std::size_t key_sz = sizeof(key);
	memset(key, 0x00, key_sz);

	uint8_t cipher[32];
	std::size_t cipher_sz = sizeof(cipher);
	memset(cipher, 0x00, cipher_sz);

	uint8_t plain[32];
	std::size_t plain_sz = sizeof(plain);
	memset(plain, 0x00, plain_sz);

	// Buffer empty, provide < BLOCK_SIZE, space 0
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = 0;
		ret = ctx.update(cipher, 8, plain, plain_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(plain_sz, (std::size_t)0);
	}

	// Buffer empty, provide = BLOCK_SIZE, space 0
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = 0;
		ret = ctx.update(cipher, 16, plain, plain_sz);
		EXPECT_EQ(ret, 1);
		EXPECT_EQ(plain_sz, (std::size_t)16);
	}

	// Buffer empty, provide = BLOCK_SIZE, space BLOCK_SIZE
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = 16;
		ret = ctx.update(cipher, 16, plain, plain_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(plain_sz, (std::size_t)16);
	}

	// Buffer empty, provide = 1.5 * BLOCK_SIZE, space 0
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = 0;
		ret = ctx.update(cipher, 24, plain, plain_sz);
		EXPECT_EQ(ret, 1);
		EXPECT_EQ(plain_sz, (std::size_t)16);
	}

	// Buffer empty, provide = 1.5 * BLOCK_SIZE, space BLOCK_SIZE
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = 16;
		ret = ctx.update(cipher, 24, plain, plain_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(plain_sz, (std::size_t)16);
	}

	// Buffer empty, provide = 2 * BLOCK_SIZE, space 0
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = 0;
		ret = ctx.update(cipher, 32, plain, plain_sz);
		EXPECT_EQ(ret, 1);
		EXPECT_EQ(plain_sz, (std::size_t)32);
	}

	// Buffer empty, provide = 2 * BLOCK_SIZE, space 2 * BLOCK_SIZE
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = 32;
		ret = ctx.update(cipher, 32, plain, plain_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(plain_sz, (std::size_t)32);
	}

	// Buffer = 0.5 * BLOCK_SIZE, provide = 0.25 * BLOCK_SIZE, space = 0
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = 0;
		ret = ctx.update(cipher, 8, plain, plain_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(plain_sz, (std::size_t)0);

		plain_sz = 0;
		ret = ctx.update(cipher, 4, plain, plain_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(plain_sz, (std::size_t)0);
	}

	// Buffer = 0.5 * BLOCK_SIZE, provide = 0.5 * BLOCK_SIZE, space = 0
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = 0;
		ret = ctx.update(cipher, 8, plain, plain_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(plain_sz, (std::size_t)0);

		plain_sz = 0;
		ret = ctx.update(cipher, 8, plain, plain_sz);
		EXPECT_EQ(ret, 1);
		EXPECT_EQ(plain_sz, (std::size_t)16);
	}

	// Buffer = 0.5 * BLOCK_SIZE, provide = 0.5 * BLOCK_SIZE, space = BLOCK_SIZE
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = 0;
		ret = ctx.update(cipher, 8, plain, plain_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(plain_sz, (std::size_t)0);

		plain_sz = 16;
		ret = ctx.update(cipher, 8, plain, plain_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(plain_sz, (std::size_t)16);
	}

	// Buffer = 0.5 * BLOCK_SIZE, provide = 1.5 * BLOCK_SIZE, space = BLOCK_SIZE
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = 0;
		ret = ctx.update(cipher, 8, plain, plain_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(plain_sz, (std::size_t)0);

		plain_sz = 16;
		ret = ctx.update(cipher, 24, plain, plain_sz);
		EXPECT_EQ(ret, 1);
		EXPECT_EQ(plain_sz, (std::size_t)32);
	}

	// Buffer = 0.5 * BLOCK_SIZE, provide = 1.5 * BLOCK_SIZE, space = 2 * BLOCK_SIZE
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = 0;
		ret = ctx.update(cipher, 8, plain, plain_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(plain_sz, (std::size_t)0);

		plain_sz = 32;
		ret = ctx.update(cipher, 24, plain, plain_sz);
		EXPECT_EQ(ret, 0);
		EXPECT_EQ(plain_sz, (std::size_t)32);
	}
}

TEST(ECB, decrypt_finish_size)
{
	int ret;

	uint8_t key[16];
	std::size_t key_sz = sizeof(key);
	memset(key, 0x00, key_sz);

	uint8_t cipher[32];
	std::size_t cipher_sz = sizeof(cipher);
	memset(cipher, 0x00, cipher_sz);

	uint8_t plain[32];
	std::size_t plain_sz = sizeof(plain);
	memset(plain, 0x00, plain_sz);

	// Buffer empty, not finished
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		cipher_sz = 0;
		ret = ctx.finish(cipher_sz);
		EXPECT_EQ(ret, 0);
	}

	// Buffer not empty, not finished
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = sizeof(cipher);
		ret = ctx.update(cipher , 8, plain, plain_sz);
		EXPECT_EQ(ret, 0);

		plain_sz = 0;
		ret = ctx.finish(plain_sz);
		EXPECT_EQ(ret, 2);
		EXPECT_EQ(plain_sz, (std::size_t)8);
	}

	// Buffer empty, finished
	{
		Crypto::ECB<Crypto::AES> ctx(key, key_sz, false);

		plain_sz = 0;
		ret = ctx.finish(plain_sz);
		EXPECT_EQ(ret, 0);

		plain_sz = 0;
		ret = ctx.finish(plain_sz);
		EXPECT_EQ(ret, 0);
	}
}
