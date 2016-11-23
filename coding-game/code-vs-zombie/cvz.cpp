#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <set>
#include <cfloat>

using namespace std;

#define ZMOVE 400
#define AMOVE 1000
#define CIRCLE_POINTS 8
#define WIGGLE_ANGLE 5
#define WIGGLE_SPACE 900
#define PI 3.14159265
#define REDO 7

double get_angle(int x1, int y1, int x2, int y2) {
  return (atan2((y2 - y1), (x2 - x1)));
}

vector<float> get_translated_point(int x1, int y1, double angle) {
  vector<float> ret_point;
  ret_point.push_back(x1 + AMOVE * cos(angle));
  ret_point.push_back(y1 + AMOVE * sin(angle));
  return ret_point;
}

/**
 * Save humans, destroy zombies!
 **/

int initial_zombie_count = 0;
int initial_human_count = 0;

typedef struct human {
  int id;
  int pos[2];
  bool alive;

  human() {
    id = -1;
    pos[0] = -1; pos[1] = -1;
    alive = true;
  }
}human;

typedef struct zombie {
  int id;
  int curr_pos[2];
  int future_pos[2];
  bool alive; // :P

  zombie() {
    id = -1;
    curr_pos[0] = -1; curr_pos[1] = -1;
    future_pos[0] = -1; future_pos[1] = -1;
    alive = true;
  }
}zombie;

typedef struct threat {
  int zombie_id;
  float euclidean_distance;
  int zombie_distance;

  threat() {
    zombie_id = -1;
    euclidean_distance = -1;
    zombie_distance = -1;
  }
}threat;

vector<human> human_vec;
vector<zombie> zombie_vec;
vector< vector<threat> > human_to_zombie;
vector<human> all_human;
vector<zombie> all_zombie;
vector<bool> hopeless_humans;
vector<int> prev_zombie_mapping;
vector<int> current_zombie_mapping;
string output_message;

float get_dist(int x1, int y1, int x2, int y2) {
  return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

bool vectors_equal(vector<int> &a, vector<int> &b) {
  if(a.size() != b.size()) { return false; }

  for(int i = 0; i < a.size(); i++) {
    if(a[i] != b[i]) { return false; } 
  }
  return true;
}

human get_human_obj(vector<human> &hvec, int hid) {
  for(int i = 0; i < hvec.size(); i++) {
    if(hvec[i].id == hid) { return hvec[i]; }
  }
  assert(0);
  return hvec[0];
}

zombie get_zombie_obj(vector<zombie> &zvec, int zid) {
  for(int i = 0; i < zvec.size(); i++) {
    if(zvec[i].id == zid) { return zvec[i]; }
  }
  assert(0);
  return zvec[0];
}

void update_human_info(vector<human> &hvec, human &h, bool first_turn) {
  /* Upadate all globals */
  all_human[h.id].id = h.id;
  all_human[h.id].alive = h.alive;
  all_human[h.id].pos[0] = h.pos[0];
  all_human[h.id].pos[1] = h.pos[1];

  if(first_turn) { 
    hvec.push_back(h);
    return;
  }

  bool element_found = false;
  for(int i = 0; i < hvec.size(); i++) {
    if(hvec[i].id == h.id) {
      hvec[i].alive = h.alive;
      hvec[i].pos[0] = h.pos[0];
      hvec[i].pos[1] = h.pos[1];
      element_found = true;
    }
  }
  assert(element_found);
}

void update_zombie_info(vector<zombie> &zvec, zombie &z, bool first_turn) {
  /* Update all globals */
  all_zombie[z.id].id = z.id;
  all_zombie[z.id].alive = z.alive;
  all_zombie[z.id].curr_pos[0] = z.curr_pos[0];
  all_zombie[z.id].curr_pos[1] = z.curr_pos[1];
  all_zombie[z.id].future_pos[0] = z.future_pos[0];
  all_zombie[z.id].future_pos[1] = z.future_pos[1];

  if(first_turn) {
    zvec.push_back(z);
    return;
  }

  bool element_found = false;
  for(int i = 0; i < zvec.size(); i++) {
    if(zvec[i].id == z.id) {
      zvec[i].alive = z.alive;
      zvec[i].curr_pos[0] = z.curr_pos[0];
      zvec[i].curr_pos[1] = z.curr_pos[1];
      zvec[i].future_pos[0] = z.future_pos[0];
      zvec[i].future_pos[1] = z.future_pos[1];
      element_found = true;
    }
  }
  assert(element_found);
}

void fill_threat(human h, zombie z, threat &t) {
  t.zombie_id = z.id;
  t.euclidean_distance = get_dist(h.pos[0], h.pos[1], z.curr_pos[0], z.curr_pos[1]);
  t.zombie_distance = (t.euclidean_distance / ZMOVE) + 1; 
}

void update_threat(human h, zombie z, threat &t) {
  t.zombie_id = z.id;
  t.euclidean_distance = get_dist(h.pos[0], h.pos[1], z.curr_pos[0], z.curr_pos[1]);
  t.zombie_distance = (t.euclidean_distance / ZMOVE) + 1; 
}

void update_threats(vector<vector<threat> > &h2z) {
  for(int i = 0; i < h2z.size(); i++) {
    for(int j = 0; j < h2z[i].size(); j++) {
      update_threat(all_human[i], all_zombie[h2z[i][j].zombie_id], h2z[i][j]);
    }
  }
}

int nearest_human(vector<human> &hvec, int zfx, int zfy) {
  float closest = FLT_MAX;
  int selected_human_index = -1;
  for(int i = 0; i < hvec.size(); i++) {
    int hx = hvec[i].pos[0];
    int hy = hvec[i].pos[1];
    float dist = get_dist(hx, hy, zfx, zfy);

    if(dist < closest) {
      closest = dist;
      selected_human_index = i;
    }
  }

  assert(selected_human_index != -1);
  return selected_human_index;
}

void build_human_to_zombie_assoc(vector<vector<threat> > &h2z, vector<human> &hvec, vector<zombie> &zvec) {

  assert(h2z.size() != 0);
  for(int i = 0; i < h2z.size(); i++) {
    h2z[i].clear();
  }

  for(int i = 0; i < zvec.size(); i++) { 
    int selected_human_index = nearest_human(hvec, zvec[i].future_pos[0], zvec[i].future_pos[1]);
    
    threat t;
    fill_threat(hvec[selected_human_index], zvec[i], t);

    h2z[hvec[selected_human_index].id].push_back(t);
  }
}

void delete_zombie_assoc(vector<vector<threat> > &human_to_zombie, int hid, int zid) {
  vector<threat>::iterator it = human_to_zombie[hid].begin(); 
  bool element_deleted = false;
  while(it != human_to_zombie[hid].end()) {
    if(it->zombie_id == zid) {
      it = human_to_zombie[hid].erase(it);
      element_deleted = true;
      break;
    } else {
      it++;
    }
  }
  assert(element_deleted);
}

void perform_reassignment(vector<pair<int, int> > &zombie_reassignment, vector<vector<threat> > &human_to_zombie,
			  vector<human> &hvec, vector<zombie> &zvec) {

  for(int i = 0; i < zombie_reassignment.size(); i++) {
    int prev_ass = zombie_reassignment[i].first;
    int curr_ass = zombie_reassignment[i].second;
    if(prev_ass == curr_ass) { continue; }

    delete_zombie_assoc(human_to_zombie, prev_ass, i);
    if(curr_ass != -1) {
      threat t;
      assert(all_human[curr_ass].id != -1);
      assert(all_zombie[i].id != -1);
      fill_threat(all_human[curr_ass], all_zombie[i], t);
      human_to_zombie[curr_ass].push_back(t);
    }
  }
}
      
vector<int> get_zombie_mapping(vector<human> &hvec, vector<zombie> &zvec) {
  vector<int> z_mapping;
  z_mapping.resize(initial_zombie_count);

  for(int i = 0; i < initial_zombie_count; i++) {
    z_mapping[i] = -1;
  }

  for(int i = 0; i < zvec.size(); i++) {
    int selected_human_index = nearest_human(hvec, zvec[i].future_pos[0], zvec[i].future_pos[1]);
    z_mapping[zvec[i].id] = hvec[selected_human_index].id;
  }

  return z_mapping;
}

vector<pair<int, int> > get_reassignment(vector<int> &prev_zombie_mapping, vector<int> &current_zombie_mapping) {
  assert(prev_zombie_mapping.size() == current_zombie_mapping.size());

  vector<pair<int, int> > assignments;
  assignments.resize(prev_zombie_mapping.size());

  for(int i = 0; i < prev_zombie_mapping.size(); i++) {
    int prev_assignment = prev_zombie_mapping[i];
    int curr_assignment = current_zombie_mapping[i];

    if(prev_assignment == curr_assignment) { 
      assignments[i].first = -1;
      assignments[i].second = -1;
      continue; 
    }

    if(curr_assignment == -1) 
    { 
      assert(prev_assignment >= 0);
      /* Zombie dead */
      assignments[i].first = prev_assignment;
      assignments[i].second = curr_assignment;
    } else {
      assert(prev_assignment >= 0);
      assert(curr_assignment >= 0);
      /* Actual reassignment */
      assignments[i].first = prev_assignment;
      assignments[i].second = curr_assignment;
    }
  }
  return assignments;
}

int get_minimal_ash_moves(int sx, int sy, int dx, int dy) {
  
  int moves = INT_MAX;
  for(int i = 0; i < 8; i++) {
    int this_moves = (get_dist(sx, sy, dx + WIGGLE_SPACE * (cos(i * 45)), dy + WIGGLE_SPACE * sin(i * 45)) / AMOVE) + 1;
    if(this_moves < moves) {
      moves = this_moves;
    }
  }

  return moves;
}

vector<int> get_ash_targets_ring_based(vector<vector<threat> > &human_to_zombie, int aposx, int aposy, vector<bool> &hopeless_humans) {
  vector<int> targets;
  
  bool all_hopeless = true;
  for(int i = 0; i < hopeless_humans.size(); i++) {
    if(hopeless_humans[i] == false) {
      all_hopeless = false;
    } 
  }

  if(all_hopeless) {
    vector<int> ret_vector;
    ret_vector.push_back(-1);
    return ret_vector;
  }

  int ring = INT_MAX;
  for(int i = 0; i < human_to_zombie.size(); i++) {
    if(hopeless_humans[i] == true) { continue; }
    for(int j = 0; j < human_to_zombie[i].size(); j++) {
      if(human_to_zombie[i][j].zombie_distance < ring) {
	ring = human_to_zombie[i][j].zombie_distance;
      }
      //targets.push_back(human_to_zombie[i][j].zombie_id);
    }
  }

  if(ring == INT_MAX) {
    vector<int> ret_vector;
    ret_vector.push_back(-1);
    return ret_vector;
  }

  /* All zombies with zombie distance == ring are the targets */
  for(int i = 0; i < human_to_zombie.size(); i++) {
    if(hopeless_humans[i] == true) { continue; }
    for(int j = 0; j < human_to_zombie[i].size(); j++) {
      if(human_to_zombie[i][j].zombie_distance == ring) {
	targets.push_back(human_to_zombie[i][j].zombie_id);
      }
    }
  }

  int total_targets = targets.size();
  vector<int> targets_in_order; 
  /* Using a greedy approach */
  vector<int>::iterator it, del_it;
  for(int i = 0; i < total_targets; i++) {
    float closest = FLT_MAX;
    for(it = targets.begin(); it != targets.end(); it++) {
      float distance = get_dist(aposx, aposy, all_zombie[*it].future_pos[0], all_zombie[*it].future_pos[1]);
      if(distance < closest) {
	closest = distance;
	del_it = it;
      }
    }
    int t = *del_it;
    targets_in_order.push_back(t);
    targets.erase(del_it);
  }
  assert(targets.size() == 0);

  vector<int> impossible_targets;
  /* Remove impossible targets */
  int hx = aposx; int hy = aposy; int passed_moves = 0;
  for(int i = 0; i < targets_in_order.size(); i++) {
    int mapped_human = current_zombie_mapping[targets_in_order[i]];
    int dx = all_human[mapped_human].pos[0];
    int dy = all_human[mapped_human].pos[1];

    //int ash_moves = (get_dist(hx, hy, dx, dy) / AMOVE) + 1;
    int ash_moves = get_minimal_ash_moves(hx, hy, dx, dy);

    if(ring - (passed_moves + ash_moves) <= 0) {
      /* No point in saving this guy; ditch him */
      impossible_targets.push_back(targets_in_order[i]);
      hopeless_humans[mapped_human] = true;
    } else {
      passed_moves += ash_moves;
      hx = dx;
      hy = dy;
    }
  }

  for(int i = 0; i < impossible_targets.size(); i++) {
    it = targets_in_order.begin();
    while(it != targets_in_order.end()) {
      if(*it == impossible_targets[i]) {
	it = targets_in_order.erase(it);
	break;
      } else {
	it++;
      }
    }
  }

  return targets_in_order;
}

vector<int> get_ash_targets(vector<vector<threat> > &human_to_zombie, int aposx, int aposy) {
  hopeless_humans.clear();
  hopeless_humans.resize(initial_human_count);

  for(int i = 0; i < initial_human_count; i++) {
    hopeless_humans[i] = true; 
  }

  for(int i = 0; i < human_vec.size(); i++) {
    hopeless_humans[human_vec[i].id] = false;
  }

  vector<int> targets_in_order;
  while(targets_in_order.size() == 0) {
    targets_in_order = get_ash_targets_ring_based(human_to_zombie, aposx, aposy, hopeless_humans);
  }
  return targets_in_order;
}


bool can_kill_next_turn(int x, int y, int tx, int ty)  {
  double angle = get_angle(x, y, tx, ty);
  vector<float> tpos = get_translated_point(x, y, angle);

  if(get_dist(tpos[0], tpos[1], tx, ty) > 2000) {
    return false;
  } else {
    return true;
  }
}

int get_num_kills(int dx, int dy) {
  int ret_val = 0;
  for(int i = 0; i < zombie_vec.size(); i++) {
    if(get_dist(dx, dy, zombie_vec[i].future_pos[0], zombie_vec[i].future_pos[1]) <= 2000) {
      ret_val++;
    }
  }
  assert(ret_val >= 1);
  return ret_val;
}

vector<int> get_best_position(int target, int x, int y) {
  vector<int> best_pos;
  assert(all_zombie[target].alive);
  int tx = all_zombie[target].future_pos[0];
  int ty = all_zombie[target].future_pos[1];

  vector<bool> can_kill;
  vector<int> num_kills;

  for(int i = 0; i < 8; i++) {
    can_kill.push_back(can_kill_next_turn(x, y, tx + WIGGLE_SPACE * (cos( i * 45)), ty + WIGGLE_SPACE * sin(i * 45)));
  }
  can_kill.push_back(can_kill_next_turn(x, y, tx, ty));

  for(int i = 0; i < 8; i++){
    if(can_kill[i]) {
      num_kills.push_back(get_num_kills( tx + WIGGLE_SPACE * (cos(i * 45)), ty + WIGGLE_SPACE * sin(i * 45)));
    } else {
      num_kills.push_back(0);
    }
  }
  num_kills.push_back(get_num_kills(tx, ty));

  /* Get the maximum point */
  int max_score = -1;
  int selected_point = -1;
  for(int i = 0; i < 9; i++) {
    if(num_kills[i] > max_score) {
      max_score = num_kills[i];
      selected_point = i;
    }
  }

  assert(max_score != -1);
  assert(max_score != 0);
  assert(selected_point != -1);

  if(selected_point == 8) {
    best_pos.push_back(tx);
    best_pos.push_back(ty);
  } else {
    best_pos.push_back(tx + WIGGLE_SPACE * (cos(selected_point * 45)));
    best_pos.push_back(ty + WIGGLE_SPACE * (sin(selected_point * 45)));
  }

  return best_pos;
}

vector<int> get_next_move(vector<int> &ash_targets, int x, int y) {
  vector<int> ret_vector;
  /* Get to a unkilled target */
  vector<int>::iterator it = ash_targets.begin();
  while(it != ash_targets.end()) {
    if(!all_zombie[*it].alive) {
      it = ash_targets.erase(it);
    } else {
      it++;
    }
  }

  if(ash_targets.size() == 0) { ret_vector.push_back(x); ret_vector.push_back(y); return ret_vector; }

  /* Can I kill the zombie in the upcoming turn */
  int target = ash_targets[0];
  assert(all_zombie[target].alive);
  bool can_kill = can_kill_next_turn(x, y, all_zombie[target].future_pos[0], all_zombie[target].future_pos[1]);

  if(!can_kill) {
    /* Move forward towards the kill */
    assert(all_zombie[target].alive);
    int tx = all_zombie[target].future_pos[0];
    int ty = all_zombie[target].future_pos[1]; 
    double angle = get_angle(x, y, tx, ty);
    vector<float> tpos = get_translated_point(x, y, angle);
    ret_vector.push_back(tpos[0]);
    ret_vector.push_back(tpos[1]);
  } else {
    ret_vector = get_best_position(target, x, y);
  }
  return ret_vector;
}

void check_mapping(vector<vector<threat> > &h2z, vector<int> &z_map) {
  for(int i = 0; i < z_map.size(); i++) {
    int zid = i;
    int mapped_hid = z_map[i];
    bool element_found = false;
    for(int j = 0; j < h2z[mapped_hid].size(); j++) {
      if(h2z[mapped_hid][j].zombie_id == zid) {
	element_found = true;
	break;
      }
    }
    assert(element_found);
  }
}

void mark_humans_dead() {
  for(int i = 0; i < human_vec.size(); i++) {
    human_vec[i].alive = false;
  }
}

void mark_zombies_dead() {
  for(int i = 0; i < zombie_vec.size(); i++) {
    zombie_vec[i].alive = false;
  }
}

void remove_dead_humans() {
  vector<human>::iterator it = human_vec.begin();
  while(it != human_vec.end()) {
    if(!it->alive) {

      /* Mark em dead in the all globals */
      all_human[it->id].alive = false;
      all_human[it->id].id = -1;
      all_human[it->id].pos[0] = -1;
      all_human[it->id].pos[1] = -1;

      it = human_vec.erase(it);

    } else {
      it++;
    }
  }
}

void remove_dead_zombies() {
  vector<zombie>::iterator it = zombie_vec.begin();
  while(it != zombie_vec.end()) {
    if(!it->alive) {

      /* Mark em dead in all globals */
      all_zombie[it->id].alive = false;
      all_zombie[it->id].id = -1;
      all_zombie[it->id].curr_pos[0] = -1;
      all_zombie[it->id].curr_pos[1] = -1;
      all_zombie[it->id].future_pos[0] = -1;
      all_zombie[it->id].future_pos[1] = -1;

      it = zombie_vec.erase(it);

    } else {
      it++;
    }
  }
}


void mark_guys_dead() {
  mark_humans_dead();
  mark_zombies_dead();
}

void remove_dead_guys() {
  remove_dead_humans();
  remove_dead_zombies();
}

int main()
{

    int prev_humanCount = 0;
    int prev_zombieCount = 0;
    bool first_turn = true;

    vector<int> ash_targets;

    int kills = 0;

    // game loop
    while (1) {
	current_zombie_mapping.clear();
	/* We dont really know which human/zombie is dead now
	 * So mark all guys dead
	 */
	mark_guys_dead();

        int x;
        int y;
        cin >> x >> y;

        int humanCount;
        cin >> humanCount;
	if(first_turn) { 
	  initial_human_count = humanCount; 
	  all_human.resize(initial_human_count); 
	  human_to_zombie.resize(initial_human_count);
	}
        for (int i = 0; i < humanCount; i++) {
            int humanId;
            int humanX;
            int humanY;
            cin >> humanId >> humanX >> humanY;

	    human h;
	    h.pos[0] = humanX;
	    h.pos[1] = humanY;
	    h.alive = true;
	    h.id = humanId;

	    update_human_info(human_vec, h, first_turn);
        }

        int zombieCount;
        cin >> zombieCount;
	if(first_turn) { initial_zombie_count = zombieCount; all_zombie.resize(initial_zombie_count); }
        for (int i = 0; i < zombieCount; i++) {
            int zombieId;
            int zombieX;
            int zombieY;
            int zombieXNext;
            int zombieYNext;
            cin >> zombieId >> zombieX >> zombieY >> zombieXNext >> zombieYNext; cin.ignore();

	    zombie z;
	    z.alive = true;
	    z.id = zombieId;
	    z.curr_pos[0] = zombieX;
	    z.curr_pos[1] = zombieY;
	    z.future_pos[0] = zombieXNext;
	    z.future_pos[1] = zombieYNext;

	    update_zombie_info(zombie_vec, z, first_turn);
        }


	/* Remove dead zombie/human from list */
	remove_dead_guys();

	current_zombie_mapping = get_zombie_mapping(human_vec, zombie_vec);

	if(first_turn) { 
	  build_human_to_zombie_assoc(human_to_zombie, human_vec, zombie_vec);

	  /* Check if htoz mapping and current_zombie_mapping checks out */
	  check_mapping(human_to_zombie, current_zombie_mapping);
	} else {
	  if(!vectors_equal(prev_zombie_mapping, current_zombie_mapping)) {
	    vector<pair<int, int> > zombie_reassignment = get_reassignment(prev_zombie_mapping, current_zombie_mapping);
	    perform_reassignment(zombie_reassignment, human_to_zombie, human_vec, zombie_vec);
	  }

	  /* Update threats */
	  update_threats(human_to_zombie);
	}


	if(ash_targets.size() == 0) {
	  ash_targets = get_ash_targets(human_to_zombie, x, y);
	  if(ash_targets[0] == -1) {
	    output_message = "Getting to the closest civilian !! :( :( !! ";
	  } else {
	    output_message = "Trying to save all or some !! ";
	  }
	}

	vector<int> pos;
	if(ash_targets[0] == -1) {
	  float closest = FLT_MAX;
    	  int selected_index = -1;
    	  for(int i = 0; i < human_vec.size(); i++) {
    	    float distance = get_dist(x, y, human_vec[i].pos[0], human_vec[i].pos[1]);
    	    if(distance < closest) {
    	      closest = distance;
    	      selected_index = i;
    	    }
    	  }
    	  assert(selected_index != -1);

	  pos.push_back(human_vec[selected_index].pos[0]);
	  pos.push_back(human_vec[selected_index].pos[1]);

	  ash_targets.clear();

	} else {
	  pos = get_next_move(ash_targets, x, y);
	}

	if(kills % REDO == 0) {
	  ash_targets.clear();
	}


        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        cout << pos[0] <<" "<<pos[1]<< endl; // Your destination coordinates
	cerr << output_message << endl;

	prev_zombie_mapping = current_zombie_mapping;
	first_turn = false;
	kills++;
    }
}
