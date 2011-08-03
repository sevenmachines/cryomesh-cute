/**
 * ImpulseCollectionTest.cpp
 *
 *  Created on: 25 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

//#define IMPULSECOLLECTIONTEST_DEBUG

#include "ImpulseCollectionTest.h"
#include "components/ImpulseCollection.h"
#include "common/TimeKeeper.h"

using namespace cryomesh::common;

namespace cryomesh {

namespace components {

void ImpulseCollectionTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(ImpulseCollectionTest::testAddRemoveImpulses));
	s.push_back(CUTE(ImpulseCollectionTest::testClear));
	s.push_back(CUTE(ImpulseCollectionTest::testGetActivity));
	s.push_back(CUTE(ImpulseCollectionTest::testOperators));
	s.push_back(CUTE(ImpulseCollectionTest::testClearActiveCycles));
	s.push_back(CUTE(ImpulseCollectionTest::testDataObjects));

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "ImpulseCollectionTest");
}

void ImpulseCollectionTest::testAddRemoveImpulses() {
	ImpulseCollection ic;
	boost::shared_ptr<Impulse> imp1(new Impulse);
	boost::shared_ptr<Impulse> imp2(new Impulse);

	// test add
	{
		ic.add(imp1);
		{
			unsigned int imps_sz = ic.getCollection().size();
			ASSERT_EQUAL(1, imps_sz);
		}
		ic.add(imp2);
		{
			unsigned int imps_sz = ic.getCollection().size();
			ASSERT_EQUAL(2, imps_sz);
		}
	}

	// test remove by pointer
	{
		ic.remove(imp1);
		{
			unsigned int imps_sz = ic.getCollection().size();
			ASSERT_EQUAL(1, imps_sz);
		}
	}

	// test remove non-existent
	{
		boost::shared_ptr<Impulse> imp3(new Impulse);
		ic.remove(imp3);
		{
			unsigned int imps_sz = ic.getCollection().size();
			ASSERT_EQUAL(1, imps_sz);
		}
	}

	// test remove by uuid
	{
		ic.remove(imp2->getUUID());
		{
			unsigned int imps_sz = ic.getCollection().size();
			ASSERT_EQUAL(0, imps_sz);
		}
	}

}
void ImpulseCollectionTest::testClear() {
	ImpulseCollection ic;
	for (int i = 0; i < 5; i++) {
		TimeKeeper::getTimeKeeper().update();
	}
	// test clear zeroes
	{
		boost::shared_ptr<Impulse> imp1(new Impulse(1, 5));
		boost::shared_ptr<Impulse> imp2(new Impulse(1, 4));
		boost::shared_ptr<Impulse> imp3(new Impulse(1, 3));

		//	std::cout<<"ImpulseCollectionTest::testClear: "<<"BASE_CYCLE: "<<BASE_CYCLE<<std::endl;
		ic.add(imp1);
		ic.add(imp2);
		ic.add(imp3);
		// assert size
		{
			unsigned int ic_sz = ic.getSize();
			ASSERT_EQUAL(3, ic_sz);
		}

		ic.clearImpulses(0, 6);

		// assert size
		{
			unsigned int ic_sz = ic.getSize();
			ASSERT_EQUAL(0, ic_sz);
		}
	}

	// test clear active
	{
		boost::shared_ptr<Impulse> imp1(new Impulse);
		boost::shared_ptr<Impulse> imp2(new Impulse);
		boost::shared_ptr<Impulse> imp3(new Impulse);
		boost::shared_ptr<Impulse> imp4(new Impulse);
		boost::shared_ptr<Impulse> imp5(new Impulse);

		imp1->randomise();
		imp2->randomise();
		imp3->randomise();
		imp4->randomise();
		imp5->randomise();

		Cycle now_cycle = common::TimeKeeper::getTimeKeeper().getCycle();

		imp1->setFirstActiveCycle(2);
		imp2->setFirstActiveCycle(4);
		imp3->setFirstActiveCycle(6);
		imp4->setFirstActiveCycle(8);
		imp5->setFirstActiveCycle(10);

		ic.add(imp2);

		const unsigned int SIZE = 1;
		// assert size
		{
			unsigned int ic_sz = ic.getSize();
			ASSERT_EQUAL(SIZE, ic_sz);
		}

		// clears before start and after end
		{
			Cycle first_cycle = imp2->getFirstActiveCycle();
			Cycle second_cycle = imp2->getLastActiveCycle();
			ic.clearImpulses(first_cycle - 1, second_cycle + 1);
		}

		// assert size
		{
			unsigned int ic_sz = ic.getSize();
			ASSERT_EQUAL(SIZE-1, ic_sz);
		}
		ic.add(imp1);

		// doesnt clear, start after beginning
		{
			Cycle first_cycle = imp1->getFirstActiveCycle();
			Cycle second_cycle = imp1->getLastActiveCycle();
			ic.clearImpulses(first_cycle + 1, second_cycle + 1);
		}
		// assert size
		{
			unsigned int ic_sz = ic.getSize();
			ASSERT_EQUAL(SIZE, ic_sz);
		}

		// doesnt clear, end before real end
		{
			Cycle first_cycle = imp1->getFirstActiveCycle();
			Cycle second_cycle = imp1->getLastActiveCycle();
			ic.clearImpulses(first_cycle - 1, second_cycle - 1);
		}
		// assert size
		{
			unsigned int ic_sz = ic.getSize();
			ASSERT_EQUAL(SIZE, ic_sz);
		}

		// doesnt clear, end non-inclusive
		{
			Cycle first_cycle = imp1->getFirstActiveCycle();
			Cycle second_cycle = imp1->getLastActiveCycle();
			ic.clearImpulses(first_cycle - 1, second_cycle);
		}

		// assert size
		{
			unsigned int ic_sz = ic.getSize();
			ASSERT_EQUAL(SIZE, ic_sz);
		}

		// clears imp1
		{
			Cycle first_cycle = imp1->getFirstActiveCycle();
			Cycle second_cycle = imp1->getLastActiveCycle();
			ic.clearImpulses(first_cycle, second_cycle + 1);
		}

		// assert size
		{
			unsigned int ic_sz = ic.getSize();
			ASSERT_EQUAL(SIZE-1, ic_sz);
		}
		ic.add(imp3);

		// zero clear version, doesnt clear imp3, second cycle non-inclusive
		{
			Cycle first_cycle = imp3->getFirstActiveCycle();
			Cycle second_cycle = imp3->getLastActiveCycle();
			ic.clearImpulses(second_cycle);
		}

		// assert size
		{
			unsigned int ic_sz = ic.getSize();
			ASSERT_EQUAL(SIZE, ic_sz);
		}

		// zero clear version, clears imp3
		{
			Cycle first_cycle = imp3->getFirstActiveCycle();
			Cycle second_cycle = imp3->getLastActiveCycle();
			ic.clearImpulses(0, second_cycle + 1);
		}

		// assert size
		{
			unsigned int ic_sz = ic.getSize();
			ASSERT_EQUAL(SIZE-1, ic_sz);
		}
		ic.add(imp4);
		// check switched params
		{
			Cycle first_cycle = imp4->getFirstActiveCycle();
			Cycle second_cycle = imp4->getLastActiveCycle();
			ic.clearImpulses(second_cycle + 1, 0);
		}

		// assert size
		{
			unsigned int ic_sz = ic.getSize();
			ASSERT_EQUAL(SIZE-1, ic_sz);
		}
		ic.add(imp5);
		/*
		 // check equal params
		 {
		 int first_cycle = imp5->getFirstActiveCycle();
		 int second_cycle = imp5->getLastActiveCycle();
		 ic.clearImpulses(first_cycle - 1, second_cycle + 1);
		 }
		 ic.clearImpulses(now_cycle + 10, now_cycle + 10);

		 // assert size
		 {
		 unsigned int ic_sz = ic.getSize();
		 ASSERT_EQUAL(SIZE-5, ic_sz);
		 }
		 */
	}
}
void ImpulseCollectionTest::testGetActivity() {
	// a vague check of summation

	boost::shared_ptr<Impulse> imp1(new Impulse);
	boost::shared_ptr<Impulse> imp2(new Impulse);
	imp1->randomise();
	imp1->setActivityDelay(0);
	imp2->randomise();
	imp2->setActivityDelay(0);
	imp1->setFirstActiveCycle(2);
	imp2->setFirstActiveCycle(2);

	ImpulseCollection ic1;
	ImpulseCollection ic2;
	ImpulseCollection ic12;

	ic1.add(imp1);
	ic2.add(imp2);
	ic12.add(imp1);
	ic12.add(imp2);

	double sum1 = ic1.getActivity(3);
	double sum2 = ic2.getActivity(3);
	double sum12 = ic12.getActivity(3);

#ifdef IMPULSECOLLECTIONTEST_DEBUG
	std::cout << "ImpulseCollectionTest::testGetActivity: " << "ic1: " << std::endl;
	std::cout << ic1 << std::endl;
	std::cout << "ImpulseCollectionTest::testGetActivity: " << "ic2: " << std::endl;
	std::cout << ic2 << std::endl;
	std::cout << "ImpulseCollectionTest::testGetActivity: " << "ic12: " << std::endl;
	std::cout << ic12 << std::endl;
std::cout<<"ImpulseCollectionTest::testGetActivity: "<<"sum1="<<sum1<<" sum2="<<sum2<<std::endl;
	#endif
	ASSERT(sum1!=0);
	ASSERT(sum2!=0);
	ASSERT_EQUAL_DELTA(sum1+sum2, sum12, 0.0001);

}

void ImpulseCollectionTest::testOperators() {
	boost::shared_ptr<Impulse> imp1(new Impulse);
	boost::shared_ptr<Impulse> imp2(new Impulse);
	boost::shared_ptr<Impulse> imp3(new Impulse);
	boost::shared_ptr<Impulse> imp4(new Impulse);
	boost::shared_ptr<Impulse> imp5(new Impulse);

	imp1->randomise();
	imp2->randomise();
	imp3->randomise();
	imp4->randomise();
	imp5->randomise();

	Cycle now_cycle = common::TimeKeeper::getTimeKeeper().getCycle();

	imp1->setFirstActiveCycle(2);
	imp2->setFirstActiveCycle(4);
	imp3->setFirstActiveCycle(6);
	imp4->setFirstActiveCycle(8);
	imp5->setFirstActiveCycle(10);

	ImpulseCollection obj1;
	obj1.add(imp1);
	obj1.add(imp2);
	obj1.add(imp3);

	// test op==
	{
		ImpulseCollection obj2;
		obj2.add(imp1);
		obj2.add(imp2);
		obj2.add(imp3);
		bool equality = (obj1 == obj2);
		ASSERT(equality );
	}

	// test op=
	{
		ImpulseCollection obj2 = obj1;
		bool equality = obj1 == obj2;
		ASSERT(equality );
	}

	// test op+
	{
		ImpulseCollection obj4;
		ImpulseCollection obj2 = obj1;
		ImpulseCollection obj3 = obj1 + obj1;

		{

			bool equality = obj1 == obj2;
			ASSERT(equality );
		}
		{

			bool equality = obj3 == obj4;
			ASSERT(!equality );
		}
	}
	// test op+=
	{
		ImpulseCollection obj2;
		obj2.add(imp4);
		obj2.add(imp5);
		ImpulseCollection obj2copy = obj2;
		//	std::cout << "ImpulseCollectionTest::testOperators: " << "DEBUG" << std::endl;
		//	std::cout << "obj2 = obj1  &  obj2copy = obj2" << std::endl;
		//	std::cout << "obj1: " << obj1 << std::endl;
		//	std::cout << "obj2: " << obj2 << std::endl;
		//	std::cout << "obj2copy: " << obj2copy << std::endl;
		obj2 += obj1;
		//	std::cout << "obj2 += obj1" << std::endl;
		//	std::cout << "obj1: " << obj1 << std::endl;
		//	std::cout << "obj2: " << obj2 << std::endl;
		//	std::cout << "obj2copy: " << obj2copy << std::endl;

		bool inequality = (obj2copy != obj2);
		ASSERT(inequality );
	}
}

void ImpulseCollectionTest::testClearActiveCycles() {
	boost::shared_ptr<Impulse> imp1(new Impulse(1, 3));
	boost::shared_ptr<Impulse> imp2(new Impulse(1, 2));
	boost::shared_ptr<Impulse> imp3(new Impulse(1, 2));
	boost::shared_ptr<Impulse> imp4(new Impulse(1, 2));

	// on cycle
	ImpulseCollection obj1;
	obj1.add(imp1);
	obj1.add(imp2);
	obj1.add(imp3);
	obj1.add(imp4);

	common::TimeKeeper & tk = common::TimeKeeper::getTimeKeeper();
	Cycle now_cycle = common::TimeKeeper::getTimeKeeper().getCycle();
	imp1->setFirstActiveCycle(2);
	imp2->setFirstActiveCycle(4);
	imp3->setFirstActiveCycle(6);
	imp4->setFirstActiveCycle(10);

	tk.reset();

	const int SIZE = 4;
	// sanity check
	{

		// check size
		{
			int sz = obj1.getCollection().size();
			ASSERT_EQUAL(SIZE, sz);
		}
	}

	// none active on 1
	{
		obj1.clearActiveImpulses();
		obj1.clearActiveImpulses(1);
		obj1.clearActiveImpulses(1, 1);
		// check size
		{
			int sz = obj1.getCollection().size();
			ASSERT_EQUAL(SIZE, sz);
		}
	}

	// On cycle4, imp1 and imp2  active
	{
		for (int i = 0; i < 5; i++) {
			tk.update();
		}
		obj1.clearActiveImpulses();
		// check size
		{
			int sz = obj1.getCollection().size();
			ASSERT_EQUAL(SIZE-2, sz);
		}
	}

	// none active 9
	{
		obj1.clearActiveImpulses(9);
		// check size
		{
			int sz = obj1.getCollection().size();
			ASSERT_EQUAL(SIZE-2, sz);
		}
	}
	// imp3 active on 8, imp4 on 10
	{
		// none active 9
		{
			obj1.clearActiveImpulses(8, 11);
			// check size
			{
				int sz = obj1.getCollection().size();
				ASSERT_EQUAL(SIZE-4, sz);
			}
		}
	}
}

void ImpulseCollectionTest::testDataObjects() {
	// test objects are automatically refreshed
	ImpulseCollection impcoll;
	impcoll.enableLogging(true);
	boost::shared_ptr<Impulse> imp1 = Impulse::getRandom();
	boost::shared_ptr<Impulse> imp2 = Impulse::getRandom();

	//test
	double ave0 = 0;
	{
		std::vector < boost::shared_ptr<Impulse> > all_impulses = impcoll.getObjectList();
		ASSERT_EQUAL(0, all_impulses.size());

		// get average
		ave0 = impcoll.getDataObject().getAverageValue();
		ASSERT_EQUAL_DELTA(0, ave0, 0.00001);

	}

	for (int i = 0; i < 100; i++) {
		TimeKeeper::getTimeKeeper().update();
		//std::cout<<"ImpulseCollectionTest::testDataObjects: "<<i<<":"<<impcoll.getActivity()<<std::endl;
	}
	impcoll.add(imp1);
	//test
	double ave1 = 0;
	{
		std::vector < boost::shared_ptr<Impulse> > all_impulses = impcoll.getObjectList();
		ASSERT_EQUAL(1, all_impulses.size());

		// get average
		ave1 = impcoll.getDataObject().getAverageValue();
		ASSERT(ave1 !=ave0);

	}

	impcoll.add(imp2);
	//test
	double ave2 = 0;
	{
		std::vector < boost::shared_ptr<Impulse> > all_impulses = impcoll.getObjectList();
		ASSERT_EQUAL(2, all_impulses.size());

		// get average
		ave2 = impcoll.getDataObject().getAverageValue();
		ASSERT(ave1 !=ave2);

	}

}

}//NAMESPACE

}//NAMESPACE
