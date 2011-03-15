#include "TimeKeeperTest.h"

#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

#include <fstream>

namespace cryomesh {
namespace common {
void TimeKeeperTest::runSuite() {
	cute::suite s;

	s.push_back(CUTE(TimeKeeperTest::testserialize));
	s.push_back(CUTE(TimeKeeperTest::testTimeKeeper));
	s.push_back(CUTE(TimeKeeperTest::testupdate));
	s.push_back(CUTE(TimeKeeperTest::testgetCycle));
	s.push_back(CUTE(TimeKeeperTest::testgetTiming));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "TimeKeeperTest");
}
#ifdef ENABLE_SERIALISATION
void TimeKeeperTest::testserialize() {
	TimeKeeper & obj1 = TimeKeeper::getTimeKeeper();
	std::string ser_file = CoreTestsDefaults::TEST_DIRECTORY
	+ "timekeeper_testserialize.txt";
	// save data to archive
	{
		// create and open a character archive for output
		std::ofstream ofs(ser_file.c_str());

		boost::archive::text_oarchive oa(ofs);
		// write class instance to archive
		oa << obj1;
		// archive and stream closed when destructors are called
	}

}
#else
void TimeKeeperTest::testserialize() {
	ASSERTM("Serialisation is disabled", true);
}
#endif

void TimeKeeperTest::testTimeKeeper() {
	TimeKeeper & tk = TimeKeeper::getTimeKeeper();
	// tick is now static so these tets wont work
	//int tick=tk.getTick();
	double time = tk.getTiming();
	//ASSERT_EQUAL(0,tick);
	ASSERT_EQUAL_DELTA(0, time, 0.0001);
}

void TimeKeeperTest::testupdate() {
	TimeKeeper & tk = TimeKeeper::getTimeKeeper();
	tk.update();
	unsigned long int pre_tick = tk.getCycle().toULInt();
	double pre_time = tk.getTiming();
	for (int i = 0; i < 100; ++i) {

	}
	tk.update();
	unsigned long int post_tick = tk.getCycle().toULInt();
	double post_time = tk.getTiming();
	ASSERT_EQUAL(pre_tick + 1, post_tick);
	ASSERT((pre_time < post_time));
}
void TimeKeeperTest::testgetCycle() {

	TimeKeeper & tk = TimeKeeper::getTimeKeeper();
	unsigned long int t1 = tk.getCycle().toULInt();
	mpz_class mpt1 = tk.getCycle().getMP();

	for (int i = 0; i < 10; i++) {
		tk.update();
	}
	unsigned long int tick = tk.getCycle().toULInt();
	mpz_class mpt2 = tk.getCycle().getMP();

	mpz_class mpt3 = mpt1 + 10;

	ASSERT_EQUAL(10 + t1, tick);
	ASSERT_EQUAL(mpt3, mpt2);

}
void TimeKeeperTest::testgetTiming() {

	TimeKeeper & tk = TimeKeeper::getTimeKeeper();
	tk.update();
	double time1 = tk.getTiming();
	tk.update();

	for (int i = 0; i < 10; ++i) {
	}
	tk.update();
	double time2 = tk.getTiming();
	ASSERT(time1 < time2);
}

}
}
