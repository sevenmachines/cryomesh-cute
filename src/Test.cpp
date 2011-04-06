#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

//manager
#include "manager/DatabaseManagerTest.h"
#include "manager/DatabaseObjectsTest.h"
#include "manager/CreatorTest.h"

//state
#include "state/BinaryStringTest.h"
#include "state/PatternChannelTest.h"
#include "state/PatternTest.h"
#include "state/SequenceTest.h"

// common
#include "common/ConnectorTest.h"
#include "common/TimeKeeperTest.h"
#include "common/CycleTest.h"

//components
#include "components/NodeTest.h"
#include "components/ConnectionTest.h"
#include "components/ImpulseCollectionTest.h"
#include "components/ImpulseTest.h"
#include "UseCasesTest.h"
#include "components/ActivityTimerTest.h"

// dataobjects
#include "dataobjects/DataObjectTest.h"

//structures
#include "structures/ClusterTest.h"
#include "structures/BundleTest.h"
#include "structures/FibreTest.h"

//utilities
#include "utilities/SequencerChannelsTest.h"
#include "utilities/SequencerGenericTest.h"

using namespace cryomesh;

void runUtilitiesSuite() {
	utilities::SequencerGenericTest::runSuite();
	utilities::SequencerChannelsTest::runSuite();
}
void runManagerSuite() {
	manager::DatabaseObjectsTest::runSuite();
	manager::DatabaseManagerTest::runSuite();
	manager::CreatorTest::runSuite();
}

void runGeneralSuite() {
	UseCasesTest::runSuite();
}

void runStateSuite() {
	state::BinaryStringTest::runSuite();
	state::PatternChannelTest::runSuite();
	state::PatternTest::runSuite();
	state::SequenceTest::runSuite();
}
void runCommonSuite() {
	common::ConnectorTest::runSuite();
	common::TimeKeeperTest::runSuite();
	//	common::SimpleCollectionTest::runSuite();
	//	common::MiscTest::runSuite();
	//	common::MathsTest::runSuite();
	//	common::ContainersTest::runSuite();
	//	common::KeyStoreTest::runSuite();
	common::CycleTest::runSuite();
}

void runComponentsSuite() {
	components::ImpulseCollectionTest::runSuite();
	components::ImpulseTest::runSuite();
	components::ConnectionTest::runSuite();
	components::ActivityTimerTest::runSuite();
	components::NodeTest::runSuite();
}

void runDataObjectsSuite() {
	dataobjects::DataObjectTest::runSuite();
}

void runStructuresSuite() {
	structures::ClusterTest::runSuite();
	structures::BundleTest::runSuite();
	structures::FibreTest::runSuite();
}

int main() {
	/**
	 * DISABLED
	 runStateSuite();
	 runCommonSuite();
	 runDataObjectsSuite();
	 runUtilitiesSuite();
	 runComponentsSuite();
	 runStructuresSuite();
	 runGeneralSuite();
	 */
	runManagerSuite();
	return 0;
}

