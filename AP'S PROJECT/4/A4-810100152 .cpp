#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

#define init_money 1000
#define init_health 100
#define max_money 10000
#define pistol_cost 400
#define heavy_cost 3000
#define pistol_damage 20
#define heavy_damage 45
#define knife_damage 35
#define knife_kill_prize 500
#define pistol_kill_prize 200
#define heavy_kill_prize 100
#define win_prize 2700
#define lose_prize 2400

using namespace std;

class Player {
private:
    string username;
    string team_name;
    int health = init_health;
    int money = init_money;
    int deaths = 0;
    int kills = 0;
    bool is_afk = false;
    bool has_pistol = false;
    bool has_heavy = false;
    bool is_dead = false;

public:

    explicit Player(string name, string team="") {
        this->username = name;
        this->team_name = team;
    }

    void kill(string weapon) {
        if (weapon == "pistol") {
            this->money += pistol_kill_prize;
        }
        else if (weapon == "heavy") {
            this->money += heavy_kill_prize;
        }
        else {
            this->money += knife_kill_prize;
        }
        this->kills += 1;
        if (this->money > max_money) {
            this->money = max_money;
        }
    }

    int get_money() {
        return this->money;
    }

    int get_health() {
        return this->health;
    }

    int get_deaths() {
        return this->deaths;
    }

    int get_kills() {
        return this->kills;
    }

    bool get_is_afk() {
        return this->is_afk;
    }

    bool get_has_pistol() {
        return this->has_pistol;
    }

    bool get_has_heavy() {
        return this->has_heavy;
    }

    bool get_is_dead() {
        return this->is_dead;
    }

    string get_username() {
        return this->username;
    }

    string get_team_name() {
        return this->team_name;
    }


    void set_money(int m) {
        this->money = m;
        if (this->money > max_money) {
            this->money = max_money;
        }
    }

    void set_health(int h) {
        this->health = h;
    }

    void set_deaths(int d) {
        this->deaths = d;
    }

    void go_afk() {
        this->is_afk = true;
    }

    void go_atk() {
        this->is_afk = false;
    }

    void set_has_pistol(bool hp) {
        this->has_pistol = hp;
    }

    void set_has_heavy(bool hh) {
        this->has_heavy = hh;
    }

    void set_dead(bool isd) {
        this->is_dead = isd;
    }

    void set_username(string name) {
        this->username = move(name);
    }
};


class Team {
private:
    string team_name;
public:
    explicit Team(string name) {
        this->team_name = name;
    }

    vector<Player> players;

    void print_scoreboard() {
        sort_scoreboard();
        cout << this->team_name << " players:" <<  endl;
        for (auto & player : players) {

            cout << player.get_username() << ' ';
            cout << player.get_kills() << ' ';
            cout << player.get_deaths() << '\n';
        }
    }

    void sort_scoreboard() {
        // sort alphabetically
        sort(players.begin(), players.end(), [](Player p1, Player p2) {
            return p1.get_username() < p2.get_username();
        });

        // sort based on death
        sort(players.begin(), players.end(), [](Player p1, Player p2) {
            return p1.get_deaths() < p2.get_deaths();
        });

        // sort based on kill
        sort(players.begin(), players.end(), [](Player p1, Player p2) {
            return p1.get_kills() > p2.get_kills();
        });
    }
};

class Game {
    Team terrorists = Team("terrorists");
    Team counter_terrorists = Team("counter-terrorists");
    bool is_started = false;
    int commands_num = 11;

public:

    static vector<string> get_command(string& command) {
        vector<string> args;
        string line;
        getline(cin, line);
        if (line.empty())
            return args;
        istringstream iss(line);
        string temp;
        while (iss >> temp) {
            args.push_back(temp);
        }
        command = args[0];
        args.erase(args.begin());
        return args;
    }

    void start(int rounds) {
        for (int i = 0; i < rounds*2; i++) {
            play_round();
            is_started = false;
            reset();
        }
        string command;
        cin >> command;
        if(command=="score-board") {
            counter_terrorists.print_scoreboard();
            terrorists.print_scoreboard();
        }
    }

    void play_round() {
        string command;
        vector<string> arguments;
        Player temp("");
        for(int i=0; i<commands_num; i++) {
            command = "";
            Player *p = &temp;
            arguments = get_command(command);

            if(command == "start") {
                is_started = true;
                cout << "fight!\n";
            }
            else if (command == "round") {
                commands_num = stoi(arguments[0]);
                break;
            }
            else if (command == "get-money") {

                p = find_player(arguments[0], p);
                cout << p->get_money() << '\n';
            }
            else if (command == "add-user") {
                add_user(arguments[0], arguments[1]);
                cout << "ok\n";
            }
            else if (command == "get-health") {
                p = find_player(arguments[0], p);
                cout << p->get_health() << '\n';
            }
            else if (command == "go-afk") {
                Player t("");
                for(auto& player : terrorists.players) {
                    if(player.get_username() == arguments[0]) {
                        player.go_afk();
                        break;
                    }
                }
                for(auto& player : counter_terrorists.players) {
                    if(player.get_username() == arguments[0]) {
                        player.go_afk();
                        break;
                    }
                }

                cout << "ok\n";
            }
            else if (command == "go-atk") {
                find_player(arguments[0], p);
                p->go_atk();
                cout << "ok\n";
            }
            else if (command == "shoot") {
                shoot(arguments[0], arguments[1], arguments[2]);
            }
            else if(command == "buy") {
                buy(arguments[0], arguments[1]);
            }
            else if (command == "score-board") {
                counter_terrorists.print_scoreboard();
                terrorists.print_scoreboard();
            }
        }

        if(!round_winner() && is_started) {
            cout << "counter-terrorist won\n";
            add_money(0);
        }
    }

    // a function for adding a player to a team
    void add_user(string username, string team_name) {
        if(is_started){
            return;
        }
        if(team_name == "terrorist") {
            Player p(username, "terrorist");
            terrorists.players.push_back(p);
        }
        else if(team_name == "counter-terrorist") {
            Player p(username, "counter-terrorist");
            counter_terrorists.players.push_back(p);
        }
        else
            cout << "invalid side\n";
    }

    Player* find_player(string username, Player *p) {
        p->set_username("");
        for(auto& player : terrorists.players) {
            if(player.get_username() == username) {
                p = &player;
                break;
            }
        }

        if(p->get_username().empty()) {
            for(auto& player : counter_terrorists.players) {
                if(player.get_username() == username) {
                    p = &player;
                    break;
                }
            }
        }
        return p;
    }


    void shoot( string atkr, string atkd, string wpn) {
        Player temp("");
        Player *attacker = &temp;
        attacker = find_player(atkr, attacker);
        Player *attacked = &temp;
        attacked = find_player(atkd, attacked);
        if (!is_started) {
            cout << "The game hasn't started yet" << endl;
            return;
        }
        if (attacker->get_is_dead()) {
            cout << "attacker is dead\n";
            return;
        }
        if (attacked->get_is_dead()) {
            cout << "attacked is dead\n";
            return;
        }
        if (attacked->get_is_afk() || attacker->get_is_afk() ||
            attacked->get_username().empty() || attacker->get_username().empty()) {
            cout << "user not available\n";
            return;
        }
        if ((!attacker->get_has_pistol() && wpn == "pistol") ||
            (!attacker->get_has_heavy() && wpn == "heavy")) {
            cout << "attacker doesn't have this gun\n";
            return;
        }
        if (attacker->get_team_name() == attacked->get_team_name()) {
            cout << "you can't shoot this player\n";
            return;
        }
        if (attacker->get_has_pistol() && wpn == "pistol") {
            attacked->set_health(attacked->get_health() - pistol_damage);
            cout << "nice shot\n";
        }
        else if (attacker->get_has_heavy() && wpn == "heavy") {
            attacked->set_health(attacked->get_health() - heavy_damage);
            cout << "nice shot\n";
        }
        else if (wpn == "knife") {
            attacked->set_health(attacked->get_health() - knife_damage);
            cout << "nice shot\n";
        }
        // check if player is dead
        if (attacked->get_health() <= 0) {
            attacker->kill(wpn);
            attacked->set_dead(true);
            attacked->set_deaths(attacked->get_deaths() + 1);
            attacked->set_health(0);
        }
    }

    void buy(string username, string weapon) {
        Player temp("");
        Player *p = &temp;
        p = find_player(username, p);
        if (is_started) {
            cout << "you can't buy weapons anymore\n";
            return;
        }
        if (p->get_is_afk() || p->get_username().empty()) {
            cout << "user not available\n";
            return;
        }

        if (weapon == "pistol") {
            if (p->get_has_pistol()) {
                cout << "you already have this weapon\n";
                return;
            }
            if (p->get_money() >= pistol_cost) {
                p->set_money(p->get_money() - pistol_cost);
                p->set_has_pistol(true);
                cout << "weapon bought successfully\n";
            }
            else
                cout << "insufficient money\n";
        }
        else if (weapon == "heavy") {
            if (p->get_has_heavy()) {
                cout << "you already have this weapon\n";
                return;
            }
            if (p->get_money() >= heavy_cost) {
                p->set_money(p->get_money() - heavy_cost);
                p->set_has_heavy(true);
                cout << "weapon bought successfully\n";
            }
            else
                cout << "insufficient money\n";
        }
        else if(weapon == "knife") {
            cout << "you already have this weapon\n";
            return;
        }
        else
            cout << "weapon not available\n ";
    }

    void reset() {
        for(auto& player : terrorists.players) {
            player.set_health(init_health);
            player.set_dead(false);
            player.set_has_heavy(false);
            player.set_has_pistol(false);
        }
        for(auto& player : counter_terrorists.players) {
            player.set_health(init_health);
            player.set_dead(false);
            player.set_has_heavy(false);
            player.set_has_pistol(false);
        }
    }


    bool round_winner() {
        bool terrorist_lose = true;
        bool counter_terrorist_lose = true;
        // check if all dead
        for (int i=0; i<counter_terrorists.players.size(); i++) {
            if (!terrorists.players[i].get_is_dead()) {
                terrorist_lose = false;
                break;
            }
        }
        for (int i=0; i<terrorists.players.size(); i++) {
            if (!counter_terrorists.players[i].get_is_dead()) {
                counter_terrorist_lose = false;
                break;
            }
        }

        if (terrorist_lose) {
            cout << "counter-terrorist won\n";
            add_money(0);
            return true;
        }
        if (counter_terrorist_lose) {
            cout << "terrorist won\n";
            add_money(1);
            return true;
        }
        return false;
    }

    void add_money(int winner) {
        if (winner == 0) {
            for(auto& player : counter_terrorists.players) {
                player.set_money(player.get_money() + win_prize);
            }
            for(auto& player : terrorists.players) {
                player.set_money(player.get_money() + lose_prize);
            }
        }
        else {
            for(auto& player : terrorists.players) {
                player.set_money(player.get_money() + win_prize);
            }
            for(auto& player : counter_terrorists.players) {
                player.set_money(player.get_money() + lose_prize);
            }
        }
    }

};


int main() {
    string command;
    vector<string> arguments;
    int r;
    cin >> r;
    Game game;
    game.start(r);

    return 0;
}
