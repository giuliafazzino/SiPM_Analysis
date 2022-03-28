#include "readTreeThreshold.h"
#include "readTreeFrequency.h"

void makeThresholdHistos(){
    std::array<int,3> p_vols = {960, 980, 1000};
    std::array<int,2> b_dacs = {2678, 2783};
    std::vector<TString> iFileNames;
    
    iFileNames.push_back("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_threshold_scan_chip0-C1.threshold_scan.dat.tree.root");
    iFileNames.push_back("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_threshold_scan_chip0-C2.threshold_scan.dat.tree.root");
    iFileNames.push_back("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_threshold_scan_chip0-C3.threshold_scan.dat.tree.root");
    iFileNames.push_back("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_threshold_scan_chip0-C4.threshold_scan.dat.tree.root");
    iFileNames.push_back("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_threshold_scan_chip0-E1.threshold_scan.dat.tree.root");
    iFileNames.push_back("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_threshold_scan_chip0-E2.threshold_scan.dat.tree.root");
    iFileNames.push_back("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_threshold_scan_chip0-E3.threshold_scan.dat.tree.root");
    iFileNames.push_back("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_threshold_scan_chip0-E4.threshold_scan.dat.tree.root");
    iFileNames.push_back("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_threshold_scan_chip0-G1.threshold_scan.dat.tree.root");
    iFileNames.push_back("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_threshold_scan_chip0-G2.threshold_scan.dat.tree.root");
    iFileNames.push_back("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_threshold_scan_chip0-G3.threshold_scan.dat.tree.root");
    iFileNames.push_back("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_threshold_scan_chip0-G4.threshold_scan.dat.tree.root");
    
    for (auto kCurrentName : iFileNames){
        for (int p_vol : p_vols){
            for (int b_dac : b_dacs){
                readTreeThreshold(p_vol, b_dac, kCurrentName);
            }
        }
    }
}

void makeFrequencyHistos(){
    std::array<int,3> p_vols = {960, 980, 1000};
    std::array<int,2> b_dacs = {2678, 2783};
    std::vector<std::pair<std::array<int, 2>, TString>> iFileNames;
    
    iFileNames.push_back({{12, 14} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/A1_stab/rate_frequency_scan_chip1-A1_01_03.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{12, 14} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/A1_stab/rate_frequency_scan_chip1-A1_25_02.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{12, 14} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/A1_stab/rate_frequency_scan_chip1-A1_26_02.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{12, 14} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/A1_stab/rate_frequency_scan_chip1-A1_27_02.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{10, 12} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_frequency_scan_chip0-C1.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{12, 14} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_frequency_scan_chip0-C2.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{10, 12} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_frequency_scan_chip0-C3.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{ 9, 11} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_frequency_scan_chip0-C4.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{14, 16} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_frequency_scan_chip0-E1.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{12, 14} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_frequency_scan_chip0-E2.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{11, 13} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_frequency_scan_chip0-E3.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{ 9, 11} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_frequency_scan_chip0-E4.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{11, 13} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_frequency_scan_chip0-G1.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{12, 14} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_frequency_scan_chip0-G2.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{ 9, 11} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_frequency_scan_chip0-G3.frequency_scan.dat.tree.root"});
    iFileNames.push_back({{13, 15} , "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/28022022/rate_frequency_scan_chip0-G4.frequency_scan.dat.tree.root"});
    
    for (auto kCurrentFile : iFileNames){
        for (int p_vol : p_vols){
            for (int b_dac : b_dacs){
                readTreeFrequency(p_vol, b_dac, kCurrentFile.first[0], kCurrentFile.second);
                readTreeFrequency(p_vol, b_dac, kCurrentFile.first[1], kCurrentFile.second);
            }
        }
    }

}
