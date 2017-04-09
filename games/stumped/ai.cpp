///======================================///
///Chau is late                          ///
///======================================///

#include "ai.hpp"

namespace cpp_client
{

namespace stumped
{

//////////////////
//Josh functions//
//////////////////
//std::vector<Tile>

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
  std::cout << "My turn " << game->current_turn << std::endl;
  //Goes through lodges, spawns fighters while less than 10 beavers
  for( auto c_lodges : player->lodges ){
    Job b_jobs;    
    for( auto c_jobs : game->jobs ){
       if(c_jobs->title == "Fighter")
         b_jobs = c_jobs;
    }
    if(c_lodges->beaver == nullptr){
      std::cout<<"created fighter"<<std::endl;
      if((player->beavers).size() < 10)
        b_jobs->recruit(c_lodges);
    }
  } 
  
  //Gives a move to all beavers
  for( auto c_beaver : player->beavers ){
    if(game->current_turn == 0 || game->current_turn == 1)
      c_beaver->build_lodge();
    if(c_beaver->job->title == "Fighter"){
      std::cout <<"fighter move" <<std::endl;
      if(player->opponent->lodges.size() != 0){
        auto target_lodge = (player->opponent->lodges).front();
        auto c_path_lodge = find_path(c_beaver->tile, target_lodge);
        auto neighbors = c_beaver->tile->get_neighbors();
	for(const auto& neighbor : neighbors){
	  if(c_beaver->actions != 0){
	    std::cout<<"action available !!!" <<std::endl;
	    if(neighbor->beaver && neighbor->beaver->owner == player->opponent&& neighbor->beaver->recruited){
	      std::cout<<"should attack adjason"<<std::endl;
	      c_beaver->attack(neighbor->beaver);
	    }
	  }
	  if(c_beaver->actions != 0)
	    if(neighbor == target_lodge){
	      if(c_beaver->branches == 0)
		c_beaver->pickup(neighbor,"branch",0);
	      else
		if(c_beaver->actions != 0)
		  c_beaver->drop(c_beaver->tile,"branch",0);
	    }
        }
	if(c_path_lodge.size() > 1)
	  if(!((c_path_lodge.front()->flow_direction == "South" && c_path_lodge.front()->tile_south == c_beaver->tile)||
	  (c_path_lodge.front()->flow_direction == "West" && c_path_lodge.front()->tile_west == c_beaver->tile)||
	  (c_path_lodge.front()->flow_direction == "North" && c_path_lodge.front()->tile_north == c_beaver->tile)||
	  (c_path_lodge.front()->flow_direction == "East" && c_path_lodge.front()->tile_east == c_beaver->tile)))
	    c_beaver->move(c_path_lodge.front());
	neighbors = c_beaver->tile->get_neighbors();
	for(const auto& neighbor : neighbors){
	  if(c_beaver->actions != 0){
	    std::cout<<"action available !!!" <<std::endl;                     
	    if(neighbor->beaver && neighbor->beaver->owner == player->opponent&& neighbor->beaver->recruited){
	      std::cout<<"should attack adjason"<<std::endl;
	      c_beaver->attack(neighbor->beaver);
	    }
	  }
	  if(c_beaver->actions != 0)
	    if(neighbor == target_lodge){
	      if(c_beaver->branches == 0)
		c_beaver->pickup(neighbor,"branch",0);
	      else
		if(c_beaver->actions != 0)
		  c_beaver->drop(c_beaver->tile,"branch",0);
	    }
        }
     }
      else{
        auto target = (player->opponent->beavers).front()->tile;
        auto c_path = find_path(c_beaver->tile, target);
        auto neighbors = c_beaver->tile->get_neighbors();
        for(auto& neighbor : neighbors)
          if(c_beaver->actions != 0)
	    if(neighbor->beaver && neighbor->beaver->owner == player->opponent && neighbor->beaver->recruited)
	      c_beaver->attack(neighbor->beaver);
	  if(c_path.size() > 1)
	    if(!((c_path.front()->flow_direction == "South" && c_path.front()->tile_south == c_beaver->tile)||
	      (c_path.front()->flow_direction == "West" && c_path.front()->tile_west == c_beaver->tile)||
	      (c_path.front()->flow_direction == "North" && c_path.front()->tile_north == c_beaver->tile)||
	      (c_path.front()->flow_direction == "East" && c_path.front()->tile_east == c_beaver->tile)))
	      c_beaver->move(c_path.front());
          neighbors = c_beaver->tile->get_neighbors();
	  for(const auto& neighbor : neighbors)
	    if(neighbor->beaver && neighbor->beaver->owner == player->opponent)
              if(c_beaver->actions != 0)
                c_beaver->attack(neighbor->beaver);
      }
    } 
    else{
     std::cout<<"basic move" <<std::endl;
     if(c_beaver&& c_beaver->moves > 2 && c_beaver->tile->tile_north != nullptr && c_beaver->tile->tile_north->is_pathable())
           c_beaver->move(c_beaver->tile->tile_north); 
     else if(c_beaver&& c_beaver->moves > 2 && c_beaver->tile->tile_west != nullptr && c_beaver->tile->tile_west->is_pathable())
           c_beaver->move(c_beaver->tile->tile_west);
     else if(c_beaver&& c_beaver->moves > 2 && c_beaver->tile->tile_south != nullptr && c_beaver->tile->tile_south->is_pathable())
           c_beaver->move(c_beaver->tile->tile_south);
     else if(c_beaver&& c_beaver->moves > 2 && c_beaver->tile->tile_east != nullptr && c_beaver->tile->tile_east->is_pathable())
           c_beaver->move(c_beaver->tile->tile_east);
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
