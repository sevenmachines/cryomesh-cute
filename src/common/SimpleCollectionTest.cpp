/**
 * SimpleCollectionTest.cpp
 *
 *  Created on: 27 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#include "SimpleCollectionTest.h"
#include "common/SimpleCollection.h"

namespace cryomesh {

namespace common {

SimpleCollectionTest::SimpleCollectionTest() {
}

SimpleCollectionTest::~SimpleCollectionTest() {
}

void SimpleCollectionTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(SimpleCollectionTest::testAddRemove));
	s.push_back(CUTE(SimpleCollectionTest::testSum));
	s.push_back(CUTE(SimpleCollectionTest::testMaxMin));
	s.push_back(CUTE(SimpleCollectionTest::testGetValueAt));
	s.push_back(CUTE(SimpleCollectionTest::testInverseReverse));
	s.push_back(CUTE(SimpleCollectionTest::testOperators));
	s.push_back(CUTE(SimpleCollectionTest::testPad));
	s.push_back(CUTE(SimpleCollectionTest::testAddRemoveList));

	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "SimpleCollectionTest.cpp");
}

void SimpleCollectionTest::testAddRemove() {
	SimpleCollection<int> sc;

	// test add
	{
		sc.add(1);
		sc.add(2);
		sc.add(3);
		ASSERT_EQUAL(3, sc.getSize());
	}
	// test remove
	{
		sc.remove(1);
		ASSERT_EQUAL(2, sc.getSize());
		sc.remove(7);
		ASSERT_EQUAL(2, sc.getSize());
		sc.remove(3);
		ASSERT_EQUAL(1, sc.getSize());
		sc.remove(2);
		ASSERT_EQUAL(0, sc.getSize());
		sc.remove(2);
		ASSERT_EQUAL(0, sc.getSize());
	}
}

void SimpleCollectionTest::testSum() {
	SimpleCollection<int> sc;

	// test add
	{
		sc.add(1);
		sc.add(2);
		sc.add(3);
		ASSERT_EQUAL(6, sc.getSum());
	}
}

void SimpleCollectionTest::testMaxMin() {
	SimpleCollection<int> sc;

	// test add
	{
		sc.add(1);
		sc.add(2);
		sc.add(3);
		ASSERT_EQUAL(3, sc.getMaximum());
		ASSERT_EQUAL(1, sc.getMinimum());
	}
}

void SimpleCollectionTest::testGetValueAt() {
	SimpleCollection<int> sc;

	sc.add(1);
	sc.add(2);
	sc.add(3);

	int first = sc[0];
	int last = sc[2];
	ASSERT_EQUAL(1, first);
	ASSERT_EQUAL(3, last);

	// test exception
	try {
		int none = sc[4];
		ASSERTM("Exception missed!", false);
	} catch (exceptions::IndexOutOfBoundsException & except) {
		ASSERT(true);
	}
}
void SimpleCollectionTest::testInverseReverse() {
	SimpleCollection<int> sc;

	sc.add(1);
	sc.add(2);
	sc.add(3);

	int inv = sc.invert()[1];
	int rev = sc.reverse()[2];

	ASSERT_EQUAL(-2, inv);
	ASSERT_EQUAL(-1, rev);

}

void SimpleCollectionTest::testOperators() {
	SimpleCollection<int> sc1;
	sc1.add(1);
	sc1.add(2);
	sc1.add(3);

	// test op==
	{
		SimpleCollection<int> sc2;
		sc2.add(1);
		sc2.add(2);
		sc2.add(3);
		bool equality = (sc1 == sc2);
		ASSERT(equality );
	}

	// test op=
	{
		SimpleCollection<int> sc2 = sc1;
		bool equality = sc1 == sc2;
		ASSERT(equality );
	}

	// test op+
	{
		SimpleCollection<int> sc4;
		SimpleCollection<int> sc2 = sc1;
		SimpleCollection<int> sc3 = sc1 + sc1;

		{

			bool equality = sc1 == sc2;
			ASSERT(equality );
		}
		{

			bool equality = sc3 == sc4;
			ASSERT(!equality );
		}
	}
	// test op+=
	{
		SimpleCollection<int> sc2 = sc1;
		SimpleCollection<int> sc2copy = sc2;
		sc2 += sc1;
		//std::cout << "SimpleCollectionTest::testOperators: " << "sc1: " << sc1 << std::endl;
		//std::cout << "SimpleCollectionTest::testOperators: " << "sc2: " << sc2 << std::endl;

		bool equality = (sc2copy == sc2);
		ASSERT(!equality );
	}

}

void SimpleCollectionTest::testPad() {
	SimpleCollection<int> sc2;
	sc2.add(1);
	sc2.add(2);
	sc2.add(3);

	sc2.pad(2, 3, 5);

	ASSERT_EQUAL(8, sc2.getSize());
	int begin = *(sc2.getCollection().begin());
	int end = *(sc2.getCollection().rbegin());
	ASSERT_EQUAL(5, begin);
	ASSERT_EQUAL(5, end);

}

void SimpleCollectionTest::testAddRemoveList() {
	SimpleCollection<int> sc;

	// add by initialiser list and list
	{
		sc.add( { 1, 2, 3 });
		ASSERT_EQUAL(3, sc.getSize());
		sc.add(std::list<int>( { 4, 5, 6 }));
		ASSERT_EQUAL(6, sc.getSize());
	}

	// remove by initiliser list and list
	{
		sc.remove( { 4, 5, 6 });
		ASSERT_EQUAL(3, sc.getSize());
		sc.remove(std::list<int>( { 1, 2, 3 }));
		ASSERT_EQUAL(0, sc.getSize());
	}

}
} //NAMESPACE
}//NAMESPACE
