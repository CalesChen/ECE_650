#include "helper.cpp"
#include "potato.h"
#include <vector>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>

class RingMaster{
  char* master_port;
  int num_players;
  int num_hops;

  int master_fd;
  vector<int> fds;
  vector<std::string> ips;
  vector<int> ports;
public:
  RingMaster(char ** argv){
    master_port = argv[1];
    num_players = atoi(argv[2]);
    num_hops = atoi(argv[3]);

    if(num_players <= 0){
      cerr<<"The num_players is "<< num_players << ", which is illegal."<<endl;
      exit(EXIT_FAILURE);
    }
    fds.resize(num_players);
    ips.resize(num_players);
    ports.resize(num_players);
    master_fd = server_start(master_port);

    cout<< "Potato Ringmaster"<<endl;
    cout<< "Players = " << num_players<<endl;
    cout<< "Hops = "<<num_hops<<endl;
  }

  ~RingMaster(){
    for(int i = 0 ; i < num_players ; i++){
      close(fds[i]);
    }
    close(master_fd);
  }

  
  void init() {
    for(int i = 0 ; i < num_players; i++){
      // As the get_port function in player.cpp just return the int
      int player_port;
      string player_ip;

      fds[i] = server_recv(master_fd, &ips[i]);//&player_ip);
      //cout << player_ip<<endl;
      // Send the ID and num_players
      send(fds[i], &i, sizeof(i), 0);
      send(fds[i], &num_players, sizeof(num_players), 0);

      // Receive the listen port

      recv(fds[i], &ports[i], sizeof(ports[i]), 0);
      //cout << player_port<<endl;
      //fds.push_back(player_master_fd);
      //ips.push_back(player_ip);
      //ports.push_back(player_port);
      //cout<< fds[i]<<ips[i]<<ports[i]<<endl;
      cout<< "Player "<< i << " is ready to play "<< endl;
    }

    // We need to init all of the players then make them connect to each other

    for(int i = 0 ; i < num_players ; i++){
      // Connect to the right
      int neighborId = i == num_players - 1? 0 : i + 1 ;
      int neighborPort = ports[neighborId];
      char neighbor_ip[100];

      memset(neighbor_ip, 0, sizeof(neighbor_ip));
      // Isolate the original data
      strcpy(neighbor_ip, ips[neighborId].c_str());

      send(fds[i], &neighborPort, sizeof(neighborPort),0);
      send(fds[i], &neighbor_ip, sizeof(neighbor_ip),0);

      //cout << neighborId << "  " << neighborPort << "  " << neighbor_ip << endl;
    }
  }

  void play(){
    Potato potato;
    potato.num_hops = num_hops;

    srand((unsigned int)time(NULL) + num_players); 

    if(num_hops != 0){
      int random = rand() % num_players;
      // Send the potato to the lucky guy
      send(fds[random], &potato, sizeof(potato), 0);
      cout << "Ready to start the game, sending potato to player "<< random<<endl;

      // Waiting for receive the potato with num_hops == 0

      fd_set allFds;
      int nfds = *max_element(fds.begin(), fds.end());

      FD_ZERO(&allFds);
      for(int i = 0 ; i < num_players ; i++){
        FD_SET(fds[i], &allFds);
      }

      select(nfds+1, &allFds, NULL, NULL, NULL);
      for(int i = 0 ; i < num_players; i++){
        if(FD_ISSET(fds[i], &allFds)){
          recv(fds[i], &potato, sizeof(potato), MSG_WAITALL);
          break;
        }
      }
    }


    //Break all of the players
    for(int i = 0 ; i < num_players ; i ++){
      send(fds[i], &potato, sizeof(potato),0);
    }

    cout << "Trace pf potato:" <<endl;
    for(int i = 0 ; i < potato.count ; i++){
      cout<<potato.path[i];
      if(i != potato.count - 1){
        cout << ",";
      }
      else{
        cout<<endl;
      }
    }
    
  }
};

int main(int argc, char ** argv){
  if(argc != 4){
    cout << "ringmaster <port_num> <num_players> <num_hops>"<<endl;
    return 1;
  }

  RingMaster ringmaster(argv);
  ringmaster.init();
  ringmaster.play();

  return 0;
}
