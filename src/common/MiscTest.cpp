#include "MiscTest.h"

namespace cryomesh {
namespace common {
void MiscTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(MiscTest::testparseArgs));
	s.push_back(CUTE(MiscTest::testtoLower));
	s.push_back(CUTE(MiscTest::testremoveChars));

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "MiscTest");
}
void MiscTest::testparseArgs() {
	std::list<std::string> args = Misc::parseArgs("1 2 A");
	int sz = args.size();
	std::string first = args.front();
	std::string last = args.back();
	ASSERT_EQUAL(3, sz);
	ASSERT_EQUAL("1", first);
	ASSERT_EQUAL("A", last);
}

void MiscTest::testtoLower() {
	std::string instr = "I Am in Upper and lower Case 3 4 5";
	std::string expstr = "i am in upper and lower case 3 4 5";
	std::string outstr = Misc::toLower(instr);
	ASSERT_EQUAL(expstr, outstr);

	// test list
	{
		std::string instr2 = "abCD";
		std::string expstr2 = "abcd";
		std::string outstr2 = "";
		std::list<std::string> templs;
		templs.push_back(instr);
		templs.push_back(instr2);
		std::list<std::string> newlist = Misc::toLower(templs);

		std::list<std::string>::const_iterator it_newls = newlist.begin();
		const std::list<std::string>::const_iterator it_newls_end = newlist.end();

		ASSERT_EQUAL(2, newlist.size());

		ASSERT_EQUAL(expstr, *it_newls);
		++it_newls;
		ASSERT_EQUAL(expstr2, *it_newls);


	}
}

void MiscTest::testremoveChars(){
	std::string messy_str = " I am messy  #";
	std::string exp_str = "Iammessy";
	std::string clean_str = Misc::removeChars(" #", messy_str);
	ASSERT_EQUAL(exp_str, clean_str);
}
}//NAMESPACE
}
