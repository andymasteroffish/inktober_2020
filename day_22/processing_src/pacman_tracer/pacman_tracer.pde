//https://www.gamasutra.com/view/feature/3938/the_pacman_dossier.php?print=1

ArrayList<Line> grid_lines = new ArrayList<Line>();
boolean take_snapshot = false;


float tile_size = 20;

int trail_length = 9999999;

boolean advance_time = true;
boolean player_control = false;

int num_cols = 28;
int num_rows = 31;
int num_depth = 9;

boolean[][][] raw_map;
Tile[][][] grid = new Tile[num_cols][num_rows][num_depth];

float view_offset_x = 20;
float view_offset_y = 20;
float view_zoom = 1;

boolean game_over = false;


//actors
int num_actors = 5;
Actor pacman;
Actor[] actors = new Actor[num_actors];

String behavior_mode = "chase";

//timing
float total_prc_time = 0;  
float game_time = 0;    //estimated in seconds

float next_behavior_change_time = 7;

//visual modes
boolean show_grid = true;
boolean show_trails = true;
boolean show_actors = true;
boolean show_connections = true;

PVector translate_pos = new PVector();

//let cursor_tile = null

void setup() {
  randomSeed(0);
  size(900,1200, P3D);

  //raw_map_old = make_raw_level()
  raw_map = test_level_json();

  //console.log("raw:")
  //console.log(raw_map)

  //set tiles
  //grid = new Array(num_cols);
  //for (let i=0; i<num_cols; i++){
  //  grid[i] = new Array(num_rows);
  //  for (let d=0; d<num_rows; d++){
  //    grid[i][d] = new Array(num_depth)
  //  }
  //}

  //tranfer it
  for (int c=0; c<num_cols; c++){
    for (int r=0; r<num_rows; r++){
      for (int d=0; d<num_depth; d++){
        grid[c][r][d] = make_tile(c,r,d);
      }
    }
  }

  //set actors
  pacman = make_actor("pacman",14,11,4,color(219, 213, 26), null, null);
  actors[num_actors-1] = pacman;

  
  //ghosts
  Tile b_scatter_tile = new Tile();
  b_scatter_tile.c = num_cols;
  b_scatter_tile.r = -1;
  b_scatter_tile.d = -1;
  actors[0] = make_actor("blinky",num_cols-2,1,0,color(255, 20, 20),pacman,b_scatter_tile);

  Tile p_scatter_tile = new Tile();
  p_scatter_tile.c = -1;
  p_scatter_tile.r = -1;
  p_scatter_tile.d = num_depth;
  actors[1] = make_actor("pinky",1,1,8,color(242, 126, 205),pacman,p_scatter_tile);

  Tile i_scatter_tile = new Tile();
  i_scatter_tile.c = num_cols;
  i_scatter_tile.r = num_rows;
  i_scatter_tile.d = num_depth;
  Actor inky = make_actor("inky",num_cols-2,num_rows-2,8,color(47, 245, 245),pacman,i_scatter_tile);
  inky.blinky = actors[0];  //this ghost is the only one that cares about another ghost
  actors[2] = inky;

  Tile c_scatter_tile = new Tile();
  c_scatter_tile.c = -1;
  c_scatter_tile.r = num_rows;
  c_scatter_tile.d = -1;
  actors[3] = make_actor("clyde",1,num_rows-2,0, color(242, 174, 63),pacman,c_scatter_tile);
  
  //actors.push(pacman)

  //cursor_tile = grid[0][0][0]
}

void set_behavior(String new_setting){
  behavior_mode = new_setting;

  //flip all ghost directions
  for (int i=0; i<num_actors; i++){
    flip_direction(actors[i]); 
  }
  //actors.forEach(actor => {
  //  if (actor.type != "pacman"){
  //    flip_direction(actor)
  //  }
  //})

  //console.log("behavior is now: "+behavior_mode)
}

void draw() {
  //behavior_mode = "chase";
  
  if (take_snapshot){
    println("snap it!");
    grid_lines.clear();
  }
  
  float test_x = 0;
  float test_y = 0;
  
  background(250);

  //line(-400,0,400,0)
    

  if (!game_over){
    //advance time
    if (advance_time){
      //figure out how much time to pass
      float turn_step = 0.1;
      if (keyPressed && key == 'f'){
        turn_step = 1.5;
      }

      //update eveyrbody
      for (int i=0; i<num_actors; i++){
        update_actor(actors[i], turn_step);
      }
      //actors.forEach( actor => {
      //  update_actor(actor, turn_step)
      //})

      //estimate the time in seconds
      total_prc_time += turn_step;
      game_time = total_prc_time / (10/pacman.speed_mod);  //it takes pacman about a second to go 10 tiles
    
      //console.log("time to mode swith "+(next_behavior_change_time-game_time) )
      //time to swicth behaviors?
      if (game_time > next_behavior_change_time){
        if (behavior_mode == "chase"){
          set_behavior("scatter");
          next_behavior_change_time += game_time<50 ?  5 : 4;
        }else{
          set_behavior("chase");
          next_behavior_change_time += 20;
        }
      }

    }

  }

  

  //draw this thing

  push();
  translate(width/4+translate_pos.x, height/4+translate_pos.y);
  float rot_limit =  PI/2;
  rotateY( map(mouseX,0,width,-rot_limit, rot_limit));
  rotateX( map(mouseY,0,height,-rot_limit, rot_limit));
  scale(view_zoom, view_zoom, view_zoom);
  //translate(-num_cols*tile_size*0.5, -num_rows*tile_size*0.5);
  
  

  //draw the map
  if (show_grid){
    for (int c=0; c<num_cols; c++){
      for (int r=0; r<num_rows; r++){
        for (int d=0; d<num_depth; d++){

          Tile tile = grid[c][r][d];
          if (tile.open){
            float size = tile_size*0.1;
            if (tile.has_pellet){
              fill(0);
              size= tile_size*0.20;
            }else{
              fill(134, 41, 140);
            }
            if (show_connections)  size *= 0.4;
            noStroke();
            push();
            translate(tile.x, tile.y, tile.z);
            
            if(c==num_cols-2 && r==num_rows-2 && d == 0){
              //println("now pls");
              //test_x = screenX(0,0,0);
              //test_y = screenY(0,0,0);
            }
            
            sphere(size);
            pop();

            if (show_connections){
              for (int dir = 0; dir<NUM_DIRS; dir++){
                if (dir == DIR_UP || dir == DIR_RIGHT || dir == DIR_IN){
                  Tile next = get_tile_in_dir(tile,dir);
                  if (next != null){
                    if (next.open){
                      stroke(0);
                      line(tile.x,tile.y,tile.z, next.x, next.y, next.z);
                      if (take_snapshot){
                        Line screen_line = new Line( screenX(tile.x,tile.y,tile.z), screenY(tile.x,tile.y,tile.z), screenX(next.x, next.y, next.z), screenY(next.x, next.y, next.z));
                        grid_lines.add(screen_line);  
                      }
                    }
                  }
                }
              }
            }
          }
          
        }

      }
    }
  }

  //draw the actors
  for (int i=0; i<num_actors; i++){
    draw_actor(actors[i]);
  }



  pop();

  //fill(0)
  //text("FPS: "+frameRate(), 10,15)
  
  //fill(255,0,0);
  //line(test_x,0, test_x,height);
  //line(0,test_y,0, width,test_y,0);
  //circle(test_x, test_y, 20);
  //println(test_x+" " +test_y);
  
  //testing
  stroke(0);
  for (int i=0; i<grid_lines.size(); i++){
    Line this_line = grid_lines.get(i);
    line(this_line.x1, this_line.y1, this_line.x2, this_line.y2);
  }
  
  for (int a=0; a<num_actors; a++){
    stroke(actors[a].col);
    for (int i=0; i<actors[a].snapshot_lines.size(); i++){
      Line this_line = actors[a].snapshot_lines.get(i);
      line(this_line.x1, this_line.y1, this_line.x2, this_line.y2);
    }
  }
  
  //save if we're done with snapshot
  if (take_snapshot){
    println("saving");
    take_snapshot = false;
    String[] text = new String[grid_lines.size()];
    for (int i=0; i<grid_lines.size(); i++){
      Line this_line = grid_lines.get(i);
      text[i] =  str(this_line.x1)+","+str(this_line.y1)+","+str(this_line.x2)+","+str(this_line.y2);
      //println(text[i]);
    }
    saveStrings("output/grid_lines.txt", text);
    
    for (int a=0; a<num_actors; a++){
      text = new String[actors[a].snapshot_lines.size()];
      for (int i=0; i<actors[a].snapshot_lines.size(); i++){
        Line this_line = actors[a].snapshot_lines.get(i);
        text[i] =  str(this_line.x1)+","+str(this_line.y1)+","+str(this_line.x2)+","+str(this_line.y2);
      }
      saveStrings("output/actor"+a+".txt", text);
    }
  }
}


void keyPressed(){
  //if (key == 'Enter'){
  //  new_turn();
  //}
  
  if (key == ' '){
    take_snapshot = true;
  }

  if (key=='0'){
    advance_time = !advance_time;
  }

  //view toggles
  if (key == '1'){
    show_trails = !show_trails;
  }
  if (key == '2'){
    show_grid = !show_grid;
  }
  if (key == '3'){
    show_actors = !show_actors;
  }
  if (key == '4'){
    show_connections = !show_connections;
  }


  if (key == 'm'){
    if (behavior_mode == "scatter"){
      set_behavior("chase");
    }else{
      set_behavior("scatter");
    }
  }
  
   float trans_dist = 10;
   if (keyCode == UP){
     translate_pos.y -= trans_dist;
   }
   if (keyCode == DOWN){
     translate_pos.y += trans_dist;
   }
   if (keyCode == LEFT){
     translate_pos.x -= trans_dist;
   }
   if (keyCode == RIGHT){
     translate_pos.x += trans_dist;
   }
   
   if (key == ','){
     view_zoom -= 0.05;
   }
   if (key == '.'){
     view_zoom += 0.05;
   }
}

/*
function mouseWheel(event) {
  print(event.delta);
  //move the square according to the vertical scroll amount
  view_zoom += event.delta * 0.01;
  
  if (view_zoom < 0.1)  view_zoom = 0.1
    console.log(view_zoom)
  //uncomment to block page scrolling
  //return false;
}
*/
