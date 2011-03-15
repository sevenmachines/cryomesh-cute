#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"

//spacial
#include "spacial/SphericalPointTest.h"
#include "spacial/PointTest.h"

//state
#include "state/BinaryStringTest.h"
#include "state/PatternChannelTest.h"
#include "state/PatternTest.h"
#include "state/SequenceTest.h"

// common
#include "common/ConnectorTest.h"
#include "common/TimeKeeperTest.h"
#include "common/SimpleCollectionTest.h"
#include "common/MiscTest.h"
#include "common/MathsTest.h"
#include "common/ContainersTest.h"
#include "common/KeyStoreTest.h"

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

using namespace cryomesh;

void runGeneralSuite() {
	UseCasesTest::runSuite();
}

void runSpacialSuite() {
	spacial::SphericalPointTest::runSuite();
	spacial::PointTest::runSuite();
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
	common::SimpleCollectionTest::runSuite();
	common::MiscTest::runSuite();
	common::MathsTest::runSuite();
	common::ContainersTest::runSuite();
	common::KeyStoreTest::runSuite();
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
	structures::FibreTest::runSuite();
	structures::BundleTest::runSuite();
}

int main() {
	/**
	 * DISABLED

	 */
	runStateSuite();
	runCommonSuite();

	runStructuresSuite();
	runDataObjectsSuite();
	runSpacialSuite();
	//runGeneralSuite();
	runComponentsSuite();
	return 0;
}

