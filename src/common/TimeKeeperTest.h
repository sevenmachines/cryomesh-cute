#ifndef TEST_TIMEKEEPERTEST
#define TEST_TIMEKEEPERTEST
#include "common/TimeKeeper.h"
#include "CoreTestsDefaults.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
namespace cryomesh {
namespace common {
class TimeKeeperTest {

public:
	static void runSuite();
	static void testserialize();
	static void testTimeKeeper();

	static void testupdate();
	static void testgetCycle();
	static void testgetTiming();


};

}
}
#endif /* TIMEKEEPERTEST */
