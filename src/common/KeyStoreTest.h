/*
 * KeyStoreTest.h
 *
 *  Created on: 13 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef KEYSTORETEST_H_
#define KEYSTORETEST_H_

#include "ICuteSuite.h"

namespace cryomesh {

namespace common {

class KeyStoreTest :ICuteSuite {
public:
	KeyStoreTest(){}
	virtual ~KeyStoreTest(){}

	static void runSuite();
	static void testCreate();
	static void testAddRemove();
};

}

}

#endif /* KEYSTORETEST_H_ */
