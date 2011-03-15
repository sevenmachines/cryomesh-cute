#include "SequenceTest.h"

#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

namespace cryomesh {
namespace state {

void SequenceTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(SequenceTest::testserialize));
	s.push_back(CUTE(SequenceTest::testSequence));
	//s.push_back(CUTE(SequenceTest::testDSequence));
	s.push_back(CUTE(SequenceTest::testSequence_sequence));
	s.push_back(CUTE(SequenceTest::testSequence_vectors));
	s.push_back(CUTE(SequenceTest::testAssignOperator));
	s.push_back(CUTE(SequenceTest::testgetCurrentPattern));
	s.push_back(CUTE(SequenceTest::testgetNextPattern));
	s.push_back(CUTE(SequenceTest::testgetSetPatterns));
	s.push_back(CUTE(SequenceTest::testsaveLoadToFile));
	s.push_back(CUTE(SequenceTest::testcompare_pattern));
	s.push_back(CUTE(SequenceTest::testcompare_sequence));
	s.push_back(CUTE(SequenceTest::testoperatorequals));
	//s.push_back(CUTE(SequenceTest::testLeftShiftOperator));
	s.push_back(CUTE(SequenceTest::testinitialise));
	s.push_back(CUTE(SequenceTest::testisAllZeroes));
	s.push_back(CUTE(SequenceTest::testisInputAllZeroes));
	s.push_back(CUTE(SequenceTest::testisOutputAllZeroes));

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "SequenceTest");
}

void SequenceTest::testserialize() {
	//std::cout << "SequenceTest::testserialize: " << std::endl;

	Sequence obj1 = getTestSequence();
	const std::string TEST_DIRECTORY="TestData/Tests/";
	std::string ser_file = TEST_DIRECTORY + "sequence_testserialize.txt";
	// save data to archive
	{
		// create and open a character archive for output
		std::ofstream ofs(ser_file.c_str());

		boost::archive::text_oarchive oa(ofs);
		// write class instance to archive
		oa << obj1;
		// archive and stream closed when destructors are called
	}

	// ... some time later restore the class instance to its orginal state
	Sequence obj2;
	{
		// create and open an archive for input
		std::ifstream ifs(ser_file.c_str());
		boost::archive::text_iarchive ia(ifs);
		// read class state from archive
		ia >> obj2;
		// archive and stream closed when destructors are called
	}
	ASSERT(obj1 == (obj2));
}
void SequenceTest::testSequence() {
	//std::cout << "SequenceTest::testSequence: " << std::endl;
	//test blank defaults
	{
		Sequence seq;
		int map_sz = seq.getPatterns().size();
		std::list<std::pair<Pattern, Pattern> >::const_iterator it_begin = seq.getPatterns().begin();
		// we start off with a default of a null pattern//
		// not any more we dont
		ASSERT_EQUAL(0, map_sz);
		// pattern is null so size 0

		ASSERT(it_begin == seq.getCurrentIterator());
	}
}
void SequenceTest::testDSequence() {
	//std::cout << "SequenceTest::testDSequence: " << std::endl;
	ASSERT(false);
}
void SequenceTest::testSequence_sequence() {
	//std::cout << "SequenceTest::testSequence_sequence: " << std::endl;

	Sequence obj1 = getTestSequence();
	Sequence obj2(obj1);
	ASSERT(obj1 == (obj2));
}
void SequenceTest::testSequence_vectors() {
	std::vector<Pattern> test_pats_in;
	std::vector<Pattern> test_pats_out;
	generateTestPatterns(test_pats_in, test_pats_out);
	Sequence seq = getTestSequence();
	int map_sz = seq.getPatterns().size();
	int inpat_sz = seq.getCurrentInputPattern().getPattern().size();
	int outpat_sz = seq.getCurrentOutputPattern().getPattern().size();
	std::list<std::pair<Pattern, Pattern> >::const_iterator it_begin = seq.getPatterns().begin();
	// we start off with a default of a null pattern
	ASSERT_EQUAL(2, map_sz);
	ASSERT_EQUAL(5, inpat_sz);
	ASSERT_EQUAL(5, outpat_sz);
	ASSERT(it_begin == seq.getCurrentIterator());

}

void SequenceTest::testAssignOperator() {
	//std::cout << "SequenceTest::testAssignOperator: " << std::endl;

	Sequence obj1 = getTestSequence();
	Sequence obj2 = obj1;
	ASSERT(obj1 == (obj2));
}
void SequenceTest::testgetCurrentPattern() {
	//std::cout << "SequenceTest::testgetCurrentPattern: " << std::endl;
	std::vector<Pattern> test_pats_in;
	std::vector<Pattern> test_pats_out;
	generateTestPatterns(test_pats_in, test_pats_out);
	Sequence obj1 = getTestSequence();
	std::vector<Pattern>::const_iterator it_test_pats_in = test_pats_in.begin();
	const std::vector<Pattern>::const_iterator it_test_pats__in_end = test_pats_in.end();
	std::vector<Pattern>::const_iterator it_test_pats_out = test_pats_out.begin();
	const std::vector<Pattern>::const_iterator it_test_pats_out_end = test_pats_out.end();
	while (it_test_pats_in != it_test_pats__in_end && it_test_pats_out != it_test_pats_out_end) {
		ASSERT_EQUAL_DELTA(1, it_test_pats_in->compare(
						obj1.getCurrentInputPattern()), 0.001);
		ASSERT_EQUAL_DELTA(1, it_test_pats_out->compare(
						obj1.getCurrentOutputPattern()), 0.001);

		obj1.getNextInputPattern();
		++it_test_pats_in;
		++it_test_pats_out;
	}
	// test for circularity
	ASSERT_EQUAL_DELTA(1, test_pats_in.front().compare(
					obj1.getCurrentInputPattern()), 0.0001);
	ASSERT_EQUAL_DELTA(1, test_pats_out.front().compare(
					obj1.getCurrentOutputPattern()), 0.0001);
}
void SequenceTest::testgetNextPattern() {
	//std::cout << "SequenceTest::testgetCurrentPattern: " << std::endl;
	std::vector<Pattern> test_pats_in;
	std::vector<Pattern> test_pats_out;
	generateTestPatterns(test_pats_in, test_pats_out);
	Sequence obj1 = getTestSequence();
	std::vector<Pattern>::const_iterator it_test_pats_in = test_pats_in.begin();
	const std::vector<Pattern>::const_iterator it_test_pats__in_end = test_pats_in.end();
	std::vector<Pattern>::const_iterator it_test_pats_out = test_pats_out.begin();
	const std::vector<Pattern>::const_iterator it_test_pats_out_end = test_pats_out.end();
	++it_test_pats_in;
	++it_test_pats_out;
	obj1.getNextInputPattern();
	while (it_test_pats_in != it_test_pats__in_end && it_test_pats_out != it_test_pats_out_end) {
		//	std::cout<<"testgetCurrentPattern: " <<std::endl;
		//				std::cout<<*it_test_pats_in<<std::endl;
		//			std::cout<<obj1.getNextInputPattern()<<std::endl;

		ASSERT_EQUAL_DELTA(1, it_test_pats_in->compare(
						obj1.getCurrentInputPattern()), 0.001);
		ASSERT_EQUAL_DELTA(1, it_test_pats_out->compare(
						obj1.getCurrentOutputPattern()), 0.001);

		++it_test_pats_in;
		++it_test_pats_out;
	}
	obj1.getNextInputPattern();
	// test for circularity
	ASSERT_EQUAL_DELTA(1, test_pats_in.front().compare(
					obj1.getCurrentInputPattern()), 0.0001);
	ASSERT_EQUAL_DELTA(1, test_pats_out.front().compare(
					obj1.getCurrentOutputPattern()), 0.0001);

}
void SequenceTest::testgetSetPatterns() {
	std::vector<Pattern> test_pats_in;
	std::vector<Pattern> test_pats_out;
	generateTestPatterns(test_pats_in, test_pats_out);
	Sequence obj1;
	std::list<std::pair<Pattern, Pattern> > test_pats_map;
	std::vector<Pattern>::const_iterator it_test_pats_in = test_pats_in.begin();
	const std::vector<Pattern>::const_iterator it_test_pats__in_end = test_pats_in.end();
	std::vector<Pattern>::const_iterator it_test_pats_out = test_pats_out.begin();
	const std::vector<Pattern>::const_iterator it_test_pats_out_end = test_pats_out.end();
	while (it_test_pats_in != it_test_pats__in_end && it_test_pats_out != it_test_pats_out_end) {
		test_pats_map.push_back(std::pair<Pattern, Pattern>(*it_test_pats_in, *it_test_pats_out));
		++it_test_pats_in;
		++it_test_pats_out;
	}

	// test seq set/get
	{
		Sequence seq;
		seq.setPatterns(test_pats_map);
		ASSERT(std::equal(test_pats_map.begin(), test_pats_map.end(),
						seq.getPatterns().begin()));

		int count = 0;
		std::list<std::pair<Pattern, Pattern> >::const_iterator it_test_pats_map = test_pats_map.begin();
		const std::list<std::pair<Pattern, Pattern> >::const_iterator it_test_pats_map_end = test_pats_map.end();
		while (it_test_pats_map != it_test_pats_map_end) {
			if (seq.getCurrentInputPattern() == it_test_pats_map->first && seq.getCurrentOutputPattern()
					== it_test_pats_map->second) {
				++count;
			}
			++it_test_pats_map;
		}
		ASSERT_EQUAL(1, count);
	}
}
void SequenceTest::testsaveLoadToFile() {
	//std::cout << "SequenceTest::testsaveToFile: " << std::endl;
	const std::string TEST_DIRECTORY = "TestData/Tests/";
	std::string filename = TEST_DIRECTORY + "testsaveLoadToFile.txt";
	std::ofstream os(filename.c_str());

	Sequence obj1 = getTestSequence();
	obj1.saveToFile(os);

	Sequence obj2;
	std::ifstream is(filename.c_str());
	obj2.loadFromFile(is);
	ASSERT_EQUAL_DELTA(1, obj1.compare(obj2), 0.0001);

}

void SequenceTest::testcompare_pattern() {
	//std::cout << "SequenceTest::testcompare_pattern: " << std::endl;
	Sequence seq1 = getTestSequence();
	// id exists
	{
		std::string pat_str("10101");
		Pattern pat1(pat_str);
		int temp_id = seq1.getCurrentInputPatternId();
		ASSERT(seq1.compare(temp_id, pat1) > 0);
	}

	//id doesnt exist
	{
		std::string pat_str("10101");
		Pattern pat1(pat_str);
		int temp_id = 34728902;
		ASSERT_EQUAL_DELTA(0, seq1.compare(temp_id, pat1), 0.0001);
	}
	//pattern is wrong width
	{
		std::string pat_str("101");
		Pattern pat1(pat_str);
		int temp_id = 34728902;
		ASSERT_EQUAL_DELTA(0, seq1.compare(temp_id, pat1), 0.0001);
	}
}
void SequenceTest::testcompare_sequence() {
	//std::cout << "SequenceTest::testcompare_sequence: " << std::endl;

	Sequence seq1 = getTestSequence();
	//full match
	{
		Sequence seq2(seq1);
		ASSERT_EQUAL_DELTA(1, seq1.compare(seq2), 0.0001);
	}

	//partial match
	{
		std::vector<Pattern> pats_in;
		std::vector<Pattern> pats_out;
		std::string pat_str1 = "11000";
		std::string pat_str2 = "10101";
		std::string pat_str3 = "01110";
		std::string pat_str4 = "00011";
		Pattern pat1(pat_str1);
		Pattern pat2(pat_str2);
		Pattern pat3(pat_str3);
		Pattern pat4(pat_str4);
		pats_in.push_back(pat1);
		pats_in.push_back(pat2);
		pats_out.push_back(pat3);
		pats_out.push_back(pat4);
		Sequence seq2(pats_in, pats_out);
		double comp = seq1.compare(seq2);
		ASSERT(comp > 0 && comp < 1);
	}

	//partial match of different depth sequences
	{
		std::vector<Pattern> pats_in;
		std::vector<Pattern> pats_out;
		std::string pat_str1 = "11000";
		std::string pat_str3 = "01110";
		Pattern pat1(pat_str1);
		Pattern pat3(pat_str3);
		pats_in.push_back(pat1);
		pats_out.push_back(pat3);
		Sequence seq2(pats_in, pats_out);
		double comp = seq1.compare(seq2);
		ASSERT(comp > 0 && comp < 1);
	}
	// input size mismatch
	{
		std::vector<Pattern> pats_in;
		std::vector<Pattern> pats_out;
		std::string pat_str1 = "100";
		std::string pat_str3 = "01110";
		Pattern pat1(pat_str1);
		Pattern pat3(pat_str3);
		pats_in.push_back(pat1);
		pats_out.push_back(pat3);
		Sequence seq2(pats_in, pats_out);
		double comp = seq1.compare(seq2);
		ASSERT_EQUAL_DELTA(0, comp, 0.0001);
	}
	// output size mismatch
	{
		std::vector<Pattern> pats_in;
		std::vector<Pattern> pats_out;
		std::string pat_str1 = "10011";
		std::string pat_str3 = "010";
		Pattern pat1(pat_str1);
		Pattern pat3(pat_str3);
		pats_in.push_back(pat1);
		pats_out.push_back(pat3);
		Sequence seq2(pats_in, pats_out);
		double comp = seq1.compare(seq2);
		ASSERT_EQUAL_DELTA(0, comp, 0.0001);

	}
}
void SequenceTest::testoperatorequals() {
	//std::cout << "SequenceTest::testoperatorequals: " << std::endl;
	Sequence seq1 = getTestSequence();

	// test equals
	{
		std::vector<Pattern> test_pats2;
		std::vector<Pattern> test_pats3;
		std::string pats2_str = ("11101");
		std::string pats3_str = ("11101");
		Pattern pat2(pats2_str);
		Pattern pat3(pats3_str);
		test_pats2.push_back(pat2);
		test_pats3.push_back(pat3);
		Sequence seq2(test_pats2, test_pats3);
		ASSERT(!(seq1 == seq2));
	}
	// test not equal
	{
		Sequence seq2(seq1);
		ASSERT_EQUAL(seq1, seq2);
	}

	// test this is default
	{
		Sequence seq2;
		ASSERT(!(seq1 == seq2));
	}

	// test obj is default
	{
		Sequence seq2;
		ASSERT(!(seq2 == seq1));
	}
}
void SequenceTest::testLeftShiftOperator() {
	ASSERT(false);
}
void SequenceTest::testinitialise() {
	ASSERT(false);
}
void SequenceTest::testisAllZeroes() {
	//test false
	{
		std::vector<Pattern> pats_in;
		std::vector<Pattern> pats_out;
		std::string pat_str1 = "11000";
		std::string pat_str2 = "10101";
		std::string pat_str3 = "01110";
		std::string pat_str4 = "00011";
		Pattern pat1(pat_str1);
		Pattern pat2(pat_str2);
		Pattern pat3(pat_str3);
		Pattern pat4(pat_str4);
		pats_in.push_back(pat1);
		pats_in.push_back(pat2);
		pats_out.push_back(pat3);
		pats_out.push_back(pat4);
		Sequence seq2(pats_in, pats_out);
		bool allzero = seq2.isAllZeroes();
		ASSERT(!allzero);
	}
	//test true
	{
		std::vector<Pattern> pats_in;
		std::vector<Pattern> pats_out;
		std::string pat_str1 = "00000";
		std::string pat_str2 = "00000";
		std::string pat_str3 = "00000";
		std::string pat_str4 = "00000";
		Pattern pat1(pat_str1);
		Pattern pat2(pat_str2);
		Pattern pat3(pat_str3);
		Pattern pat4(pat_str4);
		pats_in.push_back(pat1);
		pats_in.push_back(pat2);
		pats_out.push_back(pat3);
		pats_out.push_back(pat4);
		Sequence seq2(pats_in, pats_out);
		bool allzero = seq2.isAllZeroes();
		ASSERT(allzero);
	}
}
void SequenceTest::testisInputAllZeroes() {
	//test false
	{
		std::vector<Pattern> pats_in;
		std::vector<Pattern> pats_out;
		std::string pat_str1 = "11000";
		std::string pat_str2 = "10101";
		std::string pat_str3 = "01110";
		std::string pat_str4 = "00011";
		Pattern pat1(pat_str1);
		Pattern pat2(pat_str2);
		Pattern pat3(pat_str3);
		Pattern pat4(pat_str4);
		pats_in.push_back(pat1);
		pats_in.push_back(pat2);
		pats_out.push_back(pat3);
		pats_out.push_back(pat4);
		Sequence seq2(pats_in, pats_out);
		bool allzero = seq2.isInputAllZeroes();
		ASSERT(!allzero);
	}
	//test true
	{
		std::vector<Pattern> pats_in;
		std::vector<Pattern> pats_out;
		std::string pat_str1 = "00000";
		std::string pat_str2 = "00000";
		std::string pat_str3 = "00100";
		std::string pat_str4 = "01100";
		Pattern pat1(pat_str1);
		Pattern pat2(pat_str2);
		Pattern pat3(pat_str3);
		Pattern pat4(pat_str4);
		pats_in.push_back(pat1);
		pats_in.push_back(pat2);
		pats_out.push_back(pat3);
		pats_out.push_back(pat4);
		Sequence seq2(pats_in, pats_out);
		bool allzero = seq2.isInputAllZeroes();
		ASSERT(allzero);
	}
}
void SequenceTest::testisOutputAllZeroes() {
	//test false
	{
		std::vector<Pattern> pats_in;
		std::vector<Pattern> pats_out;
		std::string pat_str1 = "11000";
		std::string pat_str2 = "10101";
		std::string pat_str3 = "01110";
		std::string pat_str4 = "00011";
		Pattern pat1(pat_str1);
		Pattern pat2(pat_str2);
		Pattern pat3(pat_str3);
		Pattern pat4(pat_str4);
		pats_in.push_back(pat1);
		pats_in.push_back(pat2);
		pats_out.push_back(pat3);
		pats_out.push_back(pat4);
		Sequence seq2(pats_in, pats_out);
		bool allzero = seq2.isOutputAllZeroes();
		ASSERT(!allzero);
	}
	//test true
	{
		std::vector<Pattern> pats_in;
		std::vector<Pattern> pats_out;
		std::string pat_str1 = "11000";
		std::string pat_str2 = "00110";
		std::string pat_str3 = "00000";
		std::string pat_str4 = "00000";
		Pattern pat1(pat_str1);
		Pattern pat2(pat_str2);
		Pattern pat3(pat_str3);
		Pattern pat4(pat_str4);
		pats_in.push_back(pat1);
		pats_in.push_back(pat2);
		pats_out.push_back(pat3);
		pats_out.push_back(pat4);
		Sequence seq2(pats_in, pats_out);
		bool allzero = seq2.isOutputAllZeroes();
		ASSERT(allzero);
	}
}
//helpers
Sequence SequenceTest::getTestSequence() {
	std::vector<Pattern> test_pats_in;
	std::vector<Pattern> test_pats_out;
	generateTestPatterns(test_pats_in, test_pats_out);
	return Sequence(test_pats_in, test_pats_out);
}
void SequenceTest::generateTestPatterns(std::vector<Pattern> & pats_in, std::vector<Pattern> & pats_out) {
	std::string pat_str1 = "11010";
	std::string pat_str2 = "00101";
	std::string pat_str3 = "11110";
	std::string pat_str4 = "00111";
	Pattern pat1(pat_str1);
	Pattern pat2(pat_str2);
	Pattern pat3(pat_str3);
	Pattern pat4(pat_str4);
	pats_in.push_back(pat1);
	pats_in.push_back(pat2);
	pats_out.push_back(pat3);
	pats_out.push_back(pat4);
}

}
}
