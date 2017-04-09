///======================================///
///Chau is late                          ///
///======================================///

#include "ai.hpp"

namespace cpp_client
{

namespace stumped
{

/////////
//To do//
/////////
//Bugs
//----
//Segfault sometimes?
//Beavers going upstream
//
//Features
//--------
//Get closest function
//Give defualt beaver a purpose
//Possibly defense
/////////

/////////////
//Functions//
/////////////
std::vector<Tile> AI::get_spawners(){
  std::vector<Tile> our_spawners;
  for(auto i: game->tiles){
    if(i->spawner != NULL && i->spawner->type == "branches"){
      our_spawners.push_back(i);
    }
  }
  return our_spawners;
}

std::vector<Tile> AI::shortest_path(const Tile& a,const std::vector<Tile>& b){
  if(b.size() == 0){
    std::cout << "YOUR CODE IS BROKEN PLS FIX" << std::endl;
  }
  if(b.size() == 1){
    return find_path(a,b.front());
  }
  int low = 2000000;
  std::vector<Tile> our_path;
  for(auto i: b){
    if(find_path(a,i).size() < low){
      low = find_path(a,i).size();
      our_path =  find_path(a,i);
    }
  }
  return our_path;
}

/// <summary>
/// This returns your AI's name to the game server.
/// Replace the string name.
/// </summary>
/// <returns>The name of your AI.</returns>
std::string AI::get_name() const
{
    // REPLACE WITH YOUR TEAM NAME!
    return "Chau-Is-Late";
}

/// <summary>
/// This is automatically called when the game first starts, once the game objects are created
/// </summary>
void AI::start()
{
    std::random_device rd;
    gen = std::mt19937(rd());
}

/// <summary>
/// This is automatically called the game (or anything in it) updates
/// </summary>
void AI::game_updated()
{
    // If a function you call triggers an update this will be called before it returns.
}

/// <summary>
/// This is automatically called when the game ends.
/// </summary>
/// <param name="won">true if you won, false otherwise</param>
/// <param name="reason">An explanation for why you either won or lost</param>
void AI::ended(bool won, const std::string& reason)
{
    // You can do any cleanup of your AI here.  The program ends when this function returns.
}

/// <summary>
/// This is called every time it is this AI.player's turn.
/// </summary>
/// <returns>Represents if you want to end your turn. True means end your turn, False means to keep your turn going and re-call this function.</returns>
bool AI::run_turn()
{
  std::cout << "Turn: " << game->current_turn << std::endl;
  //Goes through lodges, spawns fighters while less than 10 beavers
  for( auto c_lodges : player->lodges ){
    std::cout << " Selecting a lodge" << std::endl;
    Job c_job;    
    for( auto c_jobs : game->jobs ){
      if(c_jobs->title == "Fighter"){
        c_job = c_jobs;
      }
    }
    if(c_lodges->beaver == nullptr){
      if((player->beavers).size() < 10){
        std::cout << "  Creating a " << c_job->title << std::endl;
        c_job->recruit(c_lodges);
      }
    }
  } 
  
  //Gives a move to all beavers
  for( auto c_beaver : player->beavers ){
    std::cout << " Selecting a beaver of type " << c_beaver->job->title << std::endl;
    if(game->current_turn == 0 || game->current_turn == 1){
      std::cout << "  Building initial lodge" << std::endl; 
      c_beaver->build_lodge();
    }
    if(c_beaver->job->title == "Fighter"){
      if(player->opponent->lodges.size() != 0){
        std::cout << "  Going after lodge(s)" << std::endl;
        //auto target_lodge = (player->opponent->lodges).front();
        //auto c_path_lodge = find_path(c_beaver->tile, target_lodge);
        auto c_path_lodge = shortest_path(c_beaver->tile, player->opponent->lodges);
        std::cout << "  Shortest path to lodge(s)" << std::endl;
        auto target_lodge = c_path_lodge.back();
        std::cout << "  Lodge targeted" << std::endl;
        auto neighbors = c_beaver->tile->get_neighbors();
	for(const auto& neighbor : neighbors){
	  if(c_beaver->actions != 0){
	    if(neighbor->beaver && neighbor->beaver->owner == player->opponent&& neighbor->beaver->recruited){
              std::cout << "   Attacking adjacent pre-move" << std::endl;
	      c_beaver->attack(neighbor->beaver);
	    }
	  }
	  if(c_beaver->actions != 0){
	    if(neighbor == target_lodge){
              std::cout << "   Destroying a lodge pre-move" << std::endl;
	      if(c_beaver->branches == 0){
		c_beaver->pickup(neighbor,"branch",0);
              }
	      else{
		if(c_beaver->actions != 0){
		  c_beaver->drop(c_beaver->tile,"branch",0);
                }
              }
	    }
          }
        }
	if(c_path_lodge.size() > 1){
	  if(!((c_path_lodge.front()->flow_direction == "South" && c_path_lodge.front()->tile_south == c_beaver->tile)||
	  (c_path_lodge.front()->flow_direction == "West" && c_path_lodge.front()->tile_west == c_beaver->tile)||
	  (c_path_lodge.front()->flow_direction == "North" && c_path_lodge.front()->tile_north == c_beaver->tile)||
	  (c_path_lodge.front()->flow_direction == "East" && c_path_lodge.front()->tile_east == c_beaver->tile))){
            std::cout << "   Moving the beaver" << std::endl;
	    c_beaver->move(c_path_lodge.front());
          }
        }
	neighbors = c_beaver->tile->get_neighbors();
	for(const auto& neighbor : neighbors){
	  if(c_beaver->actions != 0){
	    if(neighbor->beaver && neighbor->beaver->owner == player->opponent&& neighbor->beaver->recruited){
              std::cout << "   Attacking adjacent post-move" << std::endl;
	      c_beaver->attack(neighbor->beaver);
	    }
	  }
	  if(c_beaver->actions != 0){
	    if(neighbor == target_lodge){
              std::cout << "   Destroying a lodge post-move" << std::endl;
	      if(c_beaver->branches == 0){
		c_beaver->pickup(neighbor,"branch",0);
              }
	      else{
		if(c_beaver->actions != 0){
		  c_beaver->drop(c_beaver->tile,"branch",0);
                }
              }
	    }
          }
        }
      }
      else if(player->opponent->beavers.size() != 0){
        std::vector<Tile> beavertiles;
        for(auto i: player->opponent->beavers){
          beavertiles.push_back(i->tile);
        }
        std::cout << "  Going after beavers" << std::endl;
        //auto target = (player->opponent->beavers).front()->tile;
        std::cout << "   Getting target" << std::endl;
        //auto c_path = find_path(c_beaver->tile, target);
        auto c_path = shortest_path(c_beaver->tile, beavertiles);
        std::cout << "   Acquired target" << std::endl;
        //Beaver target = c_path.back()->beaver;
        std::cout << "   Getting path" << std::endl;
        auto neighbors = c_beaver->tile->get_neighbors();
        std::cout << "   Getting neighbors" << std::endl;
        for(auto& neighbor : neighbors){
          if(c_beaver->actions != 0){
	    if(neighbor->beaver && neighbor->beaver->owner == player->opponent && neighbor->beaver->recruited){
              std::cout << "    Attacking adjacent pre-move" << std::endl;
	      c_beaver->attack(neighbor->beaver);
            }
          }
        }
	if(c_path.size() > 1){
	  if(!((c_path.front()->flow_direction == "South" && c_path.front()->tile_south == c_beaver->tile)||
	    (c_path.front()->flow_direction == "West" && c_path.front()->tile_west == c_beaver->tile)||
	    (c_path.front()->flow_direction == "North" && c_path.front()->tile_north == c_beaver->tile)||
	    (c_path.front()->flow_direction == "East" && c_path.front()->tile_east == c_beaver->tile))){
            std::cout << "    Moving the beaver" << std::endl;
	    c_beaver->move(c_path.front());
          }
        }
        neighbors = c_beaver->tile->get_neighbors();
	for(const auto& neighbor : neighbors){
	  if(neighbor->beaver && neighbor->beaver->owner == player->opponent && neighbor->beaver->recruited){
            if(c_beaver->actions != 0){
              std::cout << "    Attacking adjacent post-move" << std::endl;
              c_beaver->attack(neighbor->beaver);
            }
          }
        }
      }
    } 
    else{
     
     /* dumb code
     std::cout<<"  Simple move" <<std::endl;
     if(c_beaver&& c_beaver->moves > 2 && c_beaver->tile->tile_north != nullptr && c_beaver->tile->tile_north->is_pathable())
           c_beaver->move(c_beaver->tile->tile_north); 
     else if(c_beaver&& c_beaver->moves > 2 && c_beaver->tile->tile_west != nullptr && c_beaver->tile->tile_west->is_pathable())
           c_beaver->move(c_beaver->tile->tile_west);
     else if(c_beaver&& c_beaver->moves > 2 && c_beaver->tile->tile_south != nullptr && c_beaver->tile->tile_south->is_pathable())
           c_beaver->move(c_beaver->tile->tile_south);
     else if(c_beaver&& c_beaver->moves > 2 && c_beaver->tile->tile_east != nullptr && c_beaver->tile->tile_east->is_pathable())
           c_beaver->move(c_beaver->tile->tile_east);
    */
    }
  }
  //end of turn
  return true;
}

/// A very basic path finding algorithm (Breadth First Search) that when given a starting Tile, will return a valid path to the goal Tile.
/// <param name="start">the starting Tile</param>
/// <param name="goal">the goal Tile</param>
/// <return>A List of Tiles representing the path, the the first element being a valid adjacent Tile to the start, and the last element being the goal. Or an empty list if no path found.</return>
std::vector<Tile> AI::find_path(const Tile& start, const Tile& goal)
{
    // no need to make a path to here...
    if(start == goal || start == nullptr || goal == nullptr)
    {
        return {};
    }

    // the tiles that will have their neighbors searched for 'goal'
    std::queue<Tile> fringe;

    // How we got to each tile that went into the fringe.
    std::unordered_map<Tile,Tile> came_from;

    // Enqueue start as the first tile to have its neighbors searched.
    fringe.push(start);

    // keep exploring neighbors of neighbors... until there are no more.
    while(fringe.size() > 0)
    {
        // the tile we are currently exploring.
        Tile inspect = fringe.front();
        fringe.pop();

        // Note, we are using the `auto` keyword here
        //   The compiler can discern that this is a `std::vector<Tile>` based on the return type of `getNeighbors()`
        auto neighbors = inspect->get_neighbors();

        // cycle through the tile's neighbors.
        for(int i = 0; i < neighbors.size(); i++)
        {
            Tile neighbor = neighbors[i];

            // If we found the goal we've found the path!
            if(neighbor == goal)
            {
                // Follow the path backward starting at the goal and return it.
                std::deque<Tile> path;
                path.push_front(goal);

                // Starting at the tile we are currently at, insert them retracing our steps till we get to the starting tile
                for(Tile step = inspect; step != start; step = came_from[step])
                {
                    path.push_front(step);
                }

                // we want to return a vector as that's what we use for all containers in C++
                // (and they don't have push_front like we need)
                // So construct the vector-ized path here
                std::vector<Tile> vector_path;
                for(auto& tile : path)
                {
                    vector_path.push_back(tile);
                }
                return vector_path;
            }

            // if the tile exists, has not been explored or added to the fringe yet, and it is pathable
            if(neighbor && came_from.count(neighbor) == 0 && neighbor->is_pathable())
            {
                // add it to the tiles to be explored and add where it came from.
                fringe.push(neighbor);
                came_from[neighbor] = inspect;
            }

        } // for each neighbor

    } // while fringe not empty

    // if you're here, that means that there was not a path to get to where you want to go.
    //   in that case, we'll just return an empty path.
    return {};
}

template<class Item>
Item AI::random_element(std::vector<Item> container)
{
    if(container.size() == 0) return nullptr;
    return container.at(gen() % container.size());
}



// You can add additional methods here for your AI to call

} // stumped

} // cpp_client
