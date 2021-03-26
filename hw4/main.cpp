#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include "exerciser.h"

using namespace std;
using namespace pqxx;

void SQLExec(const string & sql, connection * C){
  work W(*C);
  W.exec(sql);
  W.commit();
}

void dropOneTable(connection * C, string tablename){
  string sql = "DROP TABLE IF EXISTS " + tablename + " CASCADE;";
  SQLExec(sql, C);
}

void dropMultipleTable(vector<string> & s, connection * C){
  for(string tablename : s){
    dropOneTable(C, tablename);
    //cout<<tablename<<endl;
  }
}

void tableInitiation(string filename, connection * C){
  string sql, line;
  ifstream ifs (filename.c_str());
  if(ifs.is_open()){
    while(getline(ifs, line)){
      sql += line;
    }
    //cout<<sql<<endl;
    ifs.close();
  }
  else{
    cout<<"Error! Can not open the " + filename + "\n";
    return;
  }
  SQLExec(sql, C);
}

void playerInitiation(string filename, connection * C){
  string line, first_name, last_name;
  int player_id, team_id, jersey_num, mpg, ppg, rpg, apg;
  double spg, bpg;
  ifstream ifs(filename.c_str());
  if(ifs.is_open()){
    while(getline(ifs, line)){
      stringstream value;
      value << line;
      value >> player_id >> team_id >> jersey_num >> first_name >> last_name >> mpg >> ppg>>rpg>>apg>>spg>>bpg;
      add_player(C, team_id, jersey_num, first_name, last_name, mpg, ppg, rpg, apg, spg,bpg);
      cout<<spg;
    }
    ifs.close();
  }
  else{
    cout<<"Error! Can not open the " + filename + "\n";
    return;
  }
}

void teamInitiation(string filename, connection * C){
  string line, name;
  int team_id, state_id, color_id, wins, losses;
  ifstream ifs(filename.c_str());
  if(ifs.is_open()){
    while(getline(ifs, line)){
      stringstream value;
      value << line;
      value >> team_id>> name >>state_id>> color_id>> wins>> losses;
      add_team(C, name, state_id, color_id, wins, losses);
      //cout<<"team success"<<endl;
    }
    ifs.close();
  }
  else{
    cout<<"Error! Can not open the " + filename + "\n";
    return;
  }
}
void stateInitiation(string filename, connection * C){
  string line, name;
  int state_id;
  ifstream ifs(filename.c_str());
  if(ifs.is_open()){
    while(getline(ifs, line)){
      stringstream value;
      value << line;
      value >> state_id>> name;
      add_state(C, name);
    }
    ifs.close();
  }
  else{
    cout<<"Error! Can not open the " + filename + "\n";
    return;
  }
}
void colorInitiation(string filename, connection * C){
  string line, name;
  int color_id;
  ifstream ifs(filename.c_str());
  if(ifs.is_open()){
    while(getline(ifs, line)){
      stringstream value;
      value << line;
      value >> color_id>> name;
      add_color(C, name);
    }
    ifs.close();
  }
  else{
    cout<<"Error! Can not open the " + filename + "\n";
    return;
  }
}


int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      //cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
    
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }


  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files

  vector<string> table;
  table.push_back("player");
  table.push_back("team");
  table.push_back("state");
  table.push_back("color");

  dropMultipleTable(table, C);
  //cout<<"Success!"<<endl;

  tableInitiation("initiation.sql", C);
  stateInitiation("state.txt", C);
  colorInitiation("color.txt", C);
  
  teamInitiation("team.txt", C);
  playerInitiation("player.txt", C);
  
  exercise(C);


  //Close database connection
  C->disconnect();

  return 0;
}


