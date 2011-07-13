/*
 * PatternChannelMapTest.h
 *
 *  Created on: 13 Jul 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef PATTERNCHANNELMAPTEST_H_
#define PATTERNCHANNELMAPTEST_H_

namespace cryomesh {

namespace state {

class PatternChannelMapTest {
public:
	PatternChannelMapTest(){}
	virtual ~PatternChannelMapTest(){}
	static void runSuite();
	static void testGetPatterns();
};

}

}

#endif /* PATTERNCHANNELMAPTEST_H_ */
