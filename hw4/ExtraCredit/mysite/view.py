import os

os.environ.setdefault("DJANGO_SETTINGS_MODULE", "mysite.settings")

import django
# if(django.VERSION >= (1, 7)):
#     django.setup()

django.setup()

from orm.models import State, Color, Player,Team

from query_funcs import add_color, add_player, add_state, add_team
from query_funcs import query1, query2, query3, query4, query5

#def tableInitiation(filename, connect, )

def playerInitiation(filename):
    data = open(filename)
    for tuple in data:
        player_id , team_id , jersey_num , first_name , last_name , mpg , ppg,rpg,apg,spg,bpg = tuple.split(' ')
        add_player(team_id, jersey_num, first_name, last_name, mpg, ppg, rpg, apg, spg,bpg)
    
    data.close()
    return

def teamInitiation(filename):
    data = open(filename)
    for tuple in data:
        team_id, name ,state_id, color_id, wins, losses = tuple.split(' ')
        print(name, state_id, color_id, wins, losses)
        add_team(name, state_id, color_id, wins, losses)
    data.close()
    return


def stateInitiation(filename):
    data = open(filename)
    for tuple in data:
        state_id, name = tuple.split(' ')
        add_state(name)
    data.close()
    return


def colorInitiation(filename):
    data = open(filename)
    for tuple in data:
        color_id, name= tuple.split(' ')
        add_color(name)
    data.close()
    return

def show():
    stateInitiation("state.txt")
    colorInitiation("color.txt")
    teamInitiation("team.txt")
    playerInitiation("player.txt")
    print("Initial Success")
    query1(1, 35, 40, 0, 0, 0, 0, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    query1(0, 35, 40, 0, 0, 0, 1, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    query1(0, 35, 40, 0, 0, 0, 0, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    query2("Orange")
    query3("FloridaState")
    query3("Duke")
    query4("NC", "Red")
    query4("NC", "DarkBlue")
    query5(13)
    add_player(1, 1, "yyyy", "xxxx", 1, 1, 1, 1, 1, 1)
    query1(0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    query2("b")
    query3("haha")
    query4("NC", "Red")
    query5(5)
    add_player(1, 60, "xxxxx", "yyyy", 20, 20, 10, 10, 5.3, 5.3)
    add_team("team_baldwin", 10, 3, 20, 0)
    add_state("N/A")
    add_color("perry-winkle")
    query1(1, 10, 30, 0, 0, 40, 0, 0, 6, 0, 0, 5, 0, 0.0, 10.0, 0, 0.0, 10.0)
    query2("Gold")
    query3("FloridaState")
    query4("FL", "Maroon")
    query5(8)
    return

# if __name__ == "__main__":
#     main()
