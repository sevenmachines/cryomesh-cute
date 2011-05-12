/*
 * BundleTest.h
 *
 *  Created on: 15 Feb 2011
 *      Author: niall
 */

#ifndef BUNDLETEST_H_
#define BUNDLETEST_H_

#include "ICuteSuite.h"
#include "state/PatternChannelMap.h"
#include "structures/Bundle.h"
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
	static void testCheckStructure();
	static void testPatternChannelsSetup();
	static void testAutoConnection();
	static void testGetDisconnectedChannels();

private:
	static bool checkChannelsMapDepth(const  cryomesh::state::PatternChannelMap & map, const int depth);
	static bool checkFibreMapsContain(const std::vector<boost::shared_ptr< Fibre > > & check_fibres, const std::map<boost::uuids::uuid, boost::uuids::uuid> & map);
	static bool checkPatternChannelMapsContain(const std::vector<boost::shared_ptr< state::PatternChannel > > & check_channels, const state::PatternChannelMap & map);
};

}

}

#endif /* BUNDLETEST_H_ */
