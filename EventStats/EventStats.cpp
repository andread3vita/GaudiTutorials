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

// ROOT
#include "TFile.h"
#include "TH1D.h"


struct EventStats final
    : k4FWCore::MultiTransformer<   std::tuple< std::vector< podio::UserDataCollection<double>>, 
                                                podio::UserDataCollection<double>, 
                                                podio::UserDataCollection<double>, 
                                                podio::UserDataCollection<double>, 
                                                podio::UserDataCollection<double>, 
                                                podio::UserDataCollection<double>   > (const edm4hep::SimCalorimeterHitCollection&) > {

public:

    EventStats(const std::string& name, ISvcLocator* svcLoc)
        : MultiTransformer(name, svcLoc,     
                {

                    KeyValues("InputCaloHitCollection", {"simplecaloRO"})

                },
                {   

                    KeyValues("EnergyStats", {"EnergyStats"}),
                    KeyValues("EnergyBarycentre", {"EnergyBarycentre"}),
                    KeyValues("MaxEnergyPosition", {"MaxEnergyPos"}),
                    KeyValues("MinEnergyPosition", {"MinEnergyPos"}),
                    KeyValues("MaxXYZvalues", {"MaxPos"}),
                    KeyValues("MinXYZvalues", {"MinPos"})

                }) {}

    StatusCode initialize() override {

        if (m_plotHisto.value()) {

            hTotalEnergy = new TH1D("hTotalEnergy", "Total Deposited Energy, in GeV", 50, 0, 10);
            hMaxEnergy = new TH1D("hMaxEnergy", "Max Deposited Energy, in GeV", 50, 0, 10);
            hMinEnergy = new TH1D("hMinEnergy", "Min Deposited Energy, in GeV", 50, 0, 10);

        }


        return StatusCode::SUCCESS;

    }

    std::tuple< std::vector<podio::UserDataCollection<double>>, 
                podio::UserDataCollection<double>, 
                podio::UserDataCollection<double>, 
                podio::UserDataCollection<double>,
                podio::UserDataCollection<double>,
                podio::UserDataCollection<double> > operator()(const edm4hep::SimCalorimeterHitCollection& input) const override {

        std::vector<podio::UserDataCollection<double>> EnergyStats;
        
        auto totalDepositedEnergy = podio::UserDataCollection<double>();
        auto maxEnergyCollection = podio::UserDataCollection<double>();
        auto minEnergyCollection = podio::UserDataCollection<double>();

        auto barycentre = podio::UserDataCollection<double>();

        auto maxEnergyPosition = podio::UserDataCollection<double>();

        auto minEnergyPosition = podio::UserDataCollection<double>();

        auto maxPosition = podio::UserDataCollection<double>();

        auto minPosition = podio::UserDataCollection<double>();

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

        if (m_plotHisto) {
            hTotalEnergy->Fill(totalEnergy);
            hMaxEnergy->Fill(maxEnergy);
            hMinEnergy->Fill(minEnergy);
        }

        barycentre_x /= totalEnergy;
        barycentre_y /= totalEnergy;
        barycentre_z /= totalEnergy;    

        totalDepositedEnergy.push_back(totalEnergy);
        maxEnergyCollection.push_back(maxEnergy);
        minEnergyCollection.push_back(minEnergy);
        EnergyStats.push_back(std::move(totalDepositedEnergy));
        EnergyStats.push_back(std::move(maxEnergyCollection));
        EnergyStats.push_back(std::move(minEnergyCollection));

        barycentre.push_back(barycentre_x);
        barycentre.push_back(barycentre_y);
        barycentre.push_back(barycentre_z);

        maxEnergyPosition.push_back(maxEnergy_x);
        maxEnergyPosition.push_back(maxEnergy_y);
        maxEnergyPosition.push_back(maxEnergy_z);

        minEnergyPosition.push_back(minEnergy_x);
        minEnergyPosition.push_back(minEnergy_y);
        minEnergyPosition.push_back(minEnergy_z);

        maxPosition.push_back(max_pos_x);
        maxPosition.push_back(max_pos_y);
        maxPosition.push_back(max_pos_z);

        minPosition.push_back(min_pos_x);
        minPosition.push_back(min_pos_y);
        minPosition.push_back(min_pos_z);


        // Print
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
        info() << "MaxXYZ values: (" << max_pos_x << ", " << max_pos_y << ", " << max_pos_z << ") mm" << endmsg;
        info() << "MinXYZ values: (" << min_pos_x << ", " << min_pos_y << ", " << min_pos_z << ") mm" << endmsg;
        info() << "------------------------------" << endmsg;
      
        
        return std::make_tuple(std::move(EnergyStats), 
                               std::move(barycentre), 
                               std::move(maxEnergyPosition), 
                               std::move(minEnergyPosition), 
                               std::move(maxPosition), 
                               std::move(minPosition));
        
    }

    StatusCode finalize() override {

        if (m_plotHisto.value()) {

            TFile *f = new TFile("debugEventStats.root", "RECREATE");
            hTotalEnergy->Write();
            hMaxEnergy->Write();
            hMinEnergy->Write();
            f->Close();

            delete hTotalEnergy;
            delete hMaxEnergy;
            delete hMinEnergy;
            delete f;
        }

        return StatusCode::SUCCESS;
    }
    private:
    
        Gaudi::Property<bool> m_plotHisto{this, "plotHistograms", false, "flag to plot histograms"};

        TH1D* hTotalEnergy;
        TH1D* hMaxEnergy;
        TH1D* hMinEnergy;

};

DECLARE_COMPONENT(EventStats)
