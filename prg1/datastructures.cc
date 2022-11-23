// Datastructures.cc
//
// Student name: Elias Nikkinen
// Student email: elias.nikkinen@tuni.fi
// Student number:50497168

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::station_count()
{
    // Replace the line below with your implementation
    return stations_.size();

}

void Datastructures::clear_all()
{
    // Replace the line below with your implementation
    stations_.clear();
    regions_.clear();
}

std::vector<StationID> Datastructures::all_stations()
{
    // Replace the line below with your implementation
    std::vector<StationID> stationIds;
    for (auto& x : stations_)
    {
        stationIds.push_back(x.first);
    }
    return stationIds;
}

bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if (stations_.find(id) == stations_.end())
    {
        Station add_station= {id, name, xy, {}};
        stations_[id] = std::make_shared<Station>(add_station);
        return true;
    }
    else
    {
        return false;
    }
}

Name Datastructures::get_station_name(StationID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if (stations_.find(id) != stations_.end())
    {
        return{stations_[id]->name};
    }
    else
    {
        return NO_NAME;
    }
}

Coord Datastructures::get_station_coordinates(StationID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if (stations_.find(id) != stations_.end())
    {
        return stations_[id]->coord;
    }
    else
    {
       return NO_COORD;
    }

}

std::vector<StationID> Datastructures::stations_alphabetically()
{
    // Replace the line below with your implementation
    std::vector<std::pair<StationID, std::shared_ptr<Station>>> places_to_sort;
    for (auto& x : stations_)
    {
        places_to_sort.push_back(x);
    }
    std::sort(places_to_sort.begin(), places_to_sort.end(),
              [](const std::pair<StationID, std::shared_ptr<Station>>& a,
              const std::pair<StationID, std::shared_ptr<Station>>& b)
               { return a.second->name < b.second->name; });
    std::vector<StationID> sorted;
    for (auto& i : places_to_sort)
    {
        sorted.push_back(i.first);
    }
    return sorted;
}

std::vector<StationID> Datastructures::stations_distance_increasing()
{
    // Replace the line below with your implementation
    std::vector<StationID> vector_ids = all_stations();
     std::sort(vector_ids.begin(), vector_ids.end(),
                [this](StationID a, StationID b) {
         return SquareEuclidean(get_station_coordinates(a))
                 < SquareEuclidean(get_station_coordinates(b)); });
     return vector_ids;
}

int Datastructures::SquareEuclidean(Coord c1, Coord c2)
{
    int dx = (c1.x - c2.x);
    int dy = (c1.y - c2.y);
    int sqrDistance = dx * dx + dy * dy;
    return sqrDistance;
}

StationID Datastructures::find_station_with_coord(Coord xy)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    for(auto& x : stations_)
    {
        if (x.second->coord == xy)
        {
            return x.first;
        }
    }
    return NO_STATION;
}

bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(stations_.find(id) != stations_.end())
    {
        stations_[id]->coord = newcoord;
        return true;
    }
    else
    {
        return false;
    }
}

bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    for(auto& x : stations_)
    {
        if(x.first == stationid && x.second->trains.find(trainid) == x.second->trains.end())
        {
            x.second->trains[trainid] = time;
            return true;
        }
    }
    return false;
}

bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(stations_.find(stationid)!= stations_.end())
    {
        auto it = stations_[stationid]->trains.find(trainid);
        if(it != stations_[stationid]->trains.end())
        {
            stations_[stationid]->trains.erase(it);
            return true;
        }
    }
    return false;
}

std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    std::vector<std::pair<Time, TrainID>> takeoffs;
    std::pair<Time, TrainID> pairs;
    if(stations_.find(stationid) != stations_.end())
    {
            for(auto x : stations_[stationid]->trains)
            {
                if(x.second > time || x.second == time)
                {
                    pairs = std::make_pair(x.second, x.first);
                    takeoffs.push_back(pairs);
                }
            }
            std::sort(std::begin(takeoffs),
                      std::end(takeoffs),checkTakeoffs);
            return takeoffs;
    }
    else
    {
        return {{NO_TIME,NO_TRAIN}};
    }
}

bool Datastructures::checkTakeoffs(std::pair<Time, TrainID> a, std::pair<Time, TrainID> b)
{
    int outcome = a.second.compare(b.second);
    if(a.first < b.first)
    {
        return true;
    }
    else
    {
        if(outcome < 0 || outcome == 0)
        {
            return true;
        }
        return false;
    }
}

bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if (regions_.find(id) == regions_.end())
    {
        Region add_region = { id, name, coords, nullptr, {} , {}};
        regions_[id] = std::make_shared<Region>(add_region);
        return true;
    }
    else
    {
       return false;
    }
}

std::vector<RegionID> Datastructures::all_regions()
{
    // Replace the line below with your implementation
    std::vector<RegionID> region_ids;
    for(auto& x : regions_)
    {
        region_ids.push_back(x.first);
    }
    return region_ids;
}

Name Datastructures::get_region_name(RegionID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if (regions_.find(id) != regions_.end())
    {
        return{regions_[id]->name};
    }
    else
    {
        return NO_NAME;
    }
}

std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if (regions_.find(id) != regions_.end())
    {
        return regions_[id]->coords;
    }
    else
    {
       return {NO_COORD};
    }
}

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if (regions_.find(id) != regions_.end() && regions_.find(parentid) != regions_.end()
            && regions_[id]->parent == nullptr)
    {
        regions_[id]->parent = regions_[parentid];
        regions_[parentid]->subregions.push_back(id);
        return true;
    }
    else
    {
        return false;
    }
}

bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    if(stations_.find(id) != stations_.end() && regions_.find(parentid) != regions_.end()
            && stations_[id]->region==0)
    {
        stations_[id]->region = parentid;
        regions_[parentid]->stations.push_back(id);
        return true;
    }
    else
    {
         return false;
    }
}

std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    std::vector<RegionID> parentRegs;
    if(stations_.find(id) == stations_.end())
    {
        return {NO_REGION};
    }
    else
    {
       parentRegs.push_back(stations_[id]->region);
       return go_through_regions(parentRegs,stations_[id]->region);
    }
    return parentRegs;
}

std::vector<RegionID> Datastructures::go_through_regions(std::vector<RegionID> regs_vect, RegionID id)
{
    if (regions_[id]->parent == nullptr)
    {
        return regs_vect;
    }
    regs_vect.push_back(regions_[id]->parent->id);
    return go_through_regions(regs_vect, regions_[id]->parent->id);
}

// täst alaspäi ei tartte
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("all_subregions_of_region()");
}

std::vector<StationID> Datastructures::stations_closest_to(Coord /*xy*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("stations_closest_to()");
}

bool Datastructures::remove_station(StationID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_station()");
}

RegionID Datastructures::common_parent_of_regions(RegionID /*id1*/, RegionID /*id2*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("common_parent_of_regions()");
}

