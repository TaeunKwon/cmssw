#ifndef CSCDigitizer_CSCConfigurableStripConditions_h
#define CSCDigitizer_CSCConfigurableStripConditions_h

#include "SimMuon/CSCDigitizer/src/CSCStripConditions.h"
#include "SimGeneral/NoiseGenerators/interface/CorrelatedNoisifier.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class CSCConfigurableStripConditions : public CSCStripConditions
{
public:
  CSCConfigurableStripConditions(const edm::ParameterSet & p);
  virtual ~CSCConfigurableStripConditions();

  /// channels count from 1
  virtual float gain(const CSCDetId & detId, int channel) const;
  virtual float gainVariance(const CSCDetId & detId, int channel) const {return theGainVariance;}

  /// in ADC counts
  virtual float pedestal(const CSCDetId & detId, int channel) const {return thePedestal;}
  virtual float pedestalVariance(const CSCDetId & detId, int channel) const {return thePedestalVariance;}

  virtual void crosstalk(double stripLength, bool leftRight,
                 float & capacitive, float & resistive) const = 0;

private:
  virtual void fetchNoisifier(const CSCDetId & detId, int istrip);
  void makeNoisifier(int chamberType, const std::vector<double> & correlations);
  std::vector<CorrelatedNoisifier *> theNoisifiers;

  float theGain;
  float theME11Gain;
  float theGainVariance;
  float thePedestal;
  float thePedestalVariance;
  // proportional to slope of neighboring signal, per cm of strip length
  float theCapacitiveCrosstalk;
  // proportional to neighboring signal
  float theResistiveCrosstalk;
};

#endif


