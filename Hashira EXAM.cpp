#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <sstream>

using namespace std;

// Simple JSON parser for this specific format
map<string, string> parseJsonValue(const string& json, const string& key) {
    map<string, string> result;
    size_t keyPos = json.find("\"" + key + "\"");
    if (keyPos == string::npos) return result;
    
    size_t objStart = json.find("{", keyPos);
    size_t objEnd = json.find("}", objStart);
    string objStr = json.substr(objStart + 1, objEnd - objStart - 1);
    
    // Parse base and value
    size_t basePos = objStr.find("\"base\"");
    if (basePos != string::npos) {
        size_t baseValStart = objStr.find("\"", basePos + 6);
        size_t baseValEnd = objStr.find("\"", baseValStart + 1);
        result["base"] = objStr.substr(baseValStart + 1, baseValEnd - baseValStart - 1);
    }
    
    size_t valuePos = objStr.find("\"value\"");
    if (valuePos != string::npos) {
        size_t valueValStart = objStr.find("\"", valuePos + 7);
        size_t valueValEnd = objStr.find("\"", valueValStart + 1);
        result["value"] = objStr.substr(valueValStart + 1, valueValEnd - valueValStart - 1);
    }
    
    return result;
}

int parseK(const string& json) {
    size_t keysPos = json.find("\"keys\"");
    size_t kPos = json.find("\"k\"", keysPos);
    size_t colonPos = json.find(":", kPos);
    size_t numStart = colonPos + 1;
    while (json[numStart] == ' ' || json[numStart] == '\t') numStart++;
    size_t numEnd = numStart;
    while (isdigit(json[numEnd])) numEnd++;
    
    return stoi(json.substr(numStart, numEnd - numStart));
}

// Function to convert number from any base to decimal
long long convertBaseToDecimal(const string& value, int base) {
    long long result = 0;
    long long power = 1;
    
    for (int i = value.length() - 1; i >= 0; i--) {
        int digitValue;
        if (isdigit(value[i])) {
            digitValue = value[i] - '0';
        } else {
            digitValue = tolower(value[i]) - 'a' + 10;
        }
        result += digitValue * power;
        power *= base;
    }
    return result;
}

// Lagrange interpolation to find value at x_target
double lagrangeInterpolation(const vector<pair<int, long long>>& points, int x_target) {
    double result = 0.0;
    int n = points.size();
    
    for (int i = 0; i < n; i++) {
        int xi = points[i].first;
        long long yi = points[i].second;
        
        // Calculate Lagrange basis polynomial Li(x_target)
        double li = 1.0;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                int xj = points[j].first;
                li *= (double)(x_target - xj) / (xi - xj);
            }
        }
        result += yi * li;
    }
    return result;
}

long long solveShamirSecretSharing(const string& jsonInput) {
    cout << "Parsing JSON input..." << endl;
    
    // Extract k value
    int k = parseK(jsonInput);
    cout << "k (threshold): " << k << endl;
    
    // Extract points
    vector<pair<int, long long>> points;
    
    // Parse each numbered key
    for (int i = 1; i <= 10; i++) {
        map<string, string> data = parseJsonValue(jsonInput, to_string(i));
        if (!data.empty()) {
            int x = i;
            int base = stoi(data["base"]);
            string value = data["value"];
            long long y = convertBaseToDecimal(value, base);
            points.push_back({x, y});
            
            cout << "Point " << i << ": x=" << x 
                 << ", base=" << base << ", value=" << value 
                 << " -> y=" << y << endl;
        }
    }
    
    // Use first k points to reconstruct secret
    vector<pair<int, long long>> selectedPoints(points.begin(), points.begin() + k);
    
    // Find secret (value at x=0)
    double secret = lagrangeInterpolation(selectedPoints, 0);
    
    cout << "\nSecret: " << secret << endl;
    cout << "Secret (rounded): " << round(secret) << endl;
    
    // Verification
    cout << "\nVerification:" << endl;
    for (int i = 0; i < min(3, (int)selectedPoints.size()); i++) {
        int x = selectedPoints[i].first;
        long long y = selectedPoints[i].second;
        double calculated = lagrangeInterpolation(selectedPoints, x);
        cout << "At x=" << x << ": expected y=" << y 
             << ", calculated y=" << calculated << endl;
    }
    
    return round(secret);
}

int main() {
    cout << "=== HASHIRA PLACEMENTS ASSIGNMENT ===" << endl;
    cout << "Shamir's Secret Sharing Implementation" << endl;
    cout << "=====================================" << endl << endl;
    
    // Test Case 1
    string testCase1 = R"({
        "keys": {"n": 4, "k": 3},
        "1": {"base": "10", "value": "4"},
        "2": {"base": "2", "value": "111"}, 
        "3": {"base": "10", "value": "12"},
        "6": {"base": "4", "value": "213"}
    })";
    
    cout << "=== TEST CASE 1 ===" << endl;
    long long secret1 = solveShamirSecretSharing(testCase1);
    cout << "\nFINAL ANSWER: " << secret1 << endl << endl;
    
    // Test Case 2
    string testCase2 = R"({
        "keys": {"n": 10, "k": 7},
        "1": {"base": "6", "value": "13444211440455345511"},
        "2": {"base": "15", "value": "aed7015a346d635"},
        "3": {"base": "15", "value": "6aeeb69631c227c"},
        "4": {"base": "16", "value": "e1b5e05623d881f"},
        "5": {"base": "8", "value": "316034514573652620673"},
        "6": {"base": "3", "value": "2122212201122002221120200210011020220200"},
        "7": {"base": "3", "value": "20120221122211000100210021102001201112121"},
        "8": {"base": "6", "value": "20220554335330240002224253"},
        "9": {"base": "12", "value": "45153788322a1255483"},
        "10": {"base": "7", "value": "1101613130313526312514143"}
    })";
    
    cout << "=== TEST CASE 2 ===" << endl;
    long long secret2 = solveShamirSecretSharing(testCase2);
    cout << "\nFINAL ANSWER: " << secret2 << endl << endl;
    
    cout << "=== SUMMARY ===" << endl;
    cout << "Test Case 1 Secret: " << secret1 << endl;
    cout << "Test Case 2 Secret: " << secret2 << endl;
    cout << "\nAlgorithm: Lagrange Interpolation" << endl;
    cout << "Method: Shamir's Secret Sharing Reconstruction" << endl;
    
    return 0;
}