#include<iostream>
#include<vector>
#include<string>
#include<sstream>
using namespace std;
vector<int> getinfo() {
    string info;
    vector<int>information;
    while (1) {
        getline(cin, info);
        stringstream sst(info);
        if (info == "#")break;
        string first_hour, first_minute, second_hour, second_minute;
        getline(sst, first_hour, ':');
        getline(sst, first_minute, '-');
        getline(sst, second_hour, ':');
        getline(sst, second_minute);
        int hour1 = stoi(first_hour), min1 = stoi(first_minute), hour2 = stoi(second_hour), min2 = stoi(second_minute);
        information.insert(information.end(), { hour1, min1, hour2, min2 });
    }
    return information;
}
vector<int> findfreetimes(vector<int> info) {
    int time_member = info.size();
    vector<int> freetimes;
    if (!(info[0] == 12 && info[1] == 0)) {
        freetimes.insert(freetimes.end(), { 12, 00, info[0], info[1] });
    }
    for (int i = 2; i < time_member - 3; i += 4) {
        freetimes.insert(freetimes.end(), { info[i], info[i + 1], info[i + 2], info[i + 3] });
    }
    if (!(info[time_member - 2] == 20 && info[time_member - 1] == 0)) {
        freetimes.insert(freetimes.end(), { info[time_member - 2], info[time_member - 1], 20, 0 });
    }
    return freetimes;
}
string update_freetiems(int time, vector<int>& freetimes, int counter) {
    int adapt_time = freetimes.size();
    string printout = "";
    for (int i = 0; i < adapt_time; i += 4) {
        int temp = 60 * (freetimes[i + 2] - freetimes[i]) + freetimes[i + 3] - freetimes[i + 1];
        if (time <= temp) {
            printout += to_string(counter) + " " + to_string(freetimes[i]) + ":";
            if (freetimes[i + 1] < 10) printout += "0" + to_string(freetimes[i + 1]);
            else printout += to_string(freetimes[i + 1]);
            freetimes[i] += (time + freetimes[i + 1]) / 60;
            freetimes[i + 1] = (time + freetimes[i + 1]) % 60;
            printout += "-" + to_string(freetimes[i]) + ":";
            if (freetimes[i + 1] < 10) printout += "0" + to_string(freetimes[i + 1]) + "\n";
            else printout += to_string(freetimes[i + 1]) + "\n";
            break;
        }
    }
    return printout;
}
int main() {
    vector<int> info = getinfo();
    vector<int> freetimes = findfreetimes(info);
    int time, id, counter = 0;
    string output = "";
    while (1) {
        counter++;
        if (!(cin >> time))break;
        cin >> id;
        output += update_freetiems(time, freetimes, counter);
    }
    cout << output;
}
