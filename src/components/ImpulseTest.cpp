/**
 * ImpulseTest.cpp
 *
 *  Created on: 27 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#include "ImpulseTest.h"
#include "components/Impulse.h"

namespace cryomesh {

namespace components {

ImpulseTest::ImpulseTest() {

}

ImpulseTest::~ImpulseTest() {
}

void ImpulseTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(ImpulseTest::testActivityCreation));
	s.push_back(CUTE(ImpulseTest::testActivityMaxMin));
	s.push_back(CUTE(ImpulseTest::testActiveCycles));
	s.push_back(CUTE(ImpulseTest::testOperators));
	s.push_back(CUTE(ImpulseTest::testIsActive));
	s.push_back(CUTE(ImpulseTest::testRandom));

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "ImpulseTest");
}

void ImpulseTest::testActivityCreation() {
	const unsigned int LENGTH = 10;
	const double MAX_Y_PLUS = 1.5;
	const double MAX_Y_NEG = -1.5;
	const double DELTA = 0.0001;
	// test positive creation
	{
		Impulse imp(MAX_Y_PLUS, LENGTH);
		const std::list<double> & acts = imp.getActivities();

		std::list<double>::const_iterator it_acts = acts.begin();
		const std::list<double>::const_iterator it_acts_end = acts.end();
		for (unsigned int i = 0; i < LENGTH; ++i) {
			//	std::cout << "ImpulseTest::testActivityCreation: " << i << ":" << *it_acts << std::endl;
			ASSERT(*it_acts>0);
			++it_acts;
		}
	}

	// test negative creation
	{
		Impulse imp(MAX_Y_NEG, LENGTH);
		const std::list<double> & acts = imp.getActivities();

		std::list<double>::const_iterator it_acts = acts.begin();
		const std::list<double>::const_iterator it_acts_end = acts.end();
		for (unsigned int i = 0; i < LENGTH; ++i) {
			//	std::cout<<"ImpulseTest::testActivityCreation: "<<i<<":"<<*it_acts<<std::endl;
			ASSERT(*it_acts<0);
			++it_acts;
		}
	}
}

void ImpulseTest::testActivityMaxMin() {
	const unsigned int LENGTH = 10;
	const double MAX_Y_PLUS = 1.5;
	const double MAX_Y_NEG = -1.5;
	const double DELTA = 0.0001;
	// test max
	{
		Impulse imp(MAX_Y_PLUS, LENGTH);
		double max = imp.getMaximum();
		double min = imp.getMinimum();
		ASSERT_EQUAL_DELTA(MAX_Y_PLUS, max, DELTA);
		ASSERT(min>0);
	}
	// test min
	{
		Impulse imp(MAX_Y_NEG, LENGTH);
		double max = imp.getMaximum();
		double min = imp.getMinimum();
		ASSERT(max < 0);
		ASSERT_EQUAL_DELTA(MAX_Y_NEG, min, DELTA);
	}

}

void ImpulseTest::testActiveCycles() {

	//test summation
	Impulse obj1(2, 10);
	obj1.setFirstActiveCycle(2);
	// before start
	{
		double act = obj1.getActivity(1);
		ASSERT_EQUAL(0,act);
	}

	// after end
	{
		double act = obj1.getActivity(13);
		ASSERT_EQUAL(0,act);
	}

	// on start
	{
		double act = obj1.getActivity(2);
		ASSERT(act!=0);
	}

	// on end
	{
		double act = obj1.getActivity(12);
		ASSERT(act!=0);
	}

	//between start and end
	{
		for (int i = 2; i < 12; ++i) {
			double act = obj1.getActivity(i);
			ASSERT(act!=0);
		}
	}

}

void ImpulseTest::testOperators() {
	// test op==
	{
		Impulse obj1(2, 10);
		obj1.setFirstActiveCycle(2);
		Impulse obj2(2, 10);
		obj2.setFirstActiveCycle(2);
		bool equality = (obj1 == obj2);
		ASSERT(equality );
	}

	// test op=
	{
		Impulse obj1(2, 10);
		obj1.setFirstActiveCycle(2);
		Impulse obj2 = obj1;
		bool equality = obj1 == obj2;
		ASSERT(equality );
	}

	// test op+
	{
		Impulse obj1(2, 10);
		obj1.setFirstActiveCycle(2);
		Impulse obj4(1, 2);
		obj1.setFirstActiveCycle(2);
		Impulse obj2 = obj1;

		Impulse obj3 = obj1 + obj1;

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

		// overlapping
		{
			Impulse obj1(2, 10); // [2, 12]
			obj1.setFirstActiveCycle(2);
			Impulse obj2(4, 12); // [5,17]
			obj2.setFirstActiveCycle(5);
			Impulse obj2copy = obj2;
			//std::cout << "ImpulseTest::testOperators: " << "" << std::endl;
			//std::cout << "obj1: " << common::Containers::print(std::cout, obj1.getCollection()) << std::endl;
			//std::cout << "obj2: " << common::Containers::print(std::cout, obj2.getCollection()) << std::endl;
			obj2 += obj1; // [2, 17]

			int newsz = obj2.getSize();
			ASSERT_EQUAL(15, newsz);
			unsigned long int first_act = obj2.getFirstActiveCycle().toULInt();
			unsigned long int last_act = obj2.getLastActiveCycle().toULInt();
			ASSERT_EQUAL(2, first_act);
			ASSERT_EQUAL(17, last_act);
//std::cout << "ImpulseTest::testOperators: " << "" << std::endl;
			//std::cout << "obj2+=obj1: "  << std::endl;
			//common::Containers::print(std::cout, obj2.getCollection());
			// all should have a value
			const std::list<double> & vals = obj2.getCollection();
			// forall in vals
			{
				unsigned int count = 0;
				std::list<double>::const_iterator it_vals = vals.begin();
				const std::list<double>::const_iterator it_vals_end = vals.end();
				while (it_vals != it_vals_end) {
					ASSERT(*it_vals!=0);
					//std::cout << "ImpulseTest::testOperators: " << *it_vals << std::endl;
					++it_vals;
				}
			}

			bool inequality = (obj2copy != obj2);
			ASSERT(inequality );
		}
	}
}

void ImpulseTest::testIsActive() {
	Impulse obj1(2, 10);
	obj1.setFirstActiveCycle(2);

	// not active before
	{
		ASSERT(!obj1.isActive(1));
	}

	// not active after
	{
		ASSERT(!obj1.isActive(13));
	}

	// active in middle
	{
		ASSERT(obj1.isActive(5));
	}

	// active at start point
	{
		ASSERT(obj1.isActive(2));
	}

	// active at end point
	{
		ASSERT(obj1.isActive(12));
	}

}

void ImpulseTest::testRandom() {
	ASSERTM("TODO", false);
}
}//NAMESPACE

}//NAMESPACE
