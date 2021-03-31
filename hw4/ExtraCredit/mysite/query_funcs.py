
from .models import State, Color, Team, Player

def add_player(team_id, jersey_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg):
    Player.object.get_or_create(team_id=Team.object.get(team_id = team_id), jersey_num=jersey_num , first_name=first_name, last_name=last_name, mpg=mpg, ppg=ppg, rpg=rpg, apg=apg, spg=spg, bpg=bpg)
    return

def add_team(name, state_id, color_id, wins, losses):
    Team.object.get_or_create(name=name,state_id=State.object.get(state_id=state_id), color_id = Color.object.get(color_id=color_id), wins = wins, losses =losses)
    return

def add_state(name):
    State.object.get_or_create(name = name)

def add_color(name):
    Color.object.get_or_create(name=name)
    
def query1( use_mpg,  min_mpg,  max_mpg,
             use_ppg,  min_ppg,  max_ppg,
             use_rpg,  min_rpg,  max_rpg,
             use_apg,  min_apg,  max_apg,
             use_spg,  min_spg,  max_spg,
             use_bpg,  min_bpg,  max_bpg
            ):
    ret = Player.objects.all()
    if(use_mpg):
        ret = ret.filter(mpg__gte = min_mpg, mpg__lte = max_mpg)
    if(use_ppg):
        ret = ret.filter(ppg__gte = min_ppg, mpg__lte = max_ppg)
    if(use_rpg):
        ret = ret.filter(mpg__gte = min_rpg, mpg__lte = max_rpg)
    if(use_apg):
        ret = ret.filter(mpg__gte = min_apg, mpg__lte = max_apg)
    if(use_spg):
        ret = ret.filter(mpg__gte = min_spg, mpg__lte = max_spg)
    if(use_bpg):
        ret = ret.filter(mpg__gte = min_bpg, mpg__lte = max_bpg)

    print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
    for tuple in ret:
        print(tuple)
    return
    
# As we have store the Color_id as the whole project
def query2(team_color):
    ret = Team.objects.filter(color_id__name = team_color)
    print("NAME")
    for tuple in ret:
        print(tuple.name)
    return

def query3(team_name):
    ret = Player.objects.filter(team_id__name = team_name).order_by('-ppg')
    print("FIRST_NAME LAST_NAME")
    for tuple in ret:
        print(tuple.first_name, tuple.last_name)
    return

def query4(team_state, team_color):
    ret = Player.objects.filter(team_id__state_id__name=team_state, team_id__color_id__name=team_color)
    print("FIRST_NAME LAST_NAME UNIFORM_NUM")
    for tuple in ret:
        print(tuple.first_name, tuple.last_ame, tuple.jersey_num)
    return

def query5(num_wins):
    ret = Player.objects.filter(team_id__wins__gt=num_wins)
    print("FIRST_NAME LAST_NAME TEAM_NAME WINS")
    for tuple in ret:
        print(tuple.first_name, tuple.last_name, tuple.team_id.name, tuple.team_id.wins)
    return

