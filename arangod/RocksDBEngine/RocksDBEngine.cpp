#include "Basics/Exceptions.h"
#include "Basics/FileUtils.h"
#include "Basics/tri-strings.h"
#include "Logger/Logger.h"
#include "ProgramOptions/ProgramOptions.h"
#include "ProgramOptions/Section.h"
#include "RestServer/DatabasePathFeature.h"
#include "RocksDBEngine.h"
#include <Basics/Result.h>
#include <stdexcept>
#include <rocksdb/db.h>
#include <rocksdb/convenience.h>
#include <rocksdb/env.h>
#include <rocksdb/filter_policy.h>
#include <rocksdb/iterator.h>
#include <rocksdb/options.h>
#include <rocksdb/slice_transform.h>
#include <rocksdb/table.h>
#include <rocksdb/write_batch.h>

using namespace arangodb;
using namespace arangodb::application_features;
using namespace arangodb::options;

namespace arangodb {

std::string const RocksDBEngine::EngineName("rocksdb");
std::string const RocksDBEngine::FeatureName("RocksDBEngine");

// create the storage engine
RocksDBEngine::RocksDBEngine(application_features::ApplicationServer* server)
    : StorageEngine(server, EngineName, FeatureName, nullptr /*new
                                                              MMFilesIndexFactory()*/) {
      //inherits order from StorageEngine
}

RocksDBEngine::~RocksDBEngine() {
}

// inherited from ApplicationFeature
// ---------------------------------

// add the storage engine's specifc options to the global list of options
void RocksDBEngine::collectOptions(std::shared_ptr<options::ProgramOptions>) {

}

// validate the storage engine's specific options
void RocksDBEngine::validateOptions(std::shared_ptr<options::ProgramOptions>) {

}

void RocksDBEngine::start() {
  //it is already decided that rocksdb is used
  if (!isEnabled()) {
    return;
  }

  // set the database sub-directory for RocksDB
  auto database = ApplicationServer::getFeature<DatabasePathFeature>("DatabasePath");
  _path = database->subdirectoryName("engine-rocksdb");

  LOG_TOPIC(TRACE, arangodb::Logger::FIXME) << "initializing rocksdb, path: " << _path;

  rocksdb::TransactionDBOptions transactionOptions;

  _options.create_if_missing = true;
  _options.max_open_files = -1;

  rocksdb::Status status = rocksdb::TransactionDB::Open(_options, transactionOptions, _path, &_db);

  if (! status.ok()) {
    LOG_TOPIC(FATAL, arangodb::Logger::FIXME) << "unable to initialize RocksDB engine: " << status.ToString();
    FATAL_ERROR_EXIT();
  }
}
void RocksDBEngine::stop() { throw std::runtime_error("not implemented"); }
// preparation phase for storage engine. can be used for internal setup.
// the storage engine must not start any threads here or write any files
void RocksDBEngine::prepare() {
}

transaction::ContextData* RocksDBEngine::createTransactionContextData() {
  throw std::runtime_error("not implemented");
  return nullptr;
}
TransactionState* RocksDBEngine::createTransactionState(TRI_vocbase_t*) {
  throw std::runtime_error("not implemented");
  return nullptr;
}
TransactionCollection* RocksDBEngine::createTransactionCollection(
    TransactionState* state, TRI_voc_cid_t cid, AccessMode::Type accessType,
    int nestingLevel) {
  throw std::runtime_error("not implemented");
  return nullptr;
}

// create storage-engine specific collection
PhysicalCollection* RocksDBEngine::createPhysicalCollection(LogicalCollection*,
                                                            VPackSlice const&) {
  throw std::runtime_error("not implemented");
  return nullptr;
}

// create storage-engine specific view
PhysicalView* RocksDBEngine::createPhysicalView(LogicalView*,
                                                VPackSlice const&) {
  throw std::runtime_error("not implemented");
  return nullptr;
}

// inventory functionality
// -----------------------

void RocksDBEngine::getDatabases(arangodb::velocypack::Builder& result) {
  throw std::runtime_error("not implemented");
}
void RocksDBEngine::getCollectionInfo(TRI_vocbase_t* vocbase, TRI_voc_cid_t cid,
                                      arangodb::velocypack::Builder& result,
                                      bool includeIndexes,
                                      TRI_voc_tick_t maxTick) {
  throw std::runtime_error("not implemented");
}
int RocksDBEngine::getCollectionsAndIndexes(
    TRI_vocbase_t* vocbase, arangodb::velocypack::Builder& result,
    bool wasCleanShutdown, bool isUpgrade) {
  throw std::runtime_error("not implemented");
  return 0;
}

int RocksDBEngine::getViews(TRI_vocbase_t* vocbase,
                            arangodb::velocypack::Builder& result) {
  throw std::runtime_error("not implemented");
  return 0;
}

std::string RocksDBEngine::databasePath(TRI_vocbase_t const* vocbase) const {
  throw std::runtime_error("not implemented");
  return "not implemented";
}
std::string RocksDBEngine::collectionPath(TRI_vocbase_t const* vocbase,
                                          TRI_voc_cid_t id) const {
  throw std::runtime_error("not implemented");
  return "not implemented";
}

// database, collection and index management
// -----------------------------------------

// return the path for a database

void RocksDBEngine::waitForSync(TRI_voc_tick_t tick) {
  throw std::runtime_error("not implemented");
}

TRI_vocbase_t* RocksDBEngine::openDatabase(
    arangodb::velocypack::Slice const& parameters, bool isUpgrade, int&) {
  throw std::runtime_error("not implemented");
  return nullptr;
}
RocksDBEngine::Database* RocksDBEngine::createDatabase(
    TRI_voc_tick_t id, arangodb::velocypack::Slice const& args, int& status) {
  throw std::runtime_error("not implemented");
  return nullptr;
}
int RocksDBEngine::writeCreateMarker(TRI_voc_tick_t id,
                                     VPackSlice const& slice) {
  throw std::runtime_error("not implemented");
  return 0;
}
void RocksDBEngine::prepareDropDatabase(TRI_vocbase_t* vocbase,
                                        bool useWriteMarker, int& status) {
  throw std::runtime_error("not implemented");
}
void RocksDBEngine::dropDatabase(Database* database, int& status) {
  throw std::runtime_error("not implemented");
}
void RocksDBEngine::waitUntilDeletion(TRI_voc_tick_t id, bool force,
                                      int& status) {
  throw std::runtime_error("not implemented");
}

// wal in recovery
bool RocksDBEngine::inRecovery() {
  throw std::runtime_error("not implemented");
  return true;
}

// start compactor thread and delete files form collections marked as deleted
void RocksDBEngine::recoveryDone(TRI_vocbase_t* vocbase) {
  throw std::runtime_error("not implemented");
}

std::string RocksDBEngine::createCollection(
    TRI_vocbase_t* vocbase, TRI_voc_cid_t id,
    arangodb::LogicalCollection const*) {
  throw std::runtime_error("not implemented");
  return "not implemented";
}

arangodb::Result RocksDBEngine::persistCollection(
    TRI_vocbase_t* vocbase, arangodb::LogicalCollection const*) {
  throw std::runtime_error("not implemented");
  return arangodb::Result{};
}
arangodb::Result RocksDBEngine::dropCollection(TRI_vocbase_t* vocbase,
                                               arangodb::LogicalCollection*) {
  throw std::runtime_error("not implemented");
  return arangodb::Result{};
}
void RocksDBEngine::destroyCollection(TRI_vocbase_t* vocbase,
                                      arangodb::LogicalCollection*) {
  throw std::runtime_error("not implemented");
}
void RocksDBEngine::changeCollection(TRI_vocbase_t* vocbase, TRI_voc_cid_t id,
                                     arangodb::LogicalCollection const*,
                                     bool doSync) {
  throw std::runtime_error("not implemented");
}
arangodb::Result RocksDBEngine::renameCollection(
    TRI_vocbase_t* vocbase, arangodb::LogicalCollection const*,
    std::string const& oldName) {
  throw std::runtime_error("not implemented");
  return arangodb::Result{};
}
void RocksDBEngine::createIndex(TRI_vocbase_t* vocbase,
                                TRI_voc_cid_t collectionId, TRI_idx_iid_t id,
                                arangodb::velocypack::Slice const& data) {
  throw std::runtime_error("not implemented");
}
void RocksDBEngine::dropIndex(TRI_vocbase_t* vocbase,
                              TRI_voc_cid_t collectionId, TRI_idx_iid_t id) {
  throw std::runtime_error("not implemented");
}
void RocksDBEngine::dropIndexWalMarker(TRI_vocbase_t* vocbase,
                                       TRI_voc_cid_t collectionId,
                                       arangodb::velocypack::Slice const& data,
                                       bool writeMarker, int&) {
  throw std::runtime_error("not implemented");
}
void RocksDBEngine::unloadCollection(TRI_vocbase_t* vocbase,
                                     arangodb::LogicalCollection* collection) {
  throw std::runtime_error("not implemented");
}
void RocksDBEngine::createView(TRI_vocbase_t* vocbase, TRI_voc_cid_t id,
                               arangodb::LogicalView const*) {
  throw std::runtime_error("not implemented");
}

arangodb::Result RocksDBEngine::persistView(TRI_vocbase_t* vocbase,
                                            arangodb::LogicalView const*) {
  throw std::runtime_error("not implemented");
  return arangodb::Result{};
}

arangodb::Result RocksDBEngine::dropView(TRI_vocbase_t* vocbase,
                                         arangodb::LogicalView*) {
  throw std::runtime_error("not implemented");
  return arangodb::Result{};
}
void RocksDBEngine::destroyView(TRI_vocbase_t* vocbase,
                                arangodb::LogicalView*) {
  throw std::runtime_error("not implemented");
}
void RocksDBEngine::changeView(TRI_vocbase_t* vocbase, TRI_voc_cid_t id,
                               arangodb::LogicalView const*, bool doSync) {
  throw std::runtime_error("not implemented");
}
std::string RocksDBEngine::createViewDirectoryName(std::string const& basePath,
                                                   TRI_voc_cid_t id) {
  throw std::runtime_error("not implemented");
  return "not implemented";
}
void RocksDBEngine::signalCleanup(TRI_vocbase_t* vocbase) {
  throw std::runtime_error("not implemented");
}

// document operations
// -------------------
void RocksDBEngine::iterateDocuments(
    TRI_voc_tick_t databaseId, TRI_voc_cid_t collectionId,
    std::function<void(arangodb::velocypack::Slice const&)> const& cb) {
  throw std::runtime_error("not implemented");
}
void RocksDBEngine::addDocumentRevision(
    TRI_voc_tick_t databaseId, TRI_voc_cid_t collectionId,
    arangodb::velocypack::Slice const& document) {
  throw std::runtime_error("not implemented");
}
void RocksDBEngine::removeDocumentRevision(
    TRI_voc_tick_t databaseId, TRI_voc_cid_t collectionId,
    arangodb::velocypack::Slice const& document) {
  throw std::runtime_error("not implemented");
}

/// @brief remove data of expired compaction blockers
bool RocksDBEngine::cleanupCompactionBlockers(TRI_vocbase_t* vocbase) {
  throw std::runtime_error("not implemented");
  return true;
}

/// @brief insert a compaction blocker
int RocksDBEngine::insertCompactionBlocker(TRI_vocbase_t* vocbase, double ttl,
                                           TRI_voc_tick_t& id) {
  throw std::runtime_error("not implemented");
  return true;
}

/// @brief touch an existing compaction blocker
int RocksDBEngine::extendCompactionBlocker(TRI_vocbase_t* vocbase,
                                           TRI_voc_tick_t id, double ttl) {
  throw std::runtime_error("not implemented");
  return true;
}

/// @brief remove an existing compaction blocker
int RocksDBEngine::removeCompactionBlocker(TRI_vocbase_t* vocbase,
                                           TRI_voc_tick_t id) {
  throw std::runtime_error("not implemented");
  return true;
}

/// @brief a callback function that is run while it is guaranteed that there
/// is no compaction ongoing
void RocksDBEngine::preventCompaction(
    TRI_vocbase_t* vocbase,
    std::function<void(TRI_vocbase_t*)> const& callback) {
  throw std::runtime_error("not implemented");
}

/// @brief a callback function that is run there is no compaction ongoing
bool RocksDBEngine::tryPreventCompaction(
    TRI_vocbase_t* vocbase, std::function<void(TRI_vocbase_t*)> const& callback,
    bool checkForActiveBlockers) {
  throw std::runtime_error("not implemented");
  return true;
}

int RocksDBEngine::shutdownDatabase(TRI_vocbase_t* vocbase) {
  throw std::runtime_error("not implemented");
  return 0;
}

int RocksDBEngine::openCollection(TRI_vocbase_t* vocbase,
                                  LogicalCollection* collection,
                                  bool ignoreErrors) {
  throw std::runtime_error("not implemented");
  return 0;
}

/// @brief Add engine-specific AQL functions.
void RocksDBEngine::addAqlFunctions() {
  throw std::runtime_error("not implemented");
}

/// @brief Add engine-specific optimizer rules
void RocksDBEngine::addOptimizerRules() {
  throw std::runtime_error("not implemented");
}

/// @brief Add engine-specific V8 functions
void RocksDBEngine::addV8Functions() {
  throw std::runtime_error("not implemented");
}

/// @brief Add engine-specific REST handlers
void RocksDBEngine::addRestHandlers(rest::RestHandlerFactory*) {
  throw std::runtime_error("not implemented");
}
}
