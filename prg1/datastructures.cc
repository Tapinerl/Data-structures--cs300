
// Datastructures.cc

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

/**
 * @brief Datastructures::Datastructures, constructor
 */
Datastructures::Datastructures()
{
    station_count_ = 0;
    all_stations_ = {};
    stations_ = {};
    departures_ = {};
    regions_ = {};
    station_in_reg_ = {};
    station_coords_ = {};
}

/**
 * @brief Datastructures::~Datastructures, destructor
 */
Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

/**
 * @brief Datastructures::station_count, returns the amount of stations
 *                                       in datastructures
 * @return amount of stations
 */
unsigned int Datastructures::station_count()
{
    return station_count_;
}

/**
 * @brief Datastructures::clear_all, clears all containers in datastructures
 */
void Datastructures::clear_all()
{
    all_stations_.clear();
    stations_.clear();
    departures_.clear();
    regions_.clear();
    station_in_reg_.clear();
    station_coords_.clear();
    station_count_ = 0;
}

/**
 * @brief Datastructures::all_stations, returns vector with all stations
 * @return vector with station id
 */
std::vector<StationID> Datastructures::all_stations()
{
    return all_stations_;
}

/**
 * @brief Datastructures::add_station, adds station with given parameters
 * @param id, unique station id
 * @param name, name of the station
 * @param xy, unique coordinates
 * @return true if station was added, false if station with same xy or id is
 *         already in datastructures
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    if (stations_.find(id) == stations_.end())
    {
        std::shared_ptr<Station_data> new_station(new Station_data{id, name, xy});
        stations_.insert({id, new_station});

        all_stations_.push_back(id);

        station_coords_.insert({xy, id});
        station_count_ += 1;
        return true;
    }
    return false;
}

/**
 * @brief Datastructures::get_station_name, returns station name with given id
 * @param id, id of the station to search
 * @return name of the station
 */
Name Datastructures::get_station_name(StationID id)
{
    return stations_.at(id)->name_;
}

/**
 * @brief Datastructures::get_station_coordinates. returns station coordinates
 *        with given id
 * @param id, id of the station
 * @return coordinates of the station, {x,y}
 */
Coord Datastructures::get_station_coordinates(StationID id)
{
    return stations_.at(id)->coord_;
}

/**
 * @brief Datastructures::stations_alphabetically, returns all stations sorted
 *        alphabetically a-z by names
 * @return vector of station ids
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::unordered_map<StationID, std::shared_ptr<Station_data>>& ref = stations_;
    std::sort(all_stations_.begin(), all_stations_.end(),[&ref]
              (StationID a, StationID b)
                {
                    return ref.at(a)->name_ < ref.at(b)->name_;
                });
    return all_stations_;
}

/**
 * @brief Datastructures::stations_distance_increasing, returns all stations sorted
 *        by distance from {0,0}, shortest to longest
 * @return vector of station ids
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::unordered_map<StationID, std::shared_ptr<Station_data>>& ref = stations_;
    std::sort(all_stations_.begin(), all_stations_.end(),[&ref]
              (StationID a, StationID b)
    {
        int a_x = ref.at(a)->coord_.x;
        int a_y = ref.at(a)->coord_.y;
        int b_x = ref.at(b)->coord_.x;
        int b_y = ref.at(b)->coord_.y;
        Distance dist_a = sqrt(pow(a_x, 2) + pow(a_y, 2));
        Distance dist_b = sqrt(pow(b_x, 2) + pow(b_y, 2));

        if (dist_a == dist_b)
        {
            return a_y < b_y;
        }
        else
        {
            return dist_a < dist_b;
        }
    });
    return all_stations_;
}

/**
 * @brief Datastructures::find_station_with_coord, returns station id
 *        with given coordinates
 * @param xy, coordinates of station
 * @return station id, if no station was found returns NO_STATION
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
    auto station = station_coords_.find(xy);
    if (station != station_coords_.end())
    {
        return station->second;
    }
    return NO_STATION;
}

/**
 * @brief Datastructures::change_station_coord, changes given stations
 *        coordinates
 * @param id, station to edit
 * @param newcoord, new coordinates for station
 * @return true if station was found with given id and coordinates changed,
 *         false otherwise
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    auto i = stations_.find(id);
    if (i != stations_.end())
    {
        Coord old = i->second->coord_;
        i->second->coord_ = newcoord;

        auto coords = station_coords_.find(old);
        station_coords_.erase(coords);
        station_coords_.insert({newcoord, id});

        return true;
    }
    return false;
}

/**
 * @brief Datastructures::add_departure, adds train departure with given parameters
 *        train and time needs to be unique together
 * @param stationid, station where the train departs
 * @param trainid, train that departs
 * @param time, time of departure
 * @return true if station was found and given trainid and time was unique,
 *         false otherwise
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    auto i_umap = departures_.find(stationid);
    std::pair temp_pair = {time, trainid};

    if (i_umap != departures_.end())
    {
        auto i_set = i_umap->second.find(temp_pair);
        if (i_set == i_umap->second.end())
        {
            i_umap->second.insert(temp_pair);
            return true;
        }
        else
        {
            return false;
        }
    }
    departures_.insert({stationid, {{time, trainid}}});
    return true;
}

/**
 * @brief Datastructures::remove_departure, removes departure
 * @param stationid, station where to remove
 * @param trainid, train to remove
 * @param time, time to remove
 * @return true if station, train and time were found
 *         false otherwise
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    auto i_umap = departures_.find(stationid);
    std::pair temp_pair = {time, trainid};

    if (i_umap == departures_.end())
    {
        return false;
    }
    auto i_set = i_umap->second.find(temp_pair);

    if (i_set == i_umap->second.end())
    {
        return false;
    }
    i_umap->second.erase(i_set);
    return true;
}

/**
 * @brief Datastructures::station_departures_after, list of all trains that depart
 *        after given time
 * @param stationid, station to search
 * @param time, time after trains depart
 * @return vector with pairs of time and trainid, if no trains depart,
 *         returns vector with {NO_TIME, NO_TRAIN}
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    std::vector<std::pair<Time, TrainID>> temp = {};
    std::pair<Time, TrainID> comp_pair = {time, "a"};

    auto i_umap = departures_.find(stationid);

    if (i_umap == departures_.end())
    {
        temp.push_back({NO_TIME, NO_TRAIN});
        return temp;
    }
    auto upper_bound = i_umap->second.upper_bound(comp_pair);

    while (upper_bound != i_umap->second.end())
    {
        temp.push_back(*upper_bound);
        ++upper_bound;
    }
    return temp;
}

/**
 * @brief Datastructures::add_region, adds region with given parameters
 * @param id, unique id of region
 * @param name, name of the region
 * @param coords, coordinates which form a polygon
 * @return true, if id is unique and region was added
 *         false, if id already exists
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    auto region = regions_.find(id);
    if (region == regions_.end())
    {
        std::vector<std::shared_ptr<Region_data>> children = {};
        Region_data* parent = nullptr;
        std::shared_ptr<Region_data>
                new_region(new Region_data{id, name, coords, parent, children});
        regions_.insert({id, new_region});
        return true;
    }
    return false;
}

/**
 * @brief Datastructures::all_regions, list of all regions
 * @return vector with region ids
 */
std::vector<RegionID> Datastructures::all_regions()
{
    auto i = regions_.begin();
    std::vector<RegionID> temp = {};

    while (i != regions_.end())
    {
        temp.push_back(i->first);
        ++i;
    }
    return temp;
}

/**
 * @brief Datastructures::get_region_name, returns name of the region
 * @param id, region to search
 * @return name of the region, if id does not exist, returns NO_NAME
 */
Name Datastructures::get_region_name(RegionID id)
{
    auto region = regions_.find(id);
    if (region == regions_.end())
    {
        return NO_NAME;
    }
    return region->second->name;
}

/**
 * @brief Datastructures::get_region_coords, returns coordinates of the region
 * @param id, region to search
 * @return vector with coordinates of the region
 *         if id does not exist, return vector with NO_COORD in it
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    auto region = regions_.find(id);
    std::vector<Coord> temp = {};

    if (region == regions_.end())
    {
        temp.push_back(NO_COORD);
        return temp;
    }
    return region->second->coords;
}

/**
 * @brief Datastructures::add_subregion_to_region, adds existing subregion
 *        to given region, region can have only one parent
 * @param id, subregion id
 * @param parentid, parent region id
 * @return true if both region ids are valid and subregion does not have a parent
 *         false otherwise
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto child = regions_.find(id);
    auto parent = regions_.find(parentid);

    if (child == regions_.end() or parent == regions_.end())
    {
        return false;
    }
    if (child->second->parent != nullptr)
    {
        return false;
    }
    Region_data* parent_ptr = parent->second.get();
    std::shared_ptr<Region_data> child_ptr = child->second;

    child->second->parent = parent_ptr;
    parent->second->children.push_back(child_ptr);
    return true;
}

/**
 * @brief Datastructures::add_station_to_region, adds station to region,
 *        station can have only one parent region
 * @param id, station id
 * @param parentid, parent region id
 * @return true if both ids are valid and station does not have parent region
 *         false otherwise
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    if (regions_.find(parentid) == regions_.end() or
            stations_.find(id) == stations_.end() or
            station_in_reg_.find(id) != station_in_reg_.end())
    {
        return false;
    }
    std::shared_ptr<Region_data> ptr = regions_.at(parentid);
    station_in_reg_.insert({id, ptr});
    return true;
}

/**
 * @brief Datastructures::station_in_regions, all the regions that the station
 *        is part of, if station's parent region is subregion of other, add
 *        that to the list and so on
 * @param id, station to search
 * @return vector of all region ids, if station id is invalid, return vector
 *         with NO_REGION
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    std::vector<RegionID> temp = {};
    if (stations_.find(id) == stations_.end())
    {
        temp.push_back(NO_REGION);
        return temp;
    }
    auto station = station_in_reg_.find(id);
    if (station == station_in_reg_.end())
    {
        return temp;
    }
    temp.push_back(station->second->id);
    return get_station_in_regions_rec(temp, station->second->parent);
}

/**
 * @brief Datastructures::all_subregions_of_region, all the regions that are
 *        subregions of given region and all the subregions of subregion and so on
 * @param id, region id to search
 * @return vector of all region ids, if given region id is invalid return
 *         vector with NO_REGION
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{
    std::vector<RegionID> temp = {};
    auto region = regions_.find(id);

    if (region == regions_.end())
    {
        temp.push_back(NO_REGION);
    }
    else
    {
        get_subregion_rec(temp, region->second);
        temp.pop_back();
    }
    return temp;
}

/**
 * @brief Datastructures::stations_closest_to, searches three closest stations
 *        to the given coordinates, if less than three are found, return those.
 *        Distance is based on the straight distance between given coordinates
 *        and station coordinates
 * @param xy, coordinates to search
 * @return vector of station ids that are found
 */
std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{

    std::set<std::pair<Distance, StationID>> temp = {};
    std::vector<StationID> return_vec = {};

    Distance longest_dist = 0;
    auto i = station_coords_.begin();

    while (i != station_coords_.end())
    {
        Distance dist = get_distance(i->first, xy);

        if (temp.size() == 3)
        {
            if (dist < longest_dist)
            {
                auto last = temp.end();
                --last;
                temp.erase(last);
                temp.insert({dist, i->second});

                auto last_new = temp.end();
                --last_new;
                longest_dist = last_new->first;
            }
        }
        else
        {
            temp.insert({dist, i->second});

            auto last_new = temp.end();
            --last_new;
            longest_dist = last_new->first;
        }
        ++i;
    }
    for (auto pair = temp.begin(); pair != temp.end(); ++pair)
    {
        return_vec.push_back(pair->second);
    }
    return return_vec;
}

/**
 * @brief Datastructures::remove_station, removes station from all containers
 *        with given id
 * @param id, id of the station to remove
 * @return true if id is valid, false otherwise
 */
bool Datastructures::remove_station(StationID id)
{
    auto station = stations_.find(id);
    if (station == stations_.end())
    {
        return false;
    }
    Coord coords = station->second->coord_;
    stations_.erase(station);

    auto xy = station_coords_.find(coords);
    station_coords_.erase(xy);

    auto station_d = departures_.find(id);
    if (station_d != departures_.end())
    {
        departures_.erase(station_d);
    }

    auto station_r = station_in_reg_.find(id);
    if (station_r != station_in_reg_.end())
    {
        station_in_reg_.erase(station_r);
    }

    auto station_a = std::find(all_stations_.begin(), all_stations_.end(), id);
    if (station_a != all_stations_.end())
    {
        all_stations_.erase(station_a);
    }

    station_count_ -=1;
    return true;
}

/**
 * @brief Datastructures::common_parent_of_regions, closest parent of given regions.
 *        Closest parent is region which subregion is not parent of both regions.
 * @param id1, first region to search
 * @param id2, second region to search
 * @return region id of the closest parent, if one or both of the given ids are
 *         invalid or no parent region is found, return NO_REGION
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2)
{
    auto reg1 = regions_.find(id1);
    auto reg2 = regions_.find(id2);

    if (reg1 == regions_.end() or reg2 == regions_.end())
    {
        return NO_REGION;
    }

    std::unordered_set<RegionID> first_parents = get_parents(reg1->second);
    if (first_parents.empty())
    {
        return NO_REGION;
    }

    return find_common_parent(first_parents, reg2->second);
}

/**
 * @brief Datastructures::get_station_in_regions_rec, recursive function that
 *        finds all parent regions of given region
 * @param vec, vector where to store region ids
 * @param next, pointer to first parent of the region
 * @return vector with all the parent regions ids
 */
std::vector<RegionID> Datastructures::get_station_in_regions_rec
                      (std::vector<RegionID> &vec, Region_data* next)
{
    if (next == nullptr)
    {
        return vec;
    }
    vec.push_back(next->id);
    return get_station_in_regions_rec(vec, next->parent);
}

/**
 * @brief Datastructures::get_subregion_rec, recursive function to search all
 *        subregions of given region
 * @param vec, vector where to store region ids
 * @param next, pointer to given region
 * @return vector with all subregions ids
 */
std::vector<RegionID> Datastructures::get_subregion_rec(std::vector<RegionID> &vec,
                                                        std::shared_ptr<Region_data> next)
{
    if (not next->children.empty())
    {
        for(auto child = next->children.begin(); child < next->children.end(); ++child)
        {
            get_subregion_rec(vec, *child);
        }
        vec.push_back(next->id);
    }
    else
    {
        vec.push_back(next->id);
    }
    return vec;
}

/**
 * @brief Datastructures::get_parents, find all parent regions of given region
 * @param first, pointer to given region
 * @return unordered_set of all parent regions ids
 */
std::unordered_set<RegionID> Datastructures::get_parents(std::shared_ptr<Region_data> first)
{
    Region_data* curr = first->parent;
    std::unordered_set<RegionID> temp = {};
    while (curr != nullptr)
    {
        temp.insert(curr->id);
        curr = curr->parent;
    }
    return temp;
}

/**
 * @brief Datastructures::find_common_parent, searches same regions from given
 *        set of regions and from given regions parents
 * @param set, set of regions to compare
 * @param first, pointer to given region which parents are compared to set one by one
 * @return if common region is found, returns that
 *         otherwise returns NO_REGION
 */
RegionID Datastructures::find_common_parent(const std::unordered_set<RegionID> &set,
                                           std::shared_ptr<Region_data> first)
{
    Region_data* curr = first->parent;
    while (curr != nullptr)
    {
        auto search = set.find(curr->id);
        if (search != set.end())
        {
            return *search;
        }
        curr = curr->parent;
    }
    return NO_REGION;
}

/**
 * @brief Datastructures::get_distance, counts distance between two coordinates
 * @param coord1, first coordinate
 * @param coord2. second coordinate
 * @return straight distance between given coordinates
 */
Distance Datastructures::get_distance(const Coord &coord1, const Coord &coord2)
{
    int x1 = coord1.x;
    int y1 = coord1.y;
    int x2 = coord2.x;
    int y2 = coord2.y;

    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}
