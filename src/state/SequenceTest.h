#ifndef TEST_SEQUENCETEST
#define TEST_SEQUENCETEST

#include "state/Sequence.h"
#include "state/Pattern.h"
#include <vector>
#include <iostream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
namespace cryomesh {
namespace state {
class SequenceTest {
public:
	static void runSuite();

	static void testserialize() ;
	static void testSequence() ;
	static void testDSequence() ;
	static void testSequence_sequence() ;
	static void testSequence_vectors() ;

	static void testAssignOperator() ;
	static void testgetCurrentPattern() ;
	static void testgetNextPattern() ;
	static void testgetSetPatterns() ;
	static void testsaveLoadToFile() ;

	static void testcompare_pattern() ;
	static void testcompare_sequence() ;
	static void testoperatorequals() ;
	static void testLeftShiftOperator() ;
	static void testinitialise() ;
	static void testisAllZeroes() ;
	static void testisInputAllZeroes() ;
	static void testisOutputAllZeroes() ;
	//helpers
	static Sequence getTestSequence() ;
	static void generateTestPatterns(std::vector<Pattern> & pats_in, std::vector<
			Pattern> & pats_out) ;

};
}
}
#endif /* SEQUENCETEST */
