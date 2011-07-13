/*
 * PatternChannelMapTest.cpp
 *
 *  Created on: 13 Jul 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "PatternChannelMapTest.h"
#include "state/PatternChannelMap.h"
#include "ICuteSuite.h"

namespace cryomesh {

namespace state {

void PatternChannelMapTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(PatternChannelMapTest::testGetPatterns));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "PatternChannelMapTest");
}

void PatternChannelMapTest::testGetPatterns() {

	// pattern channel 1
	std::list<boost::shared_ptr<Pattern> > patterns1;
	{
		boost::shared_ptr<Pattern> pat11(new Pattern("1000"));
		boost::shared_ptr<Pattern> pat12(new Pattern("010"));
		boost::shared_ptr<Pattern> pat13(new Pattern("00100"));
		patterns1.push_back(pat11);
		patterns1.push_back(pat12);
	}

	// pattern channel 1
	std::list<boost::shared_ptr<Pattern> > patterns2;
	{
		boost::shared_ptr<Pattern> pat21(new Pattern("101111110"));
		boost::shared_ptr<Pattern> pat22(new Pattern("0111111"));
		boost::shared_ptr<Pattern> pat23(new Pattern("001111111100"));
		patterns2.push_back(pat21);
		patterns2.push_back(pat22);
		patterns2.push_back(pat23);
	}
	// create channel
	boost::shared_ptr<PatternChannel> pc1(new PatternChannel(patterns1, PatternChannel::Input));
	pc1->nextPattern();
	// create channel
	boost::shared_ptr<PatternChannel> pc2(new PatternChannel(patterns2, PatternChannel::Input));
	pc2->nextPattern();

	common::Cycle now_cycle = common::TimeKeeper::getTimeKeeper().getCycle();
	const common::Cycle start_cycle = common::TimeKeeper::getTimeKeeper().getCycle();
	for (int i = 0; i < 3; i++) {
		common::TimeKeeper::getTimeKeeper().update();
		++now_cycle;
	}

	PatternChannelMap pcm;
	pcm.add(pc1);
	pcm.add(pc2);

	for (int i = 0; i < 4; i++) {
		common::Cycle get_cycle = now_cycle - i;
		int found_size = pcm.getPatterns(get_cycle).size();
		if (i == 0) {
			ASSERT_EQUAL(2, found_size);
		} else if (i == 1) {
			ASSERT_EQUAL(2, found_size);
		} else if (i == 2) {
			ASSERT_EQUAL(1, found_size);
		} else if (i == 3) {
			ASSERT_EQUAL(0, found_size);

		}
	}

}

}//NAMESPACE
}//NAMESPACE
