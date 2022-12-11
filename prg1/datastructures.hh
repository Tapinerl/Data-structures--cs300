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

#include <map>
#include <unordered_map>
#include <memory>
#include <set>
#include <unordered_set>
using namespace std;



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
    // Short rationale for estimate: dokumentaation perusteella
    //                               unordered_map::size kompleksisuus on vakio
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate:dokumentaation perusteella
    //                              unordered_map ja vector::clear kompleksisuus on lineaarinen
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: palauttaa valmiin vectorin
    std::vector<StationID> all_stations();

    // Estimate of performance: O(n)
    // Short rationale for estimate: tarkistaa ensin aseman olemassaolon O(N)
    //                               unordered_map::insert O(N), (keskimääräisesti vakio)
    //                               vectoriin lisäys O(1)
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: etsii aseman O(N) ja
    //                               unordered_map::at O(N)
    Name get_station_name(StationID id);


    // Estimate of performance: O(n)
    // Short rationale for estimate: etsii aseman O(N) ja
    //                               unordered_map::at O(N)
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n^(2))
    // Short rationale for estimate: sort O(NlogN), unordered_map::at O(N) ->O(N²)
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n^(2))
    // Short rationale for estimate: sort O(NlogN), unordered_map::at O(N) ->O(N²)
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: käy läpi kaikki asemat ja vertaa niiden koordinaatteja annettuihin
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: tarkastaa aseman olemassaolon O(N), jonka jälkeen unordered_map::at O(N)
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(n)
    // Short rationale for estimate: tarkistaa aseman olemassaolon, jonka jälkeen lisäys
    //                               tietorakenteeseen
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: käyttää erase -algoritmia poistaakseen trainid:n
    //                               tietorakenteesta
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: käy läpi kaikki aseman ajat, ja kaikki junat ajalla, jos aika on suurempi kuin annettu
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: sama toteutusperiaate, kuin add_departure
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(1)
    // Short rationale for estimate: palauttaa valmiin vektorin
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(n)
    // Short rationale for estimate: käy kaikki regionit läpi kunnes löytää annetun id:n
    Name get_region_name(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: sama totetutusperiaate, kuin get_region_name
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: tarkistaa ensin voiko subregionin lisätä,
    //                               lisää subregionin sekä parentregionin
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: tarkistaa ensin että lisäyksen voi tehdä,
    //                               lisää aseman regioniin ja regionin asemaan
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: tarkistaa onko asema olemassa,
    //                               käy läpi kaikki parentregionit, johin aseman regioni kuuluu
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations----------------------------------

    // Estimate of performance: O(n)
    // Short rationale for estimate: vectoriin lisäys (insert) lineaarinen
    //
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: käy läpi kaikki asemat ja laskee niiden
    //                               etäisyyden xy:stä map-tietorakenteessa pidetään vain kolmea alkiota
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: vector::remove + vector::erase + unordered_map:
    bool remove_station(StationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: etsii ensin molempien kaikki parentit
    //                               minkä jälkeen etsii yhteisen
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

private:
    // Add stuff needed for your class implementation here


    struct StationInfo
    {
        StationInfo(Name stationName, Coord stationCoord): stationName(stationName), stationCoord(stationCoord) {}
        Name stationName;
        Coord stationCoord;
        map<Time, set<TrainID>> departures;
        RegionID region = NO_REGION;
    };

    struct RegionInfo
    {
        RegionInfo(Name regionName, vector<Coord> regionCoords): regionName(regionName), regionCoords(regionCoords) {}
        Name regionName;
        vector<Coord> regionCoords;
        unordered_set<RegionID> subRegions;
        RegionID parentRegion = NO_REGION;
    };


    unordered_map<StationID, shared_ptr<StationInfo>> stations;
    vector<StationID> vec_all_stations;

    unordered_map<RegionID, shared_ptr<RegionInfo>> regions;
    vector<RegionID> vec_all_regions;

    bool stationExists(StationID id);
    bool regionExists(RegionID id);

    int calc_distance(Coord xy, StationID id);


};

#endif // DATASTRUCTURES_HH
