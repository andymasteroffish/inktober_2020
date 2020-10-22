float trail_prc_spacing = 0.2;
int trail_slot_spacing = 20;

class Actor{
  String type;
  int  dir;
  float travel_prc = 0;
  float next_prc_to_store_pnt = 0;
    Tile cur_tile;
    Tile next_tile;
    Actor target_actor;
    Tile target_tile ;
    Tile scatter_tile;
    color col;
    ArrayList<PVector> trail_pnts = new ArrayList<PVector>();
    //PVector[] trail_pnts = new PVector[1000];
    //int next_index = 0;
    float speed_mod;
    float offset_dist;
    ArrayList<Line> snapshot_lines = new ArrayList<Line>();
    Actor blinky = null;
}

Actor make_actor(String type, int c, int r, int d, color col, Actor target_actor, Tile scatter_tile){

  Actor actor = new Actor();
  
    actor.type=type;
    actor.dir=1;
    actor.cur_tile= grid[c][r][d];
    actor.next_tile= grid[c][r][d];
    actor.target_actor = target_actor;
    actor.target_tile = grid[c][r][d];
    actor.scatter_tile = scatter_tile;
    actor.col= col;
  

  //setting the movement speeds
  actor.speed_mod = 1;
  if (actor.type == "pacman"){
    actor.speed_mod = 0.8;
  }else{
    actor.speed_mod = 0.75;  //should be 0.75
  }

  //offset
  actor.offset_dist = 6;
  if (actor.type == "pacman")  actor.offset_dist *= 0;
  if (actor.type == "blinky")  actor.offset_dist *= 1;
  if (actor.type == "pinky")  actor.offset_dist *= 2;
  if (actor.type == "clyde")  actor.offset_dist *= -1;
  if (actor.type == "inky")  actor.offset_dist *= -2;

  return actor;
}

void draw_actor(Actor actor){
  fill(actor.col);

  Tile cur_tile = actor.cur_tile;
  Tile next_tile = actor.next_tile;
  //const { cur_tile } = actor;
  //const { next_tile } = actor;

  PVector cur_tile_pos = new PVector(actor.cur_tile.x, actor.cur_tile.y, actor.cur_tile.z);
  PVector next_tile_pos = new PVector(actor.next_tile.x, actor.next_tile.y, actor.next_tile.z);
  PVector pos = lerp_pnt(cur_tile_pos, next_tile_pos, actor.travel_prc, 0);

  fill(actor.col);
  stroke(actor.col);

  if (show_actors){
    push();
    noFill();
    translate(pos.x,pos.y,pos.z);
    sphere(tile_size*0.4);

    pop();

    //testing
    PVector test_pos =  get_tile_pos(actor.target_tile);
    noStroke();
    push();
    float test_offset = actor.offset_dist * 0.2;
    translate(test_pos.x+test_offset, test_pos.y+test_offset, test_pos.z+test_offset);
    box(tile_size*0.2,tile_size,tile_size*0.2);
    box(tile_size,tile_size*0.2,tile_size*0.2);
    pop();
  }

  //trail
  //console.log(actor.col)
  if (show_trails){
    draw_trail(actor);
  }
}

void draw_trail(Actor actor){
  strokeWeight(1);
  stroke(actor.col);

  if (take_snapshot){
    actor.snapshot_lines.clear();
  }

  
  int start_pnt = max(0, actor.trail_pnts.size()-trail_length);
  for (int i=start_pnt; i<actor.trail_pnts.size()-trail_slot_spacing-1; i++){
    stroke(actor.col);
    PVector a = actor.trail_pnts.get(i);;
    PVector b = actor.trail_pnts.get(i+trail_slot_spacing);

    int matching_dirs = 0;
    if (a.x == b.x)  matching_dirs++;
    if (a.y == b.y)  matching_dirs++;
    if (a.z == b.z)  matching_dirs++;

    //on curves, just connect 'em
    if (matching_dirs <= 1){
      line(a.x,a.y,a.z, b.x,b.y,b.z);
      if (take_snapshot){
        Line screen_line = new Line( screenX(a.x,a.y,a.z), screenY(a.x,a.y,a.z), screenX(b.x, b.y, b.z), screenY(b.x, b.y, b.z));
        actor.snapshot_lines.add(screen_line); 
      }

    }
    //on straight passages, do something else
    else{

       PVector new_pos = new PVector(a.x,a.y,a.z);
      //let new_pos = {
      //  x:a.x,
      //  y:a.y,
      //  z:a.z
      //}

      float angle = i*0.1;
      float dist = 10;

      if (a.x==b.x && a.y==b.y){
        new_pos.x += Math.cos(angle) * dist;
        new_pos.y += Math.sin(angle) * dist;
      }
      if (a.x==b.x && a.z==b.z){
        new_pos.z += Math.cos(angle) * dist;
        new_pos.x += Math.sin(angle) * dist;
      }
      if (a.y==b.y && a.z==b.z){
        new_pos.y += Math.cos(angle) * dist;
        new_pos.z += Math.sin(angle) * dist;
      }

      line(new_pos.x,new_pos.y,new_pos.z, b.x,b.y,b.z);
      
      if (take_snapshot){
        Line screen_line = new Line( screenX(new_pos.x,new_pos.y,new_pos.z), screenY(new_pos.x,new_pos.y,new_pos.z), screenX(b.x, b.y, b.z), screenY(b.x, b.y, b.z));
        actor.snapshot_lines.add(screen_line); 
      }

    }
  }
}

void update_actor(Actor actor, float turn_prc_step){

  //move towards the goal
  actor.travel_prc += turn_prc_step * actor.speed_mod;

  //see if we should store one or more points
  while (actor.travel_prc > actor.next_prc_to_store_pnt){

    PVector cur_tile_pos = new PVector(actor.cur_tile.x, actor.cur_tile.y, actor.cur_tile.z);
  PVector next_tile_pos = new PVector(actor.next_tile.x, actor.next_tile.y, actor.next_tile.z);
    PVector pnt = lerp_pnt(cur_tile_pos, next_tile_pos, actor.next_prc_to_store_pnt, actor.offset_dist);
    actor.trail_pnts.add(pnt);
    //actor.trail_pnts[actor.next_index] = pnt;
    //actor.next_index++;
    //actor.trail_pnts.push(pnt);

    actor.next_prc_to_store_pnt += trail_prc_spacing;
  }


  //see if we hit a turn end
  if (actor.travel_prc > 1){
    actor.travel_prc -= 1;

    //reset the next prc to store
    actor.next_prc_to_store_pnt = trail_prc_spacing;

    //end the turn
    end_actor_turn(actor);
  }

  //if travel prc is still over 1 do it again
  if (actor.travel_prc > 1){
    update_actor(actor, 0);
  }
}

void flip_direction(Actor actor){
  actor.dir = opposite_dir(actor.dir);
  //swap next and current
  Tile temp = actor.next_tile;
  actor.next_tile = actor.cur_tile;
  actor.cur_tile = temp;
}

PVector lerp_pnt(PVector pos_a, PVector pos_b, float prc, float offset){
  float x1 = pos_a.x+offset;
  float y1 = pos_a.y+offset;
  float z1 = pos_a.z+offset;

  float x2 = pos_b.x+offset;
  float y2 = pos_b.y+offset;
  float z2 = pos_b.z+offset;

  PVector pnt = new PVector( (1.0-prc) * x1 + prc * x2, (1.0-prc) * y1 + prc * y2, (1.0-prc) * z1 + prc * z2);
  
  //let pnt = {
  //  x : (1.0-prc) * x1 + prc * x2,
  //  y : (1.0-prc) * y1 + prc * y2,
  //  z : (1.0-prc) * z1 + prc * z2
  //}
  
  return pnt;
}

void end_actor_turn(Actor actor){

  //prev tile becomes next tile
  actor.cur_tile = actor.next_tile;

  //store it
  //actor.trail_tiles.push(actor.cur_tile);

  //pacman eats
  if (actor.type == "pacman"){
    actor.cur_tile.has_pellet = false;
  }

  // get our next target
  make_turn_end_decision(actor);
}

void make_turn_end_decision(Actor actor){
  


  //figure out the target
  Tile target_tile = new Tile();

  //pacman always goes for what he wants
  if (actor.type == "pacman"){
    target_tile = get_target_tile(actor);
  }
  //ghosts have different behaviors
  else{
    if (behavior_mode == "chase"){
      target_tile = get_target_tile(actor);
    }
    if (behavior_mode == "scatter"){
      target_tile = actor.scatter_tile;
    }
  }
  actor.target_tile = target_tile;
  

  //now we need to figure out where we can go
  int[] possible_dirs = new  int[NUM_DIRS];
  int dir_index = 0;
  //ArrayList<int> possible_dirs = new ArrayList<int>();
  
  for (int d=0; d<NUM_DIRS; d++){
    if (d != opposite_dir(actor.dir) ){ 
      Tile tile = get_tile_in_dir(actor.cur_tile, d);
      if (tile != null){
        if (tile.open){
          possible_dirs[dir_index] = d;
          dir_index++;
          //possible_dirs.add(d);
        }
      }
    }
  }

  //console.log("possible: ")
  

  //if (possible_dirs.size() == 0){
  if (dir_index == 0){
    //console.log("NO VALID DIRECTIONS! BAD!");
  }

  //find the direction that brings us closest to the target tile
  int best_dir = possible_dirs[0];//.get(0);
  float shortest_dist = 999999;
  for (int i=0; i<dir_index; i++){
  //for (int i=0; i<possible_dirs.size(); i++){
    int dir = possible_dirs[i];//.get(i);
  //possible_dirs.forEach( dir => {
    Tile other_tile = get_tile_in_dir(actor.cur_tile, dir);
    float distance = dist(other_tile.c, other_tile.r, other_tile.d, target_tile.c, target_tile.r, target_tile.d);
    //console.log("  "+dir+" has dist "+distance)
    if (distance < shortest_dist){
      shortest_dist = distance;
      best_dir = dir;
      //console.log("     that best")
    }
  }

  //var item = items[Math.floor(Math.random() * items.length)];
  //let new_dir = possible_dirs[Math.floor(Math.random() * possible_dirs.length)];
  actor.dir = best_dir;
  actor.next_tile = get_tile_in_dir(actor.cur_tile, actor.dir);
}

Tile get_target_tile(Actor actor){
  
  //blinky chases the player
  if (actor.type == "blinky"){
    return actor.target_actor.cur_tile;
  }

  //pinky ambushes
  if (actor.type == "pinky"){
    Tile tile_pos = new Tile();
      tile_pos.c = actor.target_actor.cur_tile.c;
      tile_pos.r = actor.target_actor.cur_tile.r;
      tile_pos.d = actor.target_actor.cur_tile.d ;
    
    PVector push_dir = dir_vec(actor.target_actor.dir);
    tile_pos.c += push_dir.x*4;
    tile_pos.r += push_dir.y*4;
    tile_pos.d += push_dir.z*4;

    return tile_pos;
  }

  //inky looks at where the player is and then factors in blinky
  if (actor.type == "inky"){
    //get the tile in front of pacman
    Tile leading_tile = new Tile();
      leading_tile.c = actor.target_actor.cur_tile.c;
      leading_tile.r = actor.target_actor.cur_tile.r;
      leading_tile.d = actor.target_actor.cur_tile.d;
      
    PVector push_dir = dir_vec(actor.target_actor.dir);
    leading_tile.c += push_dir.x*2;
    leading_tile.r += push_dir.y*2;
    leading_tile.d += push_dir.z*2;

    //console.log("leading tile "+leading_tile.c+" , "+leading_tile.r)

    //get delta from blink to that tile
    Tile delta_tile = new Tile();
    delta_tile.c= leading_tile.c - actor.blinky.cur_tile.c;
    delta_tile.r= leading_tile.r - actor.blinky.cur_tile.r;
    delta_tile.d= leading_tile.d - actor.blinky.cur_tile.d;
    

    //console.log("delta tile "+delta_tile.c+" , "+delta_tile.r)

    //add that delta to the leading pos
    Tile final_tile = new Tile();
    final_tile.c= leading_tile.c + delta_tile.c;
    final_tile.r= leading_tile.r + delta_tile.r;
    final_tile.d= leading_tile.d + delta_tile.d;

    //console.log("final tile "+final_tile.c+" , "+final_tile.r)

    return final_tile;
  }

  //clyde goes for the player until he gets close
  if (actor.type == "clyde"){
    Tile pacman_tile = actor.target_actor.cur_tile;
    if(dist(actor.cur_tile.c,actor.cur_tile.r,actor.cur_tile.d, pacman_tile.c, pacman_tile.r,pacman_tile.d) < 8){
      return actor.scatter_tile;
    }else{
      return pacman_tile;
    }
  }
  
  //pacman wants them pellets
  if (actor.type == "pacman"){
    
    //find the next closes one
    float close_dist = 9999;
    Tile best_tile = new Tile();

    game_over = true;  //assume we're done

    for (int c=0; c<num_cols; c++){
      for (int r=0; r<num_rows; r++){
        for (int d=0; d<num_depth; d++){
          if (grid[c][r][d].has_pellet){
            game_over = false;
            float this_dist = dist(c,r,d, actor.cur_tile.c, actor.cur_tile.r, actor.cur_tile.d);
            if (this_dist < close_dist){
              close_dist = this_dist;
              best_tile.c = c;
              best_tile.r = r;
              best_tile.d = d;
            }
          }
        }
      }
    }

    return best_tile;
  }
  
  return null;

}
