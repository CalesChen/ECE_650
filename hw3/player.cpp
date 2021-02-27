#include "helper.cpp"
#include "potato.h"
#include <vector>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>


class Player{
  
  int port;
  int player_id;
  int num_players;

  int from_master_fd;
  int lneighbor_fd;
  int rneighbor_fd;
  int player_fd;
  //  vector<int>

public:
  Player(char ** argv){
    from_master_fd = client_start(argv[1], argv[2]);
    recv(from_master_fd, &player_id, sizeof(player_id),0);
    recv(from_master_fd, &num_players, sizeof(num_players), 0);

    player_fd = server_start("");
    int player_port = get_port(player_fd);
    send(from_master_fd, &player_port, sizeof(player_port), 0);
    cout<< "Connected as player "<< player_id<<" out of " << num_players
        <<" total players"<<endl;
    
    int neighbor_port;
    char neighbor_ip[100];
    recv(from_master_fd, &neighbor_port, sizeof(neighbor_ip), MSG_WAITALL);
    cout << neighbor_port<<endl;
    recv(from_master_fd, &neighbor_ip, sizeof(neighbor_ip), MSG_WAITALL);
    cout << neighbor_ip << " and port is "<<neighbor_port;
    // connect to the right and listen to the left. 
    char neighbor_port_str[10];
    sprintf(neighbor_port_str, "%d", neighbor_port);
    rneighbor_fd = client_start(neighbor_ip, neighbor_port_str);

    string lneighbor_ip;
    lneighbor_fd = server_recv(player_fd, &lneighbor_ip);
  }

  ~Player(){
    close(from_master_fd);
    close(lneighbor_fd);
    close(rneighbor_fd);
    close(player_fd);
  }

  void play(){
    Potato potato;
    vector<int> fd_set_mrl{from_master_fd, rneighbor_fd, lneighbor_fd};
    fd_set fds;
    int nfds = *max_element(fd_set_mrl.begin(), fd_set_mrl.end());
    srand((unsigned int)time(NULL) + player_id);
    
    bool flag = true;
    while(flag){
      // Set Zero
      FD_ZERO(&fds);
      // Set Value
      for(int i = 0 ; i < 3 ; i ++){
        FD_SET(fd_set_mrl[i], &fds);
      }
      select(nfds + 1, &fds, NULL, NULL, NULL);

      int len;
      for(int i = 0 ; i < 3 ; i ++){
        if(FD_ISSET(fd_set_mrl[i], &fds)){
          len = recv(fd_set_mrl[i], &potato, sizeof(potato), MSG_WAITALL);
          break;
        }
      }

      // The potato is over
      if(potato.num_hops == 0 || len == 0){
        break;
      }

      else if (potato.num_hops == 1){
        potato.num_hops-=1;
        potato.path[potato.count] = player_id;
        potato.count +=1;
        send(from_master_fd, &potato, sizeof(potato), 0);
        cout << "I'm it"<<endl;
      }

      else{
        potato.num_hops-=1;
        potato.path[potato.count] = player_id;
        potato.count +=1;

        int random = rand() % 2;
        if(random == 0){
          send(lneighbor_fd, &potato, sizeof(potato), 0);
          int lneighbor_id = (player_id - 1) == -1? (num_players - 1):(player_id - 1);
          cout << "Sending potato to " << lneighbor_id <<endl;
        }
        else{
          send(rneighbor_fd, &potato, sizeof(potato), 0);
          int rneighbor_id = (player_id + 1) == num_players? 0:(player_id + 1);
          cout << "Sending potato to " << rneighbor_id <<endl;  
        }
      }
    }
    
  }

  int get_port(int socket_fd){
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if(getsockname(socket_fd, (struct sockaddr *)&sin, &len) == -1){
      cerr<<"getsockname error"<<endl;
      exit(EXIT_FAILURE);
    }
    return ntohs(sin.sin_port);
  }
};

int main(int argc, char ** argv){
  Player player(argv);
  player.play();
  return 0;
}
