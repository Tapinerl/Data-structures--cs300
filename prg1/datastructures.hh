// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <map>

// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Returns saved variable
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Removes n amount of elements from containers
    void clear_all();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Returns only vector
    std::vector<StationID> all_stations();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Vector insert has amortized constant complexity
    //                               and umap insert has O(n) worst case and O(1) average

    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Umap "at" method has O(n) worst case and average O(1)
    Name get_station_name(StationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Umap "at" method has O(n) worst case and average O(1)
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n * log(n))
    // Short rationale for estimate: std::sort has this complexity
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n * log(n))
    // Short rationale for estimate: std::sort has this complexity
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Umap "find" method has O(n) worst case and average O(1)
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Umap "find" method has O(n) worst case and average O(1)
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Set "insert" and "find" have O(log(n))
    //                              complexity and Umap "find" method has average O(1),
    //                              but O(n) worst case
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Set "insert" and "find" have O(log(n))
    //                              complexity and Umap "find" method has average O(1),
    //                              but O(n) worst case
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Umap "at" method has O(n) worst case but average O(1)
    //                               Still goes trough all elements in vector
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: Umap "insert" method has O(n) worst case and average O(1)
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Adds all elements in vector
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Umap "find" method has O(n) worst case and average O(1)
    Name get_region_name(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Umap "find" method has O(n) worst case and average O(1)
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Umap "find" method has O(n) worst case and average O(1)
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Used umap methods have O(n) worst case and average O(1)
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Recursive function is called for every region
    //                               in worst case
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: Worst case goes trough every region
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Goes trough all elements in station_coords_
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find for vector goes trough all elements in worst
    //                               case. Same for umap find.
    bool remove_station(StationID id);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: Only if set.find goes O(n) in worst case.
    //                               Otherwise O(n) in average
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

private:
    // Add stuff needed for your class implementation here
    unsigned int station_count_;

    struct Station_data
    {
        StationID id_;
        Name name_;
        Coord coord_;
    };

    struct Region_data
    {
        RegionID id;
        Name name;
        std::vector<Coord> coords;
        Region_data* parent;
        std::vector<std::shared_ptr<Region_data>> children;
    };

    std::unordered_map<StationID, std::shared_ptr<Station_data>> stations_;
    std::vector<StationID> all_stations_;
    std::unordered_map<Coord, StationID, CoordHash> station_coords_;

    std::unordered_map<StationID, std::set<std::pair<Time, TrainID>>> departures_;

    std::unordered_map<RegionID, std::shared_ptr<Region_data>> regions_;
    std::unordered_map<StationID, std::shared_ptr<Region_data>> station_in_reg_;

    std::vector<RegionID> get_station_in_regions_rec(std::vector<RegionID> &vec,
                                                     Region_data* next);
    std::vector<RegionID> get_subregion_rec(std::vector<RegionID> &vec,
                                            std::shared_ptr<Region_data> next);

    std::unordered_set<RegionID> get_parents(std::shared_ptr<Region_data> first);
    RegionID find_common_parent(const std::unordered_set<RegionID> &set,
                               std::shared_ptr<Region_data> first);

    Distance get_distance(const Coord &coord1, const Coord &coord2);
};

#endif // DATASTRUCTURES_HH
