#ifndef TEST_PATTERNCHANNELTEST
#define TEST_PATTERNCHANNELTEST

#include "state/PatternChannel.h"
#include "state/Pattern.h"
#include <boost/shared_ptr.hpp>

namespace cryomesh {
namespace state {
class PatternChannelTest {
public:
	static void runSuite();
	static void testPatternChannel();
	static void testaddPatterns();
	static void testaddPatterns_1();
	static void testremovePatterns();
	static void testremovePatterns_1();
	static void testgetCurrentPattern();
	static void testnextPattern();
	static void testpreviousPattern();

	static void testgetPatternByTag(void);

	static void testforcePatternListSize(void);
	static void testmatchGlobally(void);
	static void testmatchSequentially(void);
	// HELPERS
	static boost::shared_ptr<PatternChannel> getTestPatternChannel();
};
}
}

#endif /* PATTERNCHANNELTEST */
