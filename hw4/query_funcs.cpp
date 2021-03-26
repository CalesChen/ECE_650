#include "query_funcs.h"
#include <iomanip>

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
    work W(*C);
    stringstream sql;
    // W.quote() will maintain the capital letter. 
    sql << "INSERT INTO player (team_id, jersey_num, first_name, last_name,mpg, ppg, rpg, apg, spg, bpg) VALUES (" 
            <<team_id << "," << jersey_num << ","<< W.quote(first_name)<< "," << W.quote(last_name)<< ","<<mpg<< ","<<ppg<< ","<<rpg<< ","<<apg<< ","<<spg<< ","
            <<bpg<<");";
    cout<<spg<<endl;
    W.exec(sql.str());
    W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
    work W(*C);
    stringstream sql;
    // W.quote() will maintain the capital letter. 
    sql << "INSERT INTO team (name, state_id, color_id, wins, losses) VALUES (" 
            <<W.quote(name)<< "," << state_id << "," << color_id << "," << wins << "," << losses << ");";

    W.exec(sql.str());
    W.commit();
}


void add_state(connection *C, string name)
{
    work W(*C);
    stringstream sql;
    sql << "INSERT INTO state (name) VALUES (" << W.quote(name) << ");";
    W.exec(sql.str());
    W.commit();
}


void add_color(connection *C, string name)
{
    work W(*C);
    stringstream sql;
    sql << "INSERT INTO color (name) VALUES (" << W.quote(name) << ");";
    W.exec(sql.str());
    W.commit();
}

void query1Helper(int min, int max, stringstream & sql, bool & appendStr, string attr){
    if(appendStr){
        sql << " WHERE ";
    }
    else{
        sql << " AND ";
    }
    sql << "("<<attr<<" between " << min << " AND "<< max<<") ";
    appendStr = false;
}

void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
    stringstream sql;
    sql<< "SELECT * from player";
    // if(use_mpg && use_ppg && use_rpg && use_apg && use_spg && use_spg && use_bpg){
    //     sql << " WHERE ";
    // }
    bool appendStr = true;
    if(use_mpg){
        query1Helper(min_mpg, max_mpg, sql, appendStr, "mpg");
    }
    if(use_ppg){
        query1Helper(min_ppg, max_ppg, sql, appendStr, "ppg");
    }
    if(use_rpg){
        query1Helper(min_rpg, max_rpg, sql, appendStr, "rpg");
    }
    if(use_apg){
        query1Helper(min_apg, max_apg, sql, appendStr, "apg");
    }
    if(use_spg){
        query1Helper(min_spg, max_spg, sql, appendStr, "spg");
    }
    if(use_bpg){
        query1Helper(min_bpg, max_bpg, sql, appendStr, "bpg");
    }

    sql<<";";

    /* Create a non-transactional object. */
    nontransaction N(*C);
      
    /* Execute SQL query */
    result R( N.exec( sql.str() ));

    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG\n";
    for(result::const_iterator c = R.begin() ; c != R.end() ; ++c){
        cout << c[0].as<int>() <<" "<< c[1].as<int>() <<" "<< c[2].as<int>()<<" "<<c[3].as<string>()<<" "
            << c[4].as<string>() <<" "<< c[5].as<int>() <<" "<< c[6].as<int>() <<" "<< c[7].as<int>()<<" "
            << c[8].as<int>() <<" "<< fixed << setprecision(1) << c[9].as<double>() <<" "<< c[10].as<double>() << "\n";
    }
}


void query2(connection *C, string team_color)
{
    work W(*C);
    stringstream sql;
    sql<< "SELECT team.name " <<
          "FROM team, color " <<
          "WHERE team.color_id = color.color_id and color.name = " + W.quote(team_color) + ";";
    nontransaction N(*C);
    result R(N.exec(sql.str()));
    cout << "NAME\n";
    for(auto c = R.begin() ; c != R.end() ; ++c){
        cout << c[0].as<string>() << "\n";
    }
}


void query3(connection *C, string team_name)
{
    work W(*C);
    stringstream sql;
    sql << "SELECT player.first_name, player.last_name "
        << "FROM player team "
        << "WHERE player.team_id = team.team_id and team.name = " + W.quote(team_name) + " ORDER BY ppg DESC;";

    nontransaction N(*C);
    result R(N.exec(sql.str()));
    cout << "FIRST_NAME LAST_NAME\n";
    for(auto c = R.begin() ; c != R.end() ; ++c){
        cout << c[0].as<string>() << " " << c[1].as<string>() << "\n";
    }
}


void query4(connection *C, string team_state, string team_color)
{
    work W(*C);
    stringstream sql;

    sql << "SELECT player.first_name, player.last_name, player.jersey_num " 
        << "FROM player, team, state, color "
        << "WHERE player.team_id = team.team_id AND team.state_id = state.state_id AND team.color_id = color.color_id AND state.name = "
        << W.quote(team_state) << " AND color.name = "<<W.quote(team_color)<<';';

    nontransaction N(*C);
    result R(N.exec(sql.str()));
    // In the example, the jersey_NUm is Uniform_num. So I just copy that.
    cout<< "FIRST_NAME LAST_NAME UNIFORM_NUM\n";

    for(auto c = R.begin() ; c != R.end() ; ++c){
        cout << c[0].as<int>() << " " <<c[1].as<int>() << " " << c[2].as<int>() <<"\n";
    }
}


void query5(connection *C, int num_wins)
{
    work W(*C);
    stringstream sql;

    sql << "SELECT player.first_name, player.last_name, team.name, team.wins " 
        << "FROM player, team "
        << "WHERE player.team_id = team.team_id AND team.wins > " 
        << num_wins<<";";

    nontransaction N(*C);
    result R(N.exec(sql.str()));
    // In the example, the jersey_NUm is Uniform_num. So I just copy that.
    cout<< "FIRST_NAME LAST_NAME TEAM_NAME WINS\n";

    for(auto c = R.begin() ; c != R.end() ; ++c){
        cout << c[0].as<string>() << " " <<c[1].as<string>() << " " << c[2].as<string>() << " "
            << c[3].as<int>() << "\n";
    }
}
