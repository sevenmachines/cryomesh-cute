/*
 * CreatorTest.h
 *
 *  Created on: 30 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef CREATORTEST_H_
#define CREATORTEST_H_

#include "ICuteSuite.h"

namespace cryomesh {

namespace manager {

class CreatorTest :ICuteSuite {
public:
	CreatorTest();
	virtual ~CreatorTest();
	static void runSuite();
	static void testCreation();
	static void testCheckConfigEntry();
	static void testCheckConfigStructure();
	static void testAutoConnect();

	static const std::string CONFIG_FILE;
	static const std::string AUTOCONNECT_CONFIG_FILE;
	static const std::string NULL_CONFIG_FILE;
	static const std::string DATAFILE;

};

}

}

#endif /* CREATORTEST_H_ */
