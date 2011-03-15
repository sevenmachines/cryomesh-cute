/*
 * DataObjectTest.cpp
 *
 *  Created on: 3 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "DataObjectTest.h"
#include "dataobjects/DataObject.h"

namespace cryomesh {

namespace dataobjects {

void DataObjectTest::runSuite() {
	cute::suite s;
	s.push_back(CUTE(DataObjectTest::testDataObject));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "DataObjectTest");
}

void DataObjectTest::testDataObject() {
	// create size
	const int MAX_SIZE = 10;
	DataObject<int, int> dataobj(MAX_SIZE);
	// test add
	{
		dataobj.insert(1, 11);
		dataobj.insert(2, 22);
		dataobj.insert(3, 33);
		dataobj.insert(4, 44);
		ASSERT_EQUAL(4, dataobj.getMap().size());

	}

	// test comparisons
	{
		ASSERT_EQUAL(44, dataobj.getMaximumValue());
		ASSERT_EQUAL(11, dataobj.getMinimumValue());
		ASSERT_EQUAL_DELTA(27, dataobj.getAverageValue(), 2);
	}

	// test get by key
	{
		ASSERT_EQUAL(33, dataobj.getByKey(3));
	}

	// test get range
	{
		std::map<int, int> get1 = dataobj.getMap(2, 4);
		ASSERT_EQUAL(2, get1.size());
		ASSERT_EQUAL(22, dataobj.getByKey(2));
		ASSERT_EQUAL(33, dataobj.getByKey(3));
	}

	// test get from range
	{
		std::map<int, int> get2 = dataobj.getMap(3);
		ASSERT_EQUAL(2, get2.size());
		ASSERT_EQUAL(33, dataobj.getByKey(3));
		ASSERT_EQUAL(44, dataobj.getByKey(4));
	}

	// add beyond max
	{
		for (int i = 0; i < MAX_SIZE + 2; i++) {
			dataobj.insert(i + 10, i);
		}
		ASSERT_EQUAL(MAX_SIZE, dataobj.getMap().size());
	}
}
}//NAMESPACE

}//NAMESPACE
