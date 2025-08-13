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

                    KeyValues("EventStats", {"eventStats"})

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
        auto maxEnergy_value = podio::UserDataCollection<double>();
        auto minEnergy_value = podio::UserDataCollection<double>();
        auto maxEnergy_layerPos_x = podio::UserDataCollection<double>();
        auto maxEnergy_layerPos_y = podio::UserDataCollection<double>();
        auto maxEnergy_layerPos_z = podio::UserDataCollection<double>();
        auto minEnergy_layerPos_x = podio::UserDataCollection<double>();
        auto minEnergy_layerPos_y = podio::UserDataCollection<double>();
        auto minEnergy_layerPos_z = podio::UserDataCollection<double>();

        double totalEnergy = 0.0;
        double barycentre_x = 0.0;
        double barycentre_y = 0.0;
        double barycentre_z = 0.0;

        double max_pos_x = std::numeric_limits<double>::lowest();
        double max_pos_y = std::numeric_limits<double>::lowest();
        double max_pos_z = std::numeric_limits<double>::lowest();
        double min_pos_x = std::numeric_limits<double>::max();
        double min_pos_y = std::numeric_limits<double>::max();
        double min_pos_z = std::numeric_limits<double>::max();

        double maxEnergy = std::numeric_limits<double>::lowest();
        double minEnergy = std::numeric_limits<double>::max();
        double maxEnergy_x = 0.0;
        double maxEnergy_y = 0.0;
        double maxEnergy_z = 0.0;
        double minEnergy_x = 0.0;
        double minEnergy_y = 0.0;
        double minEnergy_z = 0.0;
        

        for (const auto& simhit: input) {

            double x = simhit.getPosition().x;
            double y = simhit.getPosition().y;
            double z = simhit.getPosition().z;
            double energy = simhit.getEnergy();

            barycentre_x += x * energy;
            barycentre_y += y * energy;
            barycentre_y += y * energy;
            barycentre_z += z * energy;
            totalEnergy += energy;

            if (energy > maxEnergy) {
                maxEnergy = energy;
                maxEnergy_x = x;
                maxEnergy_y = y;
                maxEnergy_z = z;
            }
            if (energy < minEnergy) {
                minEnergy = energy;
                minEnergy_x = x;
                minEnergy_y = y;
                minEnergy_z = z;
            }
            if (x > max_pos_x) max_pos_x = x;
            if (y > max_pos_y) max_pos_y = y;
            if (z > max_pos_z) max_pos_z = z;
            if (x < min_pos_x) min_pos_x = x;
            if (y < min_pos_y) min_pos_y = y;
            if (z < min_pos_z) min_pos_z = z;
        }


        barycentre_x /= totalEnergy;
        barycentre_y /= totalEnergy;
        barycentre_z /= totalEnergy;    

        x_pos_barycentre.push_back(barycentre_x);
        y_pos_barycentre.push_back(barycentre_y);
        z_pos_barycentre.push_back(barycentre_z);
        totalDepositedEnergy.push_back(totalEnergy);
        x_pos_max.push_back(max_pos_x);
        y_pos_max.push_back(max_pos_y);
        z_pos_max.push_back(max_pos_z);
        x_pos_min.push_back(min_pos_x);
        y_pos_min.push_back(min_pos_y);
        z_pos_min.push_back(min_pos_z);
        maxEnergy_value.push_back(maxEnergy);
        minEnergy_value.push_back(minEnergy);
        maxEnergy_layerPos_x.push_back(maxEnergy_x);
        maxEnergy_layerPos_y.push_back(maxEnergy_y);
        maxEnergy_layerPos_z.push_back(maxEnergy_z);
        minEnergy_layerPos_x.push_back(minEnergy_x);
        minEnergy_layerPos_y.push_back(minEnergy_y);
        minEnergy_layerPos_z.push_back(minEnergy_z);

        // Print
        info() << "------------------------------" << endmsg;
        info() << "Event statistics:" << endmsg;
        info() << "Total deposited energy = " << totalEnergy << " GeV" << endmsg;
        info() << "Barycentre position: (" << barycentre_x << ", " << barycentre_y << ", " << barycentre_z << ") mm" << endmsg;
        info() << "Max energy: " << maxEnergy << " GeV at (" 
             << maxEnergy_x << ", " 
             << maxEnergy_y << ", " 
             << maxEnergy_z << ") mm" << endmsg;
        info() << "Min energy: " << minEnergy << " GeV at (" 
             << minEnergy_x << ", " 
             << minEnergy_y << ", " 
             << minEnergy_z << ") mm" << endmsg;
        info() << "Max position: (" << max_pos_x << ", " << max_pos_y << ", " << max_pos_z << ") mm" << endmsg;
        info() << "Min position: (" << min_pos_x << ", " << min_pos_y << ", " << min_pos_z << ") mm" << endmsg;
        info() << "------------------------------" << endmsg;
        
        if (m_plotHisto) {
            // Plot histograms if the flag is set: TO-DO

        }

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
        eventStats.push_back(std::move(maxEnergy_value));
        eventStats.push_back(std::move(minEnergy_value));
        eventStats.push_back(std::move(maxEnergy_layerPos_x));
        eventStats.push_back(std::move(maxEnergy_layerPos_y));
        eventStats.push_back(std::move(maxEnergy_layerPos_z));
        eventStats.push_back(std::move(minEnergy_layerPos_x));
        eventStats.push_back(std::move(minEnergy_layerPos_y));
        eventStats.push_back(std::move(minEnergy_layerPos_z));

        return std::make_tuple(std::move(eventStats));
        
    }

    private:
    
        Gaudi::Property<bool> m_plotHisto{this, "plotHistograms", false, "flag to plot histograms"};

};

DECLARE_COMPONENT(EventStats)
