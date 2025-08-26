// k4FWCore
#include "k4FWCore/Consumer.h"

// edm4hep
#include "edm4hep/SimCalorimeterHitCollection.h"

// STL
#include <string>

struct MoliereRadius final
    : k4FWCore::Consumer<void(const edm4hep::SimCalorimeterHitCollection&)> {

public:
    // Constructor
    MoliereRadius(const std::string& name, ISvcLocator* svcLoc)
        : Consumer(name, 
                   svcLoc, 
                   KeyValues("InputCaloHitCollection", {"simplecaloRO"})) {}

    
    void operator()(const edm4hep::SimCalorimeterHitCollection& input) const override {

        // Calcualte the Moliere radius from the simhits
        double moliere_radius = 0.0;

        for (const auto& simhit: input) {

        }

        info() << "Calculated Moliere radius: " << moliere_radius << endmsg;
        
    }
};

DECLARE_COMPONENT(MoliereRadius)