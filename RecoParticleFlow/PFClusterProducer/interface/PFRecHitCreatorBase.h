#ifndef RecoParticleFlow_PFClusterProducer_PFRecHitCreatorBase_h
#define RecoParticleFlow_PFClusterProducer_PFRecHitCreatorBase_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/ParticleFlowReco/interface/PFLayer.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"

#include "RecoParticleFlow/PFClusterProducer/interface/PFRecHitQTestBase.h"
#include <memory>

class PFRecHitCreatorBase {
public:
  PFRecHitCreatorBase() {}
  PFRecHitCreatorBase(const edm::ParameterSet& iConfig, edm::ConsumesCollector& cc) {
    std::vector<edm::ParameterSet> qTests = iConfig.getParameter<std::vector<edm::ParameterSet> >("qualityTests");
    for (auto& qTest : qTests) {
      std::string name = qTest.getParameter<std::string>("name");
      qualityTests_.emplace_back(PFRecHitQTestFactory::get()->create(name, qTest, cc));
    }
  }
  virtual ~PFRecHitCreatorBase() = default;

  virtual void init(const edm::EventSetup& es) {}

  virtual void importRecHits(std::unique_ptr<reco::PFRecHitCollection>&,
                             std::unique_ptr<reco::PFRecHitCollection>&,
                             const edm::Event&,
                             const edm::EventSetup&) = 0;

protected:
  void beginEvent(const edm::Event& event, const edm::EventSetup& setup) {
    for (auto& qualityTest : qualityTests_)
      qualityTest->beginEvent(event, setup);
  }

  std::vector<std::unique_ptr<PFRecHitQTestBase> > qualityTests_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"
typedef edmplugin::PluginFactory<PFRecHitCreatorBase*(const edm::ParameterSet&, edm::ConsumesCollector&)>
    PFRecHitFactory;
#endif
