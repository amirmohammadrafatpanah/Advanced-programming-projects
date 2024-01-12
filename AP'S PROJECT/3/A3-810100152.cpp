#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

struct entry {
    string name;
    int start{};
    int end{};
    vector<string> languages;
    vector<string> translator_name;
};

int to_minutes(const string& time) {
    int minutes;
    string time_of_event = time.substr(0, 2);
    minutes = stoi(time_of_event) * 60 + stoi(time.substr(3));
    return minutes;
}

void sort_event_languages(vector<string>* availible_language, map<string, int>* language_check) {
    sort(availible_language->begin(), availible_language->end(),
        [language_check](const string& first_input_string, const string& second_input_string) {
            return (*language_check)[first_input_string] < (*language_check)[second_input_string];
        });
}

void read_file(vector<entry>* translators, vector<entry>* event_checker, map<string, vector<string> >* unique_language, const string& file_name) {
    ifstream file;
    string seprate_line_of_input_files;
    int file_info;
    file.open(file_name);
    map<string, int> language_count;
    for (int i = 0; i < 2; ++i) { 
        getline(file, seprate_line_of_input_files);
        file_info = stoi(seprate_line_of_input_files);
        for (int j = 0; j < file_info; ++j) {
            entry time_of_event;
            file >> time_of_event.name;
            file >> seprate_line_of_input_files;
            time_of_event.start = to_minutes(seprate_line_of_input_files);
            file >> seprate_line_of_input_files;
            time_of_event.end = to_minutes(seprate_line_of_input_files);
            getline(file, seprate_line_of_input_files);
            stringstream spilit_line(seprate_line_of_input_files);
            while (spilit_line >> seprate_line_of_input_files) {
                time_of_event.languages.push_back(seprate_line_of_input_files);
                if (i == 0) language_count[seprate_line_of_input_files] += 1;
            }
            if (i == 0) translators->push_back(time_of_event);
            else {
                (*unique_language)[time_of_event.name] = time_of_event.languages;
                sort_event_languages(&time_of_event.languages, &language_count);
                event_checker->push_back(time_of_event);
            }
        }
    }
    file.close();
}

void sort_translators(vector<entry>* translators) {
    sort(translators->begin(), translators->end(),
        [](entry first_event, entry second_event) {
            return first_event.name < second_event.name;
        });
    sort(translators->begin(), translators->end(),
        [](entry first_event, entry second_event) {
            return first_event.languages.size() < second_event.languages.size();
        });
}

void scheduling(vector<entry>* translator_language, vector<entry>* event_language) {
    bool is_valid;
    for (int i = 0; i < (*event_language).size(); i++) {
        for (auto& language : (*event_language)[i].languages) {
            for (auto& translators : (*translator_language)) {
                is_valid = true;

                if (find(translators.languages.begin(), translators.languages.end(), language) != translators.languages.end()
                    && (translators.start <= (*event_language)[i].start && (*event_language)[i].end <= translators.end)) {

                    for (int j = 0; j < i; ++j) { 

                        if (find((*event_language)[j].translator_name.begin(), (*event_language)[j].translator_name.end(), translators.name) != (*event_language)[j].translator_name.end() &&
                            ((*event_language)[j].start < (*event_language)[i].end && (*event_language)[i].start < (*event_language)[j].end)) { 
                            is_valid = false;
                            break;
                        }
                    }
                    if (find((*event_language)[i].translator_name.begin(), (*event_language)[i].translator_name.end(), translators.name) != (*event_language)[i].translator_name.end())
                        is_valid = false;
                }
                else is_valid = false;

                if (is_valid) {
                    (*event_language)[i].translator_name.push_back(translators.name);
                    break;
                }
            }
            if (!is_valid) {
                (*event_language)[i].translator_name.push_back("Not Found");
            }
        }
    }
}

void print(vector<entry>* event_list, map<string, vector<string> >* unique_language) {
    for (auto& event : *event_list) {
        cout << event.name << endl;
        for (auto& sutible_language : (*unique_language)[event.name]) {
            int i = find(event.languages.begin(), event.languages.end(), sutible_language) - event.languages.begin();
            cout << sutible_language << ": " << event.translator_name[i] << endl;
        }
    }
}

int main(int argc, char const* argv[]) {
    vector<entry> translator_list;
    vector<entry> event_list;
    map<string, vector<string> > unsorted_languages;
    read_file(&translator_list, &event_list, &unsorted_languages, argv[1]);
    sort_translators(&translator_list);
    scheduling(&translator_list, &event_list);
    print(&event_list, &unsorted_languages);
    return 0;
}
