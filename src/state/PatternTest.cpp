#include "PatternTest.h"
#include <iostream>
#include <fstream>
#include "common/Misc.h"
#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

namespace cryomesh {
namespace state {

void PatternTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(PatternTest::testPattern));
	s.push_back(CUTE(PatternTest::testPattern_cc));
	s.push_back(CUTE(PatternTest::testPattern_vec));
	s.push_back(CUTE(PatternTest::testPattern_string));
	//s.push_back(CUTE(PatternTest::testDPattern));
	s.push_back(CUTE(PatternTest::testAssignOperator));
	s.push_back(CUTE(PatternTest::testcompare));
	s.push_back(CUTE(PatternTest::testoperatorequals));
	s.push_back(CUTE(PatternTest::testgetPattern));
	s.push_back(CUTE(PatternTest::testsetPattern));
	s.push_back(CUTE(PatternTest::testgetId));
	s.push_back(CUTE(PatternTest::testsetId));
	s.push_back(CUTE(PatternTest::testSerialize));

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "PatternTest");
}

void PatternTest::testPattern() {
	std::cout << "PatternTest::testPattern: " << std::endl;
	int ids = Pattern::getIds();
	Pattern pat;
	int str = pat.getId();

	int pat_sz = pat.getPattern().size();
	ASSERT_EQUAL(ids,str);
	ASSERT_EQUAL(0,pat_sz);
}

void PatternTest::testPattern_cc() {
	std::cout << "PatternTest::testPattern_cc: " << std::endl;
	std::string pat_str = "11001";
	Pattern pat1(pat_str);
	Pattern pat2(pat1);

	ASSERT(pat1==pat2);
}
void PatternTest::testPattern_vec() {
	//std::cout << "PatternTest::testPattern_vec: " << std::endl;
	int ids = Pattern::getIds();
	std::vector<bool> vec_bool;
	vec_bool.push_back(true);
	vec_bool.push_back(false);
	Pattern pat(vec_bool);

	int str = pat.getId();

	int pat_sz = pat.getPattern().size();
	ASSERT_EQUAL(ids,str);
	ASSERT_EQUAL(2,pat_sz);

}
void PatternTest::testPattern_string() {
//	std::cout << "PatternTest::testPattern_string: " << std::endl;
	{
		int ids = Pattern::getIds();
		std::string pat_str = "11001";
		Pattern pat(pat_str);
		int pat_sz = pat.getPattern().size();

		int str = pat.getId();

		ASSERT_EQUAL(ids,str);

		ASSERT_EQUAL(5,pat_sz);
		std::string pat_obj_str = pat.getString();
		ASSERT_EQUAL(pat_str,pat_obj_str);
	}
}
void PatternTest::testDPattern() {
	std::cout << "PatternTest::testDPattern: " << std::endl;
	ASSERT(false);
}
void PatternTest::testAssignOperator() {
	std::cout << "PatternTest::testAssignOperator: " << std::endl;
	std::string str_pat = "11001";
	Pattern pat1(str_pat);
	Pattern pat2 = pat1;

	ASSERT(pat1==pat2);
}
void PatternTest::testcompare() {
	std::cout << "PatternTest::testcompare: " << std::endl;
	std::string str_pat1 = "11001";
	Pattern pat1(str_pat1);

	// test same
	{
		std::string str_pat2 = "11001";
		Pattern pat2(str_pat2);
		double match = pat2.compare(pat1);
		ASSERT_EQUAL_DELTA(1, match, 0.001);
	}
	// test not same
	{
		std::string str_pat2 = "00110";
		Pattern pat2(str_pat2);
		double match = pat2.compare(pat1);
		ASSERT_EQUAL_DELTA(0, match, 0.001);
	}
	// test partial
	{
		std::string str_pat2 = "10100";
		Pattern pat2(str_pat2);
		double match = pat2.compare(pat1);
		ASSERT(match>0 && match <1);
	}

	// test size mismatch
	{
		std::string str_pat2 = "010";
		Pattern pat2(str_pat2);
		double match = pat2.compare(pat1);
		ASSERT_EQUAL_DELTA(0, match, 0.001);
	}

}
void PatternTest::testoperatorequals() {
	std::cout << "PatternTest::testequals: " << std::endl;
	std::string str_pat = "11001";
	Pattern pat1(str_pat);
	Pattern pat2(pat1);

	ASSERT(pat1==pat2);
}
void PatternTest::testgetPattern() {
	std::cout << "PatternTest::testgetPattern: " << std::endl;
	std::vector<bool> vec_bool;
	vec_bool.push_back(true);
	vec_bool.push_back(false);
	Pattern pat(vec_bool);
	std::vector<bool> compbool = pat.getPattern();
	ASSERT(compareBools(vec_bool,compbool));
}
void PatternTest::testsetPattern() {
	std::cout << "PatternTest::testsetPattern: " << std::endl;
	std::vector<bool> vec_bool;
	vec_bool.push_back(true);
	vec_bool.push_back(false);
	Pattern pat;
	pat.setPattern(vec_bool);
	std::vector<bool> compbool = pat.getPattern();
	ASSERT(compareBools(vec_bool,compbool));
}
void PatternTest::testgetId() {
	//std::cout << "PatternTest::testgetId: " << std::endl;
	int ids = Pattern::getIds();
	Pattern pat;

	int str = pat.getId();

	ASSERT_EQUAL(ids,str);
}
void PatternTest::testsetId() {
	std::cout << "PatternTest::testsetId: " << std::endl;
	Pattern pat;
	int id_str = 2;
	pat.setId(id_str);
	int str = pat.getId();

	ASSERT(id_str==str);
}
void PatternTest::testSerialize() {
	const std::string TEST_DIRECTORY="TestData/Tests/";
	std::string ser_file = TEST_DIRECTORY + "pattern_testserialize.txt";
	std::string pat_str = "001000000000011110";
	Pattern pat1(pat_str);
	// save data to archive
	{
		// create and open a character archive for output
		std::ofstream ofs(ser_file.c_str());

		boost::archive::text_oarchive oa(ofs);
		// write class instance to archive
		oa << pat1;
		// archive and stream closed when destructors are called
	}

	// ... some time later restore the class instance to its orginal state
	Pattern pat2;
	{
		// create and open an archive for input
		std::ifstream ifs(ser_file.c_str());
		boost::archive::text_iarchive ia(ifs);
		// read class state from archive
		ia >> pat2;
		// archive and stream closed when destructors are called
	}
	ASSERT(pat1==pat2);

}

bool PatternTest::compareBools(const std::vector<bool> & obj1, const std::vector<bool> & obj2) {
	std::cout << "PatternTest::compareBools: " << "" << std::endl;
	common::Misc::print(std::cout, obj1);
	common::Misc::print(std::cout, obj2);
	bool szbool = obj1.size() == obj2.size();
	if (szbool == false) {
		return false;
	}
	// forall in obj1
	{
		std::vector<bool>::const_iterator it_obj1 = obj1.begin();
		const std::vector<bool>::const_iterator it_obj1_end = obj1.end();
		std::vector<bool>::const_iterator it_obj2 = obj2.begin();
		const std::vector<bool>::const_iterator it_obj2_end = obj2.end();
		while (it_obj1 != it_obj1_end && it_obj2 != it_obj2_end) {
			if (*it_obj1 != *it_obj2) {
				return false;
			}
			++it_obj2;
			++it_obj1;
		}
	}
	return true;
}
}
}
