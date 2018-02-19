#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "crypto/Utils.hpp"
#include "crypto/SHA256.hpp"

TEST(SHA256, digest_test_vector)
{
	const std::vector<std::vector<std::string>> test = {
		{
			"",
			"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"
		}, {
			"bd",
			"68325720aabd7c82f30f554b313d0570c95accbb7dc4b5aae11204c08ffe732b"
		}, {
			"5fd4",
			"7c4fbf484498d21b487b9d61de8914b2eadaf2698712936d47c3ada2558f6788"
		}, {
			"b0bd69",
			"4096804221093ddccfbf46831490ea63e9e99414858f8d75ff7f642c7ca61803"
		}, {
			"c98c8e55",
			"7abc22c0ae5af26ce93dbb94433a0e0b2e119d014f8e7f65bd56c61ccccd9504"
		}, {
			"81a723d966",
			"7516fb8bb11350df2bf386bc3c33bd0f52cb4c67c6e4745e0488e62c2aea2605"
		}, {
			"8390cf0be07661cc7669aac54ce09a37733a629d45f5d983ef201f9b2d13800e"
			"555d9b1097fec3b783d7a50dcb5e2b644b96a1e9463f177cf34906bf388f366d"
			"b5c2deee04a30e283f764a97c3b377a034fefc22c259214faa99babaff160ab0"
			"aaa7e2ccb0ce09c6b32fe08cbc474694375aba703fadbfa31cf685b30a11c57f"
			"3cf4edd321e57d3ae6ebb1133c8260e75b9224fa47a2bb205249add2e2e62f81"
			"7491482ae152322be0900355cdcc8d42a98f82e961a0dc6f537b7b410eff105f"
			"59673bfb787bf042aa071f7af68d944d27371c64160fe9382772372516c230c1"
			"f45c0d6b6cca7f274b394da9402d3eafdf733994ec58ab22d71829a98399574d"
			"4b5908a447a5a681cb0dd50a31145311d92c22a16de1ead66a5499f2dceb4cae"
			"694772ce90762ef8336afec653aa9b1a1c4820b221136dfce80dce2ba920d88a"
			"530c9410d0a4e0358a3a11052e58dd73b0b179ef8f56fe3b5a2d117a73a0c38a"
			"1392b6938e9782e0d86456ee4884e3c39d4d75813f13633bc79baa07c0d2d555"
			"afbf207f52b7dca126d015aa2b9873b3eb065e90b9b065a5373fe1fb1b20d594"
			"327d19fba56cb81e7b6696605ffa56eba3c27a438697cc21b201fd7e09f18dee"
			"a1b3ea2f0d1edc02df0e20396a145412cd6b13c32d2e605641c948b714aec30c"
			"0649dc44143511f35ab0fd5dd64c34d06fe86f3836dfe9edeb7f08cfc3bd4095"
			"6826356242191f99f53473f32b0cc0cf9321d6c92a112e8db90b86ee9e87cc32"
			"d0343db01e32ce9eb782cb24efbbbeb440fe929e8f2bf8dfb1550a3a2e742e8b"
			"455a3e5730e9e6a7a9824d17acc0f72a7f67eae0f0970f8bde46dcdefaed3047"
			"cf807e7f00a42e5fd11d40f5e98533d7574425b7d2bc3b3845c443008b58980e"
			"768e464e17cc6f6b3939eee52f713963d07d8c4abf02448ef0b889c9671e2f8a"
			"436ddeeffcca7176e9bf9d1005ecd377f2fa67c23ed1f137e60bf46018a8bd61"
			"3d038e883704fc26e798969df35ec7bbc6a4fe46d8910bd82fa3cded265d0a3b"
			"6d399e4251e4d8233daa21b5812fded6536198ff13aa5a1cd46a5b9a17a4ddc1"
			"d9f85544d1d1cc16f3df858038c8e071a11a7e157a85a6a8dc47e88d75e7009a"
			"8b26fdb73f33a2a70f1e0c259f8f9533b9b8f9af9288b7274f21baeec78d396f"
			"8bacdcc22471207d9b4efccd3fedc5c5a2214ff5e51c553f35e21ae696fe51e8"
			"df733a8e06f50f419e599e9f9e4b37ce643fc810faaa47989771509d69a110ac"
			"916261427026369a21263ac4460fb4f708f8ae28599856db7cb6a43ac8e03d64"
			"a9609807e76c5f312b9d1863bfa304e8953647648b4f4ab0ed995e",
			"4109cdbec3240ad74cc6c37f39300f70fede16e21efc77f7865998714aad0b5e"
		}
	};

	for ( std::size_t i = 0 ; i < test.size() ; ++i ) {
		uint8_t in[2048];
		std::size_t in_sz = sizeof(in);
		uint8_t out[Crypto::SHA256::SIZE];
		std::string output;

		Crypto::Utils::from_hex(test[i][0], in, in_sz);
		Crypto::MessageDigest_get<Crypto::SHA256>(in, in_sz, out);
		Crypto::Utils::to_hex(out, sizeof(out), output, false);

		EXPECT_THAT(output, test[i][1]);
	}
}

TEST(SHA256, reset_ctx)
{
	const std::vector<std::vector<std::string>> test = {
		{
			"",
			"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"
		}, {
			"bd",
			"68325720aabd7c82f30f554b313d0570c95accbb7dc4b5aae11204c08ffe732b"
		}, {
			"5fd4",
			"7c4fbf484498d21b487b9d61de8914b2eadaf2698712936d47c3ada2558f6788"
		}, {
			"b0bd69",
			"4096804221093ddccfbf46831490ea63e9e99414858f8d75ff7f642c7ca61803"
		}, {
			"c98c8e55",
			"7abc22c0ae5af26ce93dbb94433a0e0b2e119d014f8e7f65bd56c61ccccd9504"
		}, {
			"81a723d966",
			"7516fb8bb11350df2bf386bc3c33bd0f52cb4c67c6e4745e0488e62c2aea2605"
		}, {
			"8390cf0be07661cc7669aac54ce09a37733a629d45f5d983ef201f9b2d13800e"
			"555d9b1097fec3b783d7a50dcb5e2b644b96a1e9463f177cf34906bf388f366d"
			"b5c2deee04a30e283f764a97c3b377a034fefc22c259214faa99babaff160ab0"
			"aaa7e2ccb0ce09c6b32fe08cbc474694375aba703fadbfa31cf685b30a11c57f"
			"3cf4edd321e57d3ae6ebb1133c8260e75b9224fa47a2bb205249add2e2e62f81"
			"7491482ae152322be0900355cdcc8d42a98f82e961a0dc6f537b7b410eff105f"
			"59673bfb787bf042aa071f7af68d944d27371c64160fe9382772372516c230c1"
			"f45c0d6b6cca7f274b394da9402d3eafdf733994ec58ab22d71829a98399574d"
			"4b5908a447a5a681cb0dd50a31145311d92c22a16de1ead66a5499f2dceb4cae"
			"694772ce90762ef8336afec653aa9b1a1c4820b221136dfce80dce2ba920d88a"
			"530c9410d0a4e0358a3a11052e58dd73b0b179ef8f56fe3b5a2d117a73a0c38a"
			"1392b6938e9782e0d86456ee4884e3c39d4d75813f13633bc79baa07c0d2d555"
			"afbf207f52b7dca126d015aa2b9873b3eb065e90b9b065a5373fe1fb1b20d594"
			"327d19fba56cb81e7b6696605ffa56eba3c27a438697cc21b201fd7e09f18dee"
			"a1b3ea2f0d1edc02df0e20396a145412cd6b13c32d2e605641c948b714aec30c"
			"0649dc44143511f35ab0fd5dd64c34d06fe86f3836dfe9edeb7f08cfc3bd4095"
			"6826356242191f99f53473f32b0cc0cf9321d6c92a112e8db90b86ee9e87cc32"
			"d0343db01e32ce9eb782cb24efbbbeb440fe929e8f2bf8dfb1550a3a2e742e8b"
			"455a3e5730e9e6a7a9824d17acc0f72a7f67eae0f0970f8bde46dcdefaed3047"
			"cf807e7f00a42e5fd11d40f5e98533d7574425b7d2bc3b3845c443008b58980e"
			"768e464e17cc6f6b3939eee52f713963d07d8c4abf02448ef0b889c9671e2f8a"
			"436ddeeffcca7176e9bf9d1005ecd377f2fa67c23ed1f137e60bf46018a8bd61"
			"3d038e883704fc26e798969df35ec7bbc6a4fe46d8910bd82fa3cded265d0a3b"
			"6d399e4251e4d8233daa21b5812fded6536198ff13aa5a1cd46a5b9a17a4ddc1"
			"d9f85544d1d1cc16f3df858038c8e071a11a7e157a85a6a8dc47e88d75e7009a"
			"8b26fdb73f33a2a70f1e0c259f8f9533b9b8f9af9288b7274f21baeec78d396f"
			"8bacdcc22471207d9b4efccd3fedc5c5a2214ff5e51c553f35e21ae696fe51e8"
			"df733a8e06f50f419e599e9f9e4b37ce643fc810faaa47989771509d69a110ac"
			"916261427026369a21263ac4460fb4f708f8ae28599856db7cb6a43ac8e03d64"
			"a9609807e76c5f312b9d1863bfa304e8953647648b4f4ab0ed995e",
			"4109cdbec3240ad74cc6c37f39300f70fede16e21efc77f7865998714aad0b5e"
		}
	};

	for ( std::size_t i = 0 ; i < test.size() ; ++i ) {
		uint8_t in[2048];
		std::size_t in_sz = sizeof(in);

		uint8_t out_1[Crypto::SHA256::SIZE];
		uint8_t out_2[Crypto::SHA256::SIZE];
		std::string output_1, output_2;

		Crypto::Utils::from_hex(test[i][0], in, in_sz);

		Crypto::SHA256 ctx;
		ctx.update(in, in_sz);
		ctx.reset();
		ctx.update(in, in_sz);
		ctx.finish(out_1);
		Crypto::Utils::to_hex(out_1, sizeof(out_1), output_1, false);

		ctx.update(in, in_sz);
		ctx.finish(out_2);
		Crypto::Utils::to_hex(out_2, sizeof(out_2), output_2, false);

		EXPECT_THAT(output_1, test[i][1]);
		EXPECT_THAT(output_2, test[i][1]);
	}
}
