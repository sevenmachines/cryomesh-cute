#ifndef TEST_PATTERNTEST
#define TEST_PATTERNTEST

#include "state/Pattern.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace cryomesh {
namespace state {

class PatternTest  {
public:
	static void runSuite();
	static void testPattern();
	static void testPattern_cc()	;
	static void testPattern_vec()	;
	static void testPattern_string();
	static void testDPattern()	;
	static void testAssignOperator()	;
	static void testcompare()	;
	static void testoperatorequals()	;
	static void testgetPattern()	;
	static void testsetPattern()	;
	static void testgetId()	;
	static void testsetId()	;
	static void testSerialize();
	static bool compareBools(const std::vector<bool> & obj1, const std::vector<bool> & obj2);

};
}
}

#endif /* PATTERNTEST */
