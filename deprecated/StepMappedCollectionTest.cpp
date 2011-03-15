/**
 * StepMappedCollectionTest.cpp
 *
 *  Created on: 9 Feb 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#include "StepMappedCollectionTest.h"
#include "common/TimeKeeper.h"
#include "components/ActivityTimerDistance.h"

using namespace cryomesh::common;

namespace cryomesh {
namespace common {

void StepMappedCollectionTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(StepMappedCollectionTest::testGeneral));
	/**
	s.push_back(CUTE(StepMappedCollectionTest::testGetRange));
	s.push_back(CUTE(StepMappedCollectionTest::testClear));
	s.push_back(CUTE(StepMappedCollectionTest::testGetValue));
	*/
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "StepMappedCollectionTest");
}

void StepMappedCollectionTest::testGeneral() {
	StepMappedCollection<components::Impulse, components::ActivityTimerDistance> smc;

	boost::shared_ptr<components::Impulse> obj1(new components::Impulse(1, 8));
	boost::shared_ptr<components::Impulse> obj2(new components::Impulse(1, 6));
	boost::shared_ptr<components::Impulse> obj3(new components::Impulse(1, 4));
	boost::shared_ptr<components::Impulse> obj4(new components::Impulse(1, 2));
	const int SIZE = 3;
	//add
	{
		smc.add(components::ActivityTimerDistance(1,2), obj1);
		smc.add(components::ActivityTimerDistance(1,3), obj2);
		smc.add(components::ActivityTimerDistance(1,4), obj3);

		// size check
		{
			int sz = smc.getSize();
			ASSERT_EQUAL(SIZE, sz);
		}
	}
	//std::cout<<"StepMappedCollectionTest::testGeneral: "<<"DEBUG1"<<std::endl<<smc<<std::endl;
	//remove
	{
		smc.remove(obj1);
		smc.remove(obj4); //doesnt exist
		//std::cout<<"StepMappedCollectionTest::testGeneral: "<<"DEBUG2"<<smc<<std::endl;

		// size check
		{
			int sz = smc.getSize();
			ASSERT_EQUAL(SIZE-1, sz);
		}
		smc.remove(obj2);
		smc.remove(obj3);
		// size check
		{
			int sz = smc.getSize();
			ASSERT_EQUAL(0, sz);
		}
	}
}

/**
void StepMappedCollectionTest::testGetRange() {
	StepMappedCollection<common::Cycle, components::Impulse> smc;
	boost::shared_ptr<components::Impulse> obj1(new components::Impulse);
	boost::shared_ptr<components::Impulse> obj2(new components::Impulse);
	boost::shared_ptr<components::Impulse> obj3(new components::Impulse);
	boost::shared_ptr<components::Impulse> obj4(new components::Impulse);
	boost::shared_ptr<components::Impulse> obj5(new components::Impulse);
	const int SIZE = 5;
	smc.add(Cycle(1), obj1);
	smc.add(Cycle(2), obj2);
	smc.add(Cycle(3), obj3);
	smc.add(Cycle(4), obj4);
	smc.add(Cycle(5), obj5);
	// size check
	{
		int sz = smc.getSize();
		ASSERT_EQUAL(SIZE, sz);
	}

	// [1, 3)
	{
		int range = smc.getRange(Cycle(1), Cycle(3)).size();
		ASSERT_EQUAL(2, range);
	}

	// [0, 1)
	{
		int range = smc.getRange(Cycle(0), Cycle(1)).size();
		ASSERT_EQUAL(0, range);
	}

	// [5, 10)
	{
		int range = smc.getRange(Cycle(5), Cycle(10)).size();
		ASSERT_EQUAL(1, range);
	}
}
void StepMappedCollectionTest::testClear() {
	StepMappedCollection<common::Cycle, components::Impulse> smc;

	common::TimeKeeper & tk = common::TimeKeeper::getTimeKeeper();
	tk.reset();
	boost::shared_ptr<components::Impulse> obj1(new components::Impulse);
	boost::shared_ptr<components::Impulse> obj2(new components::Impulse);
	boost::shared_ptr<components::Impulse> obj3(new components::Impulse);
	boost::shared_ptr<components::Impulse> obj4(new components::Impulse);
	boost::shared_ptr<components::Impulse> obj5(new components::Impulse);
	const int SIZE = 5;
	smc.add(Cycle(1), obj1);
	smc.add(Cycle(2), obj2);
	smc.add(Cycle(3), obj3);
	smc.add(Cycle(4), obj4);
	smc.add(Cycle(5), obj5);
	// size check
	{
		int sz = smc.getSize();
		ASSERT_EQUAL(SIZE, sz);
	}

	//clear 0, none deleted
	{
		smc .clearRange(Cycle(0));
		int sz = smc.getSize();
		ASSERT_EQUAL(SIZE, sz);
	}
	//clear [0, 1), none deleted
	{
		smc .clearRange(Cycle(0), Cycle(1));
		int sz = smc.getSize();
		ASSERT_EQUAL(SIZE, sz);
	}
	// clear [5,5), none deleted
	{
		smc .clearRange(Cycle(5), Cycle(5));
		int sz = smc.getSize();
		ASSERT_EQUAL(SIZE, sz);
	}

	// clear current, obj1 removed
	{
		smc.update(Cycle(2));
		smc.clearRange();
		int sz = smc.getSize();
		ASSERT_EQUAL(SIZE-1, sz);
		tk.reset();
	}

	// clear [2,3), imp2 deleted
	{
		smc .clearRange(Cycle(2), Cycle(3));
		int sz = smc.getSize();
		ASSERT_EQUAL(SIZE-2, sz);
	}

	//clear [3, 5), obj3 & 4 deleted
	{
		smc .clearRange(Cycle(3), Cycle(5));
		int sz = smc.getSize();
		ASSERT_EQUAL(SIZE-4, sz);
	}
	//clear 7, obj1 deleted
	{
		smc .clearRange(Cycle(7));
		int sz = smc.getSize();
		ASSERT_EQUAL(SIZE-5, sz);
	}
}

void StepMappedCollectionTest::testGetValue() {
	StepMappedCollection<common::Cycle, components::Impulse> smc;
	common::TimeKeeper & tk = common::TimeKeeper::getTimeKeeper();
	tk.reset();
	boost::shared_ptr<components::Impulse> obj1(new components::Impulse(1, 8));
	boost::shared_ptr<components::Impulse> obj2(new components::Impulse(1, 6));
	boost::shared_ptr<components::Impulse> obj3(new components::Impulse(1, 4));
	const int SIZE = 3;
	//add
	{
		smc.add(Cycle(1), obj1);
		smc.add(Cycle(2), obj2);
		smc.add(Cycle(3), obj3);

		// size check
		{
			int sz = smc.getSize();
			ASSERT_EQUAL(SIZE, sz);
		}
	}

	// 0<  sum1<sum2<sum3 == sum4
	{
		const double DELTA = 0.00001;
		double sum0 = smc.getValue()->getActivity();
		double sum0a = smc.getValue(1)->getActivity();
		ASSERT_EQUAL_DELTA(0, sum0, DELTA);
		ASSERT_EQUAL_DELTA(0, sum0a, DELTA);
		tk.update();
		double sum1 = smc.getValue()->getActivity(); //get current at 1, obj1
		double sum1a = smc.getValue(2)->getActivity(); // obj1
		ASSERT( sum1>0);
		ASSERT_EQUAL_DELTA(sum1, sum1a, DELTA);

		double sum2 = smc.getValue(3)->getActivity(); // value [0,3), obj1+obj2
		ASSERT( sum2>sum1);
		double sum3 = smc.getValue(2, 4)->getActivity(); // obj2 +obj3
		ASSERT( sum3>sum2);
		double sum4 = smc.getValue(2, 5)->getActivity();
		ASSERT_EQUAL_DELTA(sum3, sum4, DELTA);
	}
}
*/
}//NAMESPACE
}//NAMESPACE
