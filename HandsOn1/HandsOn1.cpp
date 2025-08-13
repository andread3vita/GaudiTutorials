// k4FWCore
#include "k4FWCore/Transformer.h"

// edm4hep/podio
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "Gaudi/Property.h"
#include "podio/UserDataCollection.h"

// STL
#include <string>
#include <vector>
#include <cmath>


struct EventStats final
    : k4FWCore::MultiTransformer<   std::tuple< std::vector<podio::UserDataCollection<double>> > (const edm4hep::SimCalorimeterHitCollection&) > {

public:

    EventStats(const std::string& name, ISvcLocator* svcLoc)
        : MultiTransformer(name, svcLoc,     
                {

                    KeyValues("InputCaloHitCollection", {"simplecaloRO"})

                },
                {   

                    KeyValues("eventStats", {"eventStats"})

                }) {}

    std::tuple< std::vector<podio::UserDataCollection<double>> > operator()(const edm4hep::SimCalorimeterHitCollection& input) const override {

        std::vector<podio::UserDataCollection<double>> eventStats;
        auto totalDepositedEnergy = podio::UserDataCollection<double>();
        auto x_pos_barycentre = podio::UserDataCollection<double>();
        auto y_pos_barycentre = podio::UserDataCollection<double>();
        auto z_pos_barycentre = podio::UserDataCollection<double>();
        auto x_pos_max = podio::UserDataCollection<double>();
        auto y_pos_max = podio::UserDataCollection<double>();
        auto z_pos_max = podio::UserDataCollection<double>();
        auto x_pos_min = podio::UserDataCollection<double>();
        auto y_pos_min = podio::UserDataCollection<double>();
        auto z_pos_min = podio::UserDataCollection<double>();
        auto maxEnergy_layer = podio::UserDataCollection<double>();
        auto minEnergy_layer = podio::UserDataCollection<double>();
        auto maxEnergy_layerPos = podio::UserDataCollection<double>();
        auto minEnergy_layerPos = podio::UserDataCollection<double>();


        // fill userDataCollections with values

        eventStats.push_back(std::move(totalDepositedEnergy));
        eventStats.push_back(std::move(x_pos_barycentre));
        eventStats.push_back(std::move(y_pos_barycentre));
        eventStats.push_back(std::move(z_pos_barycentre));
        eventStats.push_back(std::move(x_pos_max));
        eventStats.push_back(std::move(y_pos_max));
        eventStats.push_back(std::move(z_pos_max));
        eventStats.push_back(std::move(x_pos_min));     
        eventStats.push_back(std::move(y_pos_min));
        eventStats.push_back(std::move(z_pos_min));
        eventStats.push_back(std::move(maxEnergy_layer));
        eventStats.push_back(std::move(minEnergy_layer));
        eventStats.push_back(std::move(maxEnergy_layerPos));
        eventStats.push_back(std::move(minEnergy_layerPos));

        return std::make_tuple(std::move(eventStats));
        
    }
};

DECLARE_COMPONENT(EventStats)
