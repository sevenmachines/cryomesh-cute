/*
 * BundleTest.h
 *
 *  Created on: 15 Feb 2011
 *      Author: niall
 */

#ifndef BUNDLETEST_H_
#define BUNDLETEST_H_

#include "ICuteSuite.h"

namespace cryomesh {

namespace structures {

class BundleTest {
public:
	BundleTest(){}
	virtual ~BundleTest(){}

	static void runSuite();

	static void testCreateConnect();
	static void testPropagation();
	static void testLoadData();
	static void testConnectPatternChannels();
};

}

}

#endif /* BUNDLETEST_H_ */
