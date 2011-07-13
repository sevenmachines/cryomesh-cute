#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"
#include "PatternChannelTest.h"
#include "common/TimeKeeper.h"

//#define PATTERNCHANNELTEST_DEBUG

namespace cryomesh {
namespace state {
void PatternChannelTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(PatternChannelTest::testPatternChannel));
	s.push_back(CUTE(PatternChannelTest::testaddPatterns));
	s.push_back(CUTE(PatternChannelTest::testaddPatterns_1));
	s.push_back(CUTE(PatternChannelTest::testremovePatterns));
	s.push_back(CUTE(PatternChannelTest::testremovePatterns_1));
	s.push_back(CUTE(PatternChannelTest::testnextPattern));
	s.push_back(CUTE(PatternChannelTest::testpreviousPattern));
	s.push_back(CUTE(PatternChannelTest::testmatchGlobally));
	s.push_back(CUTE(PatternChannelTest::testmatchSequentially));
	s.push_back(CUTE(PatternChannelTest::testgetCurrentPattern));
	s.push_back(CUTE(PatternChannelTest::testgetPatternByCycle));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "PatternChannelTest");
}
void PatternChannelTest::testPatternChannel() {
	// create a uniform list of patterns
	{
		std::list<boost::shared_ptr<Pattern> > patterns;
		boost::shared_ptr<Pattern> pat1(new Pattern("100"));
		boost::shared_ptr<Pattern> pat2(new Pattern("01012"));
		boost::shared_ptr<Pattern> pat3(new Pattern("001124"));
		boost::shared_ptr<Pattern> pat4(new Pattern("110"));
		boost::shared_ptr<Pattern> pat5(new Pattern("011"));
		patterns.push_back(pat1);
		patterns.push_back(pat2);
		patterns.push_back(pat3);
		patterns.push_back(pat4);
		patterns.push_back(pat5);
		// create channel
		PatternChannel pc(patterns, PatternChannel::Input);

		// check length and width
		int width = pc.getWidth();
		int length = pc.getLength();

		ASSERT_EQUAL(3, width);
		ASSERT_EQUAL(5, length);
		//check sizes of containers
		int patlist = pc.getPatternList().size();
		int patmap = pc.getPatternMap().size();

		ASSERT_EQUAL(5, patlist);
		ASSERT_EQUAL(5, patmap);

		// check iterator start and position 1
		int pos = pc.getPatternPosition();
		const std::list<boost::uuids::uuid>::iterator it_now = pc.getPatternListIterator();
		bool at_start = (it_now == pc.getPatternList().begin());

		ASSERT_EQUAL(1, pos);
		ASSERT(at_start);

		// check patternbytag stuff
		int patbytag = pc.getPatternByTagMap().size();
		ASSERT_EQUAL( 5, patbytag);
	}
}
void PatternChannelTest::testaddPatterns() {
	std::list<boost::shared_ptr<Pattern> > patterns1;
	std::list<boost::shared_ptr<Pattern> > patterns2;
	std::list<boost::shared_ptr<Pattern> > patterns3;
	boost::shared_ptr<Pattern> pat1(new Pattern("100"));
	boost::shared_ptr<Pattern> pat2(new Pattern("01012"));
	boost::shared_ptr<Pattern> pat3(new Pattern("001124"));
	boost::shared_ptr<Pattern> pat4(new Pattern("110"));
	boost::shared_ptr<Pattern> pat5(new Pattern("011"));
	patterns1.push_back(pat1);
	patterns1.push_back(pat2);
	patterns2.push_back(pat3);
	patterns2.push_back(pat4);
	patterns3.push_back(pat5);

	// create channel
	PatternChannel pc(patterns1, PatternChannel::Input);

	// insert past end
	pc.addPatterns(patterns2, 4);

	// check size position
	{
		int sz = pc.getLength();
		int pos = pc.getPatternPosition();
		ASSERT_EQUAL(4, sz);
		ASSERT_EQUAL(1, pos);
	}

	// move position past insertion point
	for (int i = 0; i < 3; i++) {
		pc.nextPattern();
	}
	int pre_pos = pc.getPatternPosition();

	pc.addPatterns(patterns3, 2);

	// check size position
	{
		int sz = pc.getLength();
		int pos = pc.getPatternPosition();
		ASSERT_EQUAL(5, sz);
		ASSERT_EQUAL(pre_pos +1, pos);
	}

	// test pattern tag stuff
	{
		int sz = pc.getPatternByTagMap().size();
		ASSERT_EQUAL(5, sz);
	}

	//check patterns are added in order to end
	{
		// order is pat1, pat5, pat2, pat3, pat4
		const std::list<boost::uuids::uuid> & pat_list = pc.getPatternList();

		std::list<boost::uuids::uuid>::const_iterator it_pat_list = pat_list.begin();
		const std::list<boost::uuids::uuid>::const_iterator it_pat_list_end = pat_list.end();

		bool eq = false;
		// match patterns
		{
			eq = (*it_pat_list == pat1->getUUID());
			ASSERT(eq);

			++it_pat_list;
			eq = (*it_pat_list == pat5->getUUID());
			ASSERT(eq);

			++it_pat_list;
			eq = (*it_pat_list == pat2->getUUID());
			ASSERT(eq);

			++it_pat_list;
			eq = (*it_pat_list == pat3->getUUID());
			ASSERT(eq);

			++it_pat_list;
			eq = (*it_pat_list == pat4->getUUID());
			ASSERT(eq);
		}

	}
}
void PatternChannelTest::testaddPatterns_1() {
	std::list<boost::shared_ptr<Pattern> > patterns1;
	std::list<boost::shared_ptr<Pattern> > patterns2;
	boost::shared_ptr<Pattern> pat1(new Pattern("1000"));
	boost::shared_ptr<Pattern> pat2(new Pattern("010"));
	boost::shared_ptr<Pattern> pat3(new Pattern("00100"));
	patterns1.push_back(pat1);
	patterns2.push_back(pat2);
	patterns2.push_back(pat3);

	// create channel
	PatternChannel pc(patterns1, PatternChannel::Input);

	pc.addPatterns(patterns2);

	// test size and minimum width are used
	{
		int sz = pc.getPatternList().size();
		int wd = pc.getWidth();
		ASSERT_EQUAL(3, sz);
		ASSERT_EQUAL(3, wd);

		int tagsz = pc.getPatternByTagMap().size();
		ASSERT_EQUAL(3, sz);
	}
	//check patterns are added in order to end
	{
		const std::list<boost::uuids::uuid> & pat_list = pc.getPatternList();

		std::list<boost::uuids::uuid>::const_iterator it_pat_list = pat_list.begin();
		const std::list<boost::uuids::uuid>::const_iterator it_pat_list_end = pat_list.end();

		bool eq1 = (*it_pat_list == pat1->getUUID());
		ASSERT(eq1);

		++it_pat_list;
		bool eq2 = (*it_pat_list == pat2->getUUID());
		ASSERT(eq2);
		++it_pat_list;
		bool eq3 = (*it_pat_list == pat3->getUUID());
		ASSERT(eq3);

	}
}
void PatternChannelTest::testremovePatterns() {
	// mostly covered by the other remove test
	std::list<boost::shared_ptr<Pattern> > patterns1;
	std::list<boost::shared_ptr<Pattern> > patterns2;
	boost::shared_ptr<Pattern> pat1(new Pattern("1000"));
	boost::shared_ptr<Pattern> pat2(new Pattern("010"));
	boost::shared_ptr<Pattern> pata(new Pattern("00100"));
	patterns1.push_back(pat1);
	patterns1.push_back(pat2);

	patterns2.push_back(pat1);
	patterns2.push_back(pata); //doesnt exist in pc
	// create channel
	PatternChannel pc(patterns1, PatternChannel::Input);

	// delete and test
	{
		pc.removePatterns(patterns2);

		int sz = pc.getLength();
		ASSERT_EQUAL(1,sz);

		// check the inly pattern left is 2
		bool eq = (*(pc.getPatternList().begin()) == pat2->getUUID());
		ASSERT(eq);

		int tagsz = pc.getPatternByTagMap().size();

		ASSERT_EQUAL(1, sz);
	}
}
void PatternChannelTest::testremovePatterns_1() {
	std::list<boost::shared_ptr<Pattern> > patterns1;
	std::list<boost::uuids::uuid> patterns2;
	boost::shared_ptr<Pattern> pat1(new Pattern("1000"));
	boost::shared_ptr<Pattern> pat2(new Pattern("010"));
	boost::shared_ptr<Pattern> pata(new Pattern("00100"));
	patterns1.push_back(pat1);
	patterns1.push_back(pat2);

	patterns2.push_back(pat1->getUUID());
	patterns2.push_back(pata->getUUID()); //doesnt exist in pc
	// create channel
	PatternChannel pc(patterns1, PatternChannel::Input);

	// delete and test
	{
		pc.removePatterns(patterns2);

		int sz = pc.getLength();
		ASSERT_EQUAL(1,sz);

		// check the inly pattern left is 2
		bool eq = (*(pc.getPatternList().begin()) == pat2->getUUID());

		// check position and iterator have been update from pat1
		int pos = pc.getPatternPosition();
		ASSERT_EQUAL(1, pos);

		bool same = *(pc.getPatternListIterator()) == pat2->getUUID();
		ASSERT(same);

	}
}
void PatternChannelTest::testgetCurrentPattern() {

	std::list<boost::shared_ptr<Pattern> > patterns1;
	boost::shared_ptr<Pattern> pat1(new Pattern("1000"));
	boost::shared_ptr<Pattern> pat2(new Pattern("010"));
	boost::shared_ptr<Pattern> pat3(new Pattern("00100"));
	patterns1.push_back(pat1);
	patterns1.push_back(pat2);
	patterns1.push_back(pat3);

	// create channel
	PatternChannel pc(patterns1, PatternChannel::Input);
	pc.nextPattern();

	{
		const boost::shared_ptr<Pattern> current_pat = pc.getCurrentPattern();
#ifdef PATTERNCHANNELTEST_DEBUG
		std::cout << "PatternChannelTest::testgetCurrentPattern: " << "" << std::endl;
		std::cout << "\tpat1: " << pat1->getUUIDString() << std::endl;
		std::cout << "\tpat2: " << pat2->getUUIDString() << std::endl;
		std::cout << "\tpat3: " << pat3->getUUIDString() << std::endl;
		std::cout << "\tcurrent_pat: " << current_pat->getUUIDString() << std::endl;
#endif
		ASSERT_EQUAL(current_pat->getUUIDString() ,pat2->getUUIDString());
	}
}

void PatternChannelTest::testgetPatternByCycle() {

	std::list<boost::shared_ptr<Pattern> > patterns1;
	boost::shared_ptr<Pattern> pat1(new Pattern("1000"));
	boost::shared_ptr<Pattern> pat2(new Pattern("010"));
	boost::shared_ptr<Pattern> pat3(new Pattern("00100"));
	patterns1.push_back(pat1);
	patterns1.push_back(pat2);
	patterns1.push_back(pat3);

	common::Cycle now_cycle = common::TimeKeeper::getTimeKeeper().getCycle();
	const common::Cycle start_cycle = common::TimeKeeper::getTimeKeeper().getCycle();
	for (int i = 0; i < 3; i++) {
		common::TimeKeeper::getTimeKeeper().update();
		++now_cycle;
	}
	// create channel
	PatternChannel pc(patterns1, PatternChannel::Input);
	pc.nextPattern();

	for (int i = 0; i < 4; i++) {
		common::Cycle get_cycle = now_cycle - i;
		const boost::shared_ptr<state::Pattern> temp_pat = pc.getPatternByCycle(get_cycle);
		std::cout << "PatternChannelTest::testGetPatternByCycle: " << "i=" << i << " now_cycle=" << now_cycle
				<< " tk_cycle=" << common::TimeKeeper::getTimeKeeper().getCycle() << " get_cycle=" << get_cycle
				<< std::endl;
		if (i == 0) {
			ASSERT_EQUAL(pat3->getString(), temp_pat->getString());
			ASSERT_EQUAL(pat3->getUUIDString(), temp_pat->getUUIDString());
		} else if (i == 1) {
			ASSERT_EQUAL(pat2->getString(), temp_pat->getString());
			ASSERT_EQUAL(pat2->getUUIDString(), temp_pat->getUUIDString());
		} else if (i == 2) {
			ASSERT_EQUAL(pat1->getString(), temp_pat->getString());
			ASSERT_EQUAL(pat1->getUUIDString(), temp_pat->getUUIDString());
		} else if (i == 3) {
			bool null_ptr = (temp_pat == 0);
			ASSERT(null_ptr);
		}
	}
}

void PatternChannelTest::testnextPattern() {

	std::list<boost::shared_ptr<Pattern> > patterns1;
	boost::shared_ptr<Pattern> pat1(new Pattern("1000"));
	boost::shared_ptr<Pattern> pat2(new Pattern("010"));
	boost::shared_ptr<Pattern> pat3(new Pattern("00100"));
	patterns1.push_back(pat1);
	patterns1.push_back(pat2);
	patterns1.push_back(pat3);

	// create channel
	PatternChannel pc(patterns1, PatternChannel::Input);

	{
		int pre_pos = pc.getPatternPosition();

		const boost::shared_ptr<Pattern> new_pat = pc.nextPattern();
		int pos = pc.getPatternPosition();

#ifdef PATTERNCHANNELTEST_DEBUG
		std::cout << "PatternChannelTest::testnextPattern: " << "" << std::endl;
		std::cout << "\tpat1: " << pat1->getUUIDString() << std::endl;
		std::cout << "\tpat2: " << pat2->getUUIDString() << std::endl;
		std::cout << "\tpat3: " << pat3->getUUIDString() << std::endl;
		std::cout << "\tnew_pat: " << new_pat->getUUIDString() << std::endl;
#endif
		ASSERT_EQUAL(new_pat->getUUIDString() ,pat2->getUUIDString());

		ASSERT_EQUAL(pre_pos +1, pos);
	}

	{
		int pre_pos = pc.getPatternPosition();

		const boost::shared_ptr<Pattern> new_pat = pc.nextPattern();
		bool eq = (new_pat->getUUID() == pat3->getUUID());
		int pos = pc.getPatternPosition();
		ASSERT(eq);
		ASSERT_EQUAL(pre_pos +1, pos);
	}

	// cycle to start
	{

		const boost::shared_ptr<Pattern> new_pat = pc.nextPattern();
		bool eq = (new_pat->getUUID() == pat1->getUUID());
		int pos = pc.getPatternPosition();
		ASSERT(eq);
		ASSERT_EQUAL(1, pos);
	}
}
void PatternChannelTest::testpreviousPattern() {
	std::list<boost::shared_ptr<Pattern> > patterns1;
	boost::shared_ptr<Pattern> pat1(new Pattern("1000"));
	boost::shared_ptr<Pattern> pat2(new Pattern("010"));
	boost::shared_ptr<Pattern> pat3(new Pattern("00100"));
	patterns1.push_back(pat1);
	patterns1.push_back(pat2);
	patterns1.push_back(pat3);

	// create channel
	PatternChannel pc(patterns1, PatternChannel::Input);

	// cycle to end
	{

		const boost::shared_ptr<Pattern> new_pat = pc.previousPattern();
		bool eq = (new_pat->getUUID() == pat3->getUUID());
		int pos = pc.getPatternPosition();
		ASSERT(eq);
		ASSERT_EQUAL(3, pos);
	}

	{
		int pre_pos = pc.getPatternPosition();

		const boost::shared_ptr<Pattern> new_pat = pc.previousPattern();
		bool eq = (new_pat->getUUID() == pat2->getUUID());
		int pos = pc.getPatternPosition();
		ASSERT(eq);
		ASSERT_EQUAL(pre_pos -1, pos);
	}

	{
		int pre_pos = pc.getPatternPosition();

		const boost::shared_ptr<Pattern> new_pat = pc.previousPattern();
		bool eq = (new_pat->getUUID() == pat1->getUUID());
		int pos = pc.getPatternPosition();
		ASSERT(eq);
		ASSERT_EQUAL(pre_pos -1, pos);
	}

}

void PatternChannelTest::testgetPatternByTag(void) {
	std::list<boost::shared_ptr<Pattern> > patterns1;
	boost::shared_ptr<Pattern> pat1(new Pattern("1000"));
	boost::shared_ptr<Pattern> pat2(new Pattern("010"));
	boost::shared_ptr<Pattern> pat3(new Pattern("00100"));
	patterns1.push_back(pat1);
	patterns1.push_back(pat2);
	patterns1.push_back(pat3);
	// create channel
	PatternChannel pc(patterns1, PatternChannel::Input);

	// get a pattern
	boost::shared_ptr<Pattern> test_pat = pc.getPatternByTag(pat2->getPatternTag());

	// check we got the right pattern
	{
		bool eq = (pat2->getUUID() == test_pat->getUUID());
		ASSERT(eq);
	}
}

void PatternChannelTest::testforcePatternListSize() {
	// test simple resize
	{
		boost::shared_ptr<PatternChannel> pc = getTestPatternChannel();

		int pre_sz = pc->getPatternList().size();
		pc->forcePatternListSize(pre_sz - 2);
		int post_sz = pc->getPatternList().size();
		ASSERT_EQUAL(pre_sz-2, post_sz);
	}

	// test resize when our position will now be past end
	{
		boost::shared_ptr<PatternChannel> pc = getTestPatternChannel();

		int pre_sz = pc->getPatternList().size();
		int pre_pos = pc->getPatternPosition();
		for (int i = 0; i < pre_sz - 1; i++) {
			pc->nextPattern();
		}
		pc->forcePatternListSize(pre_sz - 2);
		int post_sz = pc->getPatternList().size();
		int post_pos = pc->getPatternPosition();

		ASSERT_EQUAL(pre_sz-2, post_sz);

		ASSERT_EQUAL(1, post_pos);
	}

}
void PatternChannelTest::testmatchGlobally() {
	std::list<boost::shared_ptr<Pattern> > patlist1;
	boost::shared_ptr<Pattern> pat1(new Pattern("1111111111"));
	boost::shared_ptr<Pattern> pat2(new Pattern("0000011111"));
	boost::shared_ptr<Pattern> pat3(new Pattern("1111100000"));
	boost::shared_ptr<Pattern> pat4(new Pattern("0000000000"));

	boost::shared_ptr<Pattern> pata(new Pattern("1010101010"));
	boost::shared_ptr<Pattern> patb(new Pattern("0101010101"));
	patlist1.push_back(pat1);
	patlist1.push_back(pat2);
	patlist1.push_back(pat3);
	patlist1.push_back(pat4);

	PatternChannel patchan1(patlist1, PatternChannel::Output);

	// check  full match
	{
		std::list<boost::shared_ptr<Pattern> > patlist2;
		patlist2.push_back(pat1);
		patlist2.push_back(pat2);
		patlist2.push_back(pat3);
		patlist2.push_back(pat4);
		PatternChannel patchan2(patlist2, PatternChannel::Output);

		double match = patchan1.matchSequentially(patchan2);
		ASSERT_EQUAL_DELTA(1, match, 0.00001);
	}

	// check  randomised but full match
	{
		std::list<boost::shared_ptr<Pattern> > patlist2;
		patlist2.push_back(pat4);
		patlist2.push_back(pat3);
		patlist2.push_back(pat2);
		patlist2.push_back(pat1);
		PatternChannel patchan2(patlist2, PatternChannel::Output);

		double match = patchan1.matchSequentially(patchan2);
		ASSERT_EQUAL_DELTA(0, match, 0.00001);
	}

	// check  partial match
	{
		std::list<boost::shared_ptr<Pattern> > patlist2;
		patlist2.push_back(pat2);
		patlist2.push_back(pat3);
		patlist2.push_back(pata);
		patlist2.push_back(patb);
		PatternChannel patchan2(patlist2, PatternChannel::Output);

		double match = patchan1.matchSequentially(patchan2);
		ASSERT(match >-1);
		ASSERT(match <1);
	}
}
void PatternChannelTest::testmatchSequentially() {
	std::list<boost::shared_ptr<Pattern> > patlist1;
	boost::shared_ptr<Pattern> pat1(new Pattern("1111111111"));
	boost::shared_ptr<Pattern> pat2(new Pattern("0000011111"));
	boost::shared_ptr<Pattern> pat3(new Pattern("1111100000"));
	boost::shared_ptr<Pattern> pat4(new Pattern("0000000000"));

	patlist1.push_back(pat1);
	patlist1.push_back(pat2);
	patlist1.push_back(pat3);
	patlist1.push_back(pat4);

	PatternChannel patchan1(patlist1, PatternChannel::Output);

	// check  full match
	{
		std::list<boost::shared_ptr<Pattern> > patlist2;
		patlist2.push_back(pat1);
		patlist2.push_back(pat2);
		patlist2.push_back(pat3);
		patlist2.push_back(pat4);
		PatternChannel patchan2(patlist2, PatternChannel::Output);

		double match = patchan1.matchSequentially(patchan2);
		ASSERT_EQUAL_DELTA(1, match, 0.00001);
	}

	// check  no match
	{
		std::list<boost::shared_ptr<Pattern> > patlist2;
		patlist2.push_back(pat4);
		patlist2.push_back(pat3);
		patlist2.push_back(pat2);
		patlist2.push_back(pat1);
		PatternChannel patchan2(patlist2, PatternChannel::Output);

		double match = patchan1.matchSequentially(patchan2);
		ASSERT_EQUAL_DELTA(0, match, 0.00001);
	}

	// check  partial match
	{
		std::list<boost::shared_ptr<Pattern> > patlist2;
		patlist2.push_back(pat2);
		patlist2.push_back(pat3);
		patlist2.push_back(pat4);
		patlist2.push_back(pat1);
		PatternChannel patchan2(patlist2, PatternChannel::Output);

		double match = patchan1.matchSequentially(patchan2);
		ASSERT(match >-1);
		ASSERT(match <1);
	}
}

// HELPERS
boost::shared_ptr<PatternChannel> PatternChannelTest::getTestPatternChannel() {
	std::list<boost::shared_ptr<Pattern> > patterns;
	boost::shared_ptr<Pattern> pat1(new Pattern("100"));
	boost::shared_ptr<Pattern> pat2(new Pattern("01012"));
	boost::shared_ptr<Pattern> pat3(new Pattern("001124"));
	boost::shared_ptr<Pattern> pat4(new Pattern("110"));
	boost::shared_ptr<Pattern> pat5(new Pattern("011"));
	patterns.push_back(pat1);
	patterns.push_back(pat2);
	patterns.push_back(pat3);
	patterns.push_back(pat4);
	patterns.push_back(pat5);
	// create channel
	boost::shared_ptr<PatternChannel> pc(new PatternChannel(patterns, PatternChannel::Input));
	return pc;
}
}
}
