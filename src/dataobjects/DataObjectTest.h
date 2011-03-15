/*
 * DataObjectTest.h
 *
 *  Created on: 3 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef DATAOBJECTTEST_H_
#define DATAOBJECTTEST_H_

#include "ICuteSuite.h"

namespace cryomesh {

namespace dataobjects {

class DataObjectTest :public ICuteSuite{
public:
	DataObjectTest(){}
	virtual ~DataObjectTest(){}
	static void runSuite();

	static void testDataObject();
};

}

}

#endif /* DATAOBJECTTEST_H_ */
