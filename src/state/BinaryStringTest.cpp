/*
 * BinaryStringTest.cpp
 *
 *  Created on: 26 Nov 2010
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#include "BinaryStringTest.h"
#include "state/BinaryString.h"
#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"
#include "state/Pattern.h"
#include <cstdlib>

namespace cryomesh {
namespace state {
void BinaryStringTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(BinaryStringTest::testresize));
	//s.push_back(CUTE(BinaryStringTest::testformatBinaryStringToInt));
	//s.push_back(CUTE(BinaryStringTest::testformatTextToInt));
	s.push_back(CUTE(BinaryStringTest::BinaryStringString));
	s.push_back(CUTE(BinaryStringTest::BinaryStringPattern));
	s.push_back(CUTE(BinaryStringTest::testformatTextToBinaryString));
	s.push_back(CUTE(BinaryStringTest::testisValidBinary));
	s.push_back(CUTE(BinaryStringTest::testgetBools));

//	s.push_back(CUTE(BinaryStringTest::testtoInt));
//	s.push_back(CUTE(BinaryStringTest::BinaryStringInt));

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "BinaryStringTest");
}
//DEPRECATED
/*
void BinaryStringTest::testtoInt() {
	//1
	{
		int inint = 1;
		std::string bin = "1";
		BinaryString bs(bin, false, BinaryString::BIN);
		int outint = bs.toInt();
		ASSERT_EQUAL(inint, outint);
	}
	//3
	{
		int inint = 3;
		std::string bin = "011";
		BinaryString bs(bin, false, BinaryString::BIN);
		int outint = bs.toInt();
		ASSERT_EQUAL(inint, outint);
	}

	///8
	{
		int inint = 8;
		std::string bin = "000000001000";
		BinaryString bs(bin, false, BinaryString::BIN);
		int outint = bs.toInt();
		ASSERT_EQUAL(inint, outint);
	}

	//67
	{
		int inint = 67;
		std::string bin = "00000001000011";
		BinaryString bs(bin, false, BinaryString::BIN);
		int outint = bs.toInt();
		ASSERT_EQUAL(inint, outint);
	}
}
*/
void BinaryStringTest::BinaryStringString() {
	std::string teststr_exp = "TeSt1";
	// TeSt1 = 54h 65h 53h 74h 31h
	// TeSt1 = 01010100 01100101 01010011 01110100 00110001
	std::string bin_exp = "0101010001100101010100110111010000110001";

	// from string
	{
		BinaryString bs(teststr_exp);
		std::string bin_out = bs.getBinaryString();
		std::string teststr_out = bs.toText();

		ASSERT_EQUAL(bin_exp, bin_out);
		ASSERT_EQUAL(teststr_exp, teststr_out);
	}

}

void BinaryStringTest::BinaryStringPattern() {
	std::string teststr_exp = "TeSt1";
	// TeSt1 = 54h 65h 53h 74h 31h
	// TeSt1 = 01010100 01100101 01010011 01110100 00110001
	std::string bin_exp = "0101010001100101010100110111010000110001";
	Pattern pat(bin_exp);
	// from string
	{
		BinaryString bs(pat.getString(), false, BinaryString::BIN);
		std::string bin_out = bs.getBinaryString();
		std::string teststr_out = bs.toText();

		ASSERT_EQUAL(bin_exp, bin_out);
		ASSERT_EQUAL(teststr_exp, teststr_out);
	}
}
void BinaryStringTest::testisValidBinary() {
	std::string good_bin = "1111001001001001";
	std::string bad_bin = "111100003g";
	BinaryString bs_good(good_bin, false, BinaryString::BIN);
	BinaryString bs_bad(bad_bin, false, BinaryString::BIN);
	ASSERT(bs_good.isValidBinary() ==true);
	ASSERT(bs_bad.isValidBinary() ==false);

}
// DEPRECATED
/*
void BinaryStringTest::BinaryStringInt() {
	{
		std::string bin1 = "10010010010010001001010000100010010";
		long long  int1 = 19632;
		BinaryString bs1(bin1, false, BinaryString::BIN);
		int int1_out = bs1.toInt();
		ASSERT_EQUAL(int1, int1_out);
	}
	std::string teststr_exp = "TeSt1";
	BinaryString bstest(teststr_exp);
	int testint_exp = bstest.toInt();

	// TeSt1 = 54h 65h 53h 74h 31h
	// TeSt1 = 01010100 01100101 01010011 01110100 00110001

	std::string bin_exp = "0101010001100101010100110111010000110001";

	// from string
	{
		BinaryString bs(testint_exp);
		std::cout << "BinaryStringTest::BinaryStringInt: " << "bstest: " << bstest.toText() << " bs:" << bs.toText()
				<< std::endl;
		std::string bin_out = bs.getBinaryString();
		std::string teststr_out = bs.toText();

		ASSERT_EQUAL(bin_exp, bin_out);
		ASSERT_EQUAL(teststr_exp, teststr_out);
	}
}
*/
void BinaryStringTest::testresize() {
	std::string str1 = "10";
	//std::cout << "BinaryStringTest::testresize: 1" << "" << std::endl;
	BinaryString bs(str1, false, BinaryString::BIN);
	//std::cout << "BinaryStringTest::testresize: 2" << "" << std::endl;
	ASSERT_EQUAL(2, bs.getBinaryString().size());
	bs.resize(8);
	ASSERT_EQUAL(8, bs.getBinaryString().size());
	ASSERT_EQUAL(0, bs.getBinaryString().compare("00000010"));

}
//DEPRECATED
/*
void BinaryStringTest::testformatBinaryStringToInt() {
	int int1 = 2;
	std::string str1 = "10";
	int int2 = 11;
	std::string str2 = "1011";
	int int3 = 145;
	std::string str3 = "10010001";

	//test1
	{
		BinaryString bs;
		bs.setBinaryString(str1);
		std::cout << "BinaryStringTest::testformatBinaryStringToInt: DEBUG1: " << bs.getBinaryString() << std::endl;
		int out1 = bs.toInt();
		std::cout << "BinaryStringTest::testformatBinaryStringToInt: DEBUG1 " << "" << std::endl;

		ASSERT_EQUAL(int1, out1);
	}
	//test2
	{
		BinaryString bs;
		bs.setBinaryString(str2);
		int out2 = bs.toInt();
		ASSERT_EQUAL(int2, out2);
	}
	//test1
	{
		BinaryString bs;
		bs.setBinaryString(str3);
		int out3 = bs.toInt();
		ASSERT_EQUAL(int3, out3);
	}

	//testa
	{
		BinaryString bs(int1);
		std::string outstr = bs.toText();
		std::cout << "MiscTest::testformatBinaryStringToInt: " << "outstr: " << outstr << std::endl;
		ASSERT(str1.compare(outstr));
	}
	//testb
	{
		BinaryString bs(int2);
		std::string outstr = bs.toText();
		std::cout << "MiscTest::testformatBinaryStringToInt: " << "outstr: " << outstr << std::endl;
		ASSERT(str2.compare(outstr));
	}
	//testc
	{
		BinaryString bs(int3);
		std::string outstr = bs.toText();
		std::cout << "MiscTest::testformatBinaryStringToInt: " << "outstr: " << outstr << std::endl;
		ASSERT(str3.compare(outstr));
	}
}
*/
void BinaryStringTest::testformatTextToInt() {
	std::string str1 = "f*cK YoU";
	std::list<int> intcodes;

	std::list<int> gotcodes = BinaryString::formatTextToInts(str1);

	/*
	 std::cout << "MiscTest::formatTextToInt: ";
	 // forall in gotcodes
	 {
	 std::list<int>::const_iterator it_gotcodes = gotcodes.begin();
	 const std::list<int>::const_iterator it_gotcodes_end = gotcodes.end();
	 while (it_gotcodes != it_gotcodes_end) {
	 std::cout << *it_gotcodes << " ";
	 ++it_gotcodes;
	 }
	 }
	 */
	std::cout << std::endl;
	std::string gotstr = BinaryString::formatIntsToText(gotcodes);
	//std::cout << "MiscTest::formatTextToInt: compare " <<"\""<<str1<<"\""<< " to " <<"\""<<gotstr <<"\""<< " err: "<<str1.compare(gotstr) << std::endl;
	ASSERT(str1.compare(gotstr) == 0);

}

void BinaryStringTest::testformatTextToBinaryString() {
	{
		std::string teststr_exp = "TeSt1";
		// TeSt1 = 54h 65h 53h 74h 31h
		// TeSt1 = 01010100 01100101 01010011 01110100 00110001
		std::string bin_exp = "0101010001100101010100110111010000110001";

		BinaryString bs(teststr_exp);
		std::string text_out = bs.toText();
		std::string bin_out = bs.getBinaryString();
		ASSERT_EQUAL(bin_exp, bin_out);
		ASSERT_EQUAL(teststr_exp, text_out);
	}

	{
		std::string text_exp = "f";
		std::string bin_exp = "01100110";
		BinaryString bs(text_exp);
		std::string bin_out = bs.getBinaryString();
		std::string text_out = bs.toText();
		ASSERT_EQUAL(BinaryString::BINARY_CHAR_LENGTH, bin_out.size());
	//	std::cout << "MiscTest::testformatTextToBinaryString: " << "bin_out " << "\"" << bin_out << "\"" << std::endl;
	//	std::cout << "MiscTest::testformatTextToBinaryString: " << "text_out " << "\"" << text_out << "\"" << std::endl;

		ASSERT_EQUAL(0, bin_exp.compare(bin_out));
		ASSERT_EQUAL(0, text_exp.compare(text_out));

	}
	{
		std::string text_exp = "fD";
		// f = 66 = 0110 0110
		// D = 44 = 0100 0100
		std::string bin_exp = "0110011001000100";
		BinaryString bs(text_exp);
		std::string bin_out = bs.getBinaryString();
		std::string text_out = bs.toText();
	//	std::cout << "BinaryStringTest::testformatTextToBinaryString: " << "bin_exp:" << bin_exp << std::endl;
	//	std::cout << "BinaryStringTest::testformatTextToBinaryString: " << "bin_out:" << bin_out << std::endl;
		ASSERT_EQUAL(0, bin_exp.compare(bin_out));
		ASSERT_EQUAL(0, text_exp.compare(text_out));

		std::string allbins;
		std::list<BinaryString> bins_out = BinaryString::formatTextToBinaryStrings(text_exp, allbins);
		// forall in bins_out
		{
			std::list<BinaryString>::const_iterator it_bins_out = bins_out.begin();
			const std::list<BinaryString>::const_iterator it_bins_out_end = bins_out.end();
			while (it_bins_out != it_bins_out_end) {
				std::string nowstr = it_bins_out->getBinaryString();
				std::string nowtxt = it_bins_out->toText();
			//	std::cout << "BinaryStringTest::testformatTextToBinaryString: " << "str: " << nowstr << " txt: "
			//			<< nowtxt << std::endl;
				++it_bins_out;
			}
		}
		{
			std::list<BinaryString>::const_iterator it_bins_out = bins_out.begin();
			const std::list<BinaryString>::const_iterator it_bins_out_end = bins_out.end();
			std::string nowstr = it_bins_out->getBinaryString();
			std::string nowtxt = it_bins_out->toText();
			//std::cout << "BinaryStringTest::testformatTextToBinaryString: " << "str: " << nowstr << " txt: " << nowtxt
			//		<< std::endl;
			ASSERT_EQUAL(8,nowstr.size());
			ASSERT_EQUAL("01100110", nowstr);

			++it_bins_out;
			ASSERT_EQUAL("01000100", it_bins_out->getBinaryString());
			++it_bins_out;
			ASSERT_EQUAL(it_bins_out_end, it_bins_out);
		}
	}

	{
		std::string text_exp = "f*";
		std::string bin_exp = "0110011000101010";
		BinaryString bs(text_exp);
		std::string bin_out = bs.getBinaryString();
		std::string text_out = bs.toText();
		ASSERT_EQUAL(0, bin_exp.compare(bin_out));
		ASSERT_EQUAL(0, text_exp.compare(text_out));
	}
}

void BinaryStringTest::testgetBools(){
	BinaryString bstr("101", false, BinaryString::BIN);
	std::vector<bool> bvec;
	bvec.push_back(true);
	bvec.push_back(false);
	bvec.push_back(true);

	std::vector<bool> bvec_new = bstr.getBools();

	ASSERT_EQUAL(bvec, bvec_new);

}
}
}
