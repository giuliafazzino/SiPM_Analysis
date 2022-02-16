#include "AnalysisUtility.h"
void
TestMacro
( ) {
    std::vector<std::pair<float,float>> kTest = {{1,1},{1,1},{1,1},{1,1},{1,1}};
    auto result_test = uCalculateRate(kTest);
    cout << result_test.first << endl;
    cout << result_test.second << endl;
}
