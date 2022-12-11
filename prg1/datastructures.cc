// Datastructures.cc
//

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <stdexcept>

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

Datastructures::Datastructures(): stations()
{

    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

/**
 * @brief Datastructures::station_count
 * palauttaa asemien lukumäärän eli vectorin
 * vec_all_stations alkioiden määrän
 * @return asemien lukm
 */
unsigned int Datastructures::station_count()
{
    return vec_all_stations.size();
}

/**
 * @brief Datastructures::clear_all
 * poistaa kaikki tallennetut asemat
 */
void Datastructures::clear_all()
{
    stations.clear();
    vec_all_stations.clear();
}

/**
 * @brief Datastructures::all_stations
 * palauttaa valmiin vektorin, jossa kaikkien asemien id:t
 * @return palauttaa vectorin jossa kaikkien asemien id:t
 */
std::vector<StationID> Datastructures::all_stations()
{
    return vec_all_stations;
}

/**
 * @brief Datastructures::add_station
 * lisää annetun aseman tietorakenteisiin:
 * stations ja vec_all_stations
 * @param id lisättävän aseman id
 * @param name lisättävän aseman nimi
 * @param xy lisättävän aseman koordinaatti
 * @return jos asemaa ei ole olemassa: false
 *         muuten: true
 */
bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    if(stationExists(id)){
        return false;
    }

    shared_ptr<StationInfo> newStation = make_shared<StationInfo>(name, xy);
    stations.insert( { id, newStation } );

    vec_all_stations.push_back(id);

    return true;
}

/**
 * @brief Datastructures::get_station_name
 * tarkistaa, että asema on olemassa ja
 * palauttaa id:tä vastaavan aseman nimen
 * @param id aseman id, jonka nimi palautetaan
 * @return id:tä vastaavan aseman nimi tai
 * NO_NAME, jos asemaa ei ole olemassa
 */
Name Datastructures::get_station_name(StationID id)
{
    if(stationExists(id)){
        return stations.at(id)->stationName;
    }
    return NO_NAME;

}

/**
 * @brief Datastructures::get_station_coordinates
 * tarkistaa, että asema on olemassa ja
 * palauttaa id:tä vastaavan aseman koordinaatit
 * @param id aseman id, jonka koordinaatit palautetaan
 * @return id:tä vastaavan aseman koordinaatit,
 * NO_COORD, jos asemaa ei ole olemassa
 */
Coord Datastructures::get_station_coordinates(StationID id)
{
    if(stationExists(id)){
        return stations.at(id)->stationCoord;
    }
    return NO_COORD;
}

/**
 * @brief Datastructures::stations_alphabetically
 * järjestää vec_all_stations id:t asemien nimien
 * akkoosjärjestyksen perusteella
 * @return vectori, jossa asemien id:t asemien nimen mukaan akkosjärj.
 */
std::vector<StationID> Datastructures::stations_alphabetically()
{
    vector<StationID> vec = vec_all_stations;
    unordered_map<StationID, shared_ptr<StationInfo>> map = stations;
    sort(vec.begin(), vec.end(), [map](StationID i, StationID j)
        {return map.at(i)->stationName < map.at(j)->stationName;});

    return vec;

}

/**
 * @brief Datastructures::stations_distance_increasing
 * järjestää vec_all_stations id:t asemien koordinaattien perusteella
 * matka origosta järjestykseen, pienin matka ensin, jos matka on sama, niin
 * y-koordinaatin mukaan järjestykseessä, pienin arvo ensin
 * @return vectori, jossa asemien id:t asemien koordinaattien mukaan järj.
 */
std::vector<StationID> Datastructures::stations_distance_increasing()
{
    vector<StationID> vec = vec_all_stations;
    unordered_map<StationID, shared_ptr<StationInfo>> map = stations;
    sort(vec.begin(), vec.end(), [map](StationID i, StationID j)
        {if (sqrt(pow(map.at(j)->stationCoord.x,2) + pow(map.at(j)->stationCoord.y,2)) ==
             sqrt(pow(map.at(i)->stationCoord.x,2) + pow(map.at(i)->stationCoord.y,2))){
            return map.at(i)->stationCoord.y < map.at(j)->stationCoord.y;
        }
        return sqrt(pow(map.at(i)->stationCoord.x,2) + pow(map.at(i)->stationCoord.y,2)) <
               sqrt(pow(map.at(j)->stationCoord.x,2) + pow(map.at(j)->stationCoord.y,2));});

    return vec;
}

/**
 * @brief Datastructures::find_station_with_coord
 * etsii koordinaattia vastaavan aseman
 * @param xy koordinaatti jonka asema halutaan
 * @return aseman id, jolla on annettu koordinaatti,
 * NO_STATION, jos asemaa ei ole
 */
StationID Datastructures::find_station_with_coord(Coord xy)
{
    for(auto it = stations.begin(); it != stations.end(); ++it){
        if(it->second->stationCoord == xy){
            return it->first;
        }
    }
    return NO_STATION;
}

/**
 * @brief Datastructures::change_station_coord
 * vaihtaa aseman koordinaatin
 * @param id aseman id, jonka koordinaatti vaihdetaan
 * @param newcoord uusi aseman koordinaatti
 * @return true, jos vaihto onnistuu (asema on olemassa)
 * false, muuten
 */
bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    if(stationExists(id)){
        stations.at(id)->stationCoord = newcoord;
        return true;
    }

    return false;

}

/**
 * @brief Datastructures::add_departure
 * lisää asemalle lähtevän junan
 * @param stationid aseman id, jolta juna lähtee
 * @param trainid junan id, joka lähtee asemalta
 * @param time aika, jolloin juna lähtee
 * @return true, jos lisäys onnistuu (asema on olemassa)
 * false, muuten
 */
bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{
    try{
        stations.at(stationid)->departures.at(time).insert(trainid);
        return true;
    } catch(const out_of_range& oor1){
        try{
            stations.at(stationid)->departures[time] = {};
            stations.at(stationid)->departures.at(time).insert(trainid);
            return true;
        } catch(const out_of_range& oor2){
            return false;
        }
    }
}

/**
 * @brief Datastructures::remove_departure
 * poistaa junan lähdön asemalta
 * @param stationid asema josta junan lähtö poistetaan
 * @param trainid juna jonka lähtö poistetaan
 * @param time aika jolloin poistettava juna lähtee asemalta
 * @return true, jos poisto onnistui (asema on olemassa, juna lähtee asemalta annettuna aikana)
 * false, muuten
 */
bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    try{
        stations.at(stationid)->departures.at(time).erase(trainid);
        return true;
    }
    catch(const out_of_range& oor) {
        return false;
    }

}

/**
 * @brief Datastructures::station_departures_after
 * etsii kaikki lähdöt asemalta annetulla kellonajalla ja sen jälkeen
 * @param stationid asema, jonka lähdöt halutaan
 * @param time aika, jolloin, ja jonka jälkeen lähdöt halutaan
 * @return vectori jossa pari joka kertoo: junan lähtöajan ja junan id:n,
 * jos asemaa ei ole olemassa {{NO_TIME, NO_TRAIN}}
 */
std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{
    if(!stationExists(stationid)){
        return {{NO_TIME, NO_TRAIN}};
    }
    map<Time, set<TrainID>> dep_info = stations.at(stationid)->departures;
    vector<pair<Time, TrainID>> vec;

    for(auto it = dep_info.begin(); it != dep_info.end(); ++it){
        if(it->first >= time){
            for(auto j = it->second.begin(); j != it->second.end(); ++j){
                vec.push_back(pair{it->first, *j});
            }
        }
    }
    return vec;
}

/**
 * @brief Datastructures::add_region
 * lisää alueen tietorakenteeseen
 * regions ja vec_all_regions
 * @param id alueen id
 * @param name alueen nimi
 * @param coords alueen rajojen koordinaatit
 * @return false, jos alue on jo olemassa
 * true, lisäys onnistuu, alue ei ollut vielä olemassa
 */
bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    if(regionExists(id)){
        return false;
    }

    shared_ptr<RegionInfo> newRegion = make_shared<RegionInfo>(name, coords);
    regions.insert( { id, newRegion } );

    vec_all_regions.push_back(id);

    return true;

}

/**
 * @brief Datastructures::all_regions
 * palauttaa vec_all_regions
 * @return vectorin jossa kaikkien alueiden id:t
 */
std::vector<RegionID> Datastructures::all_regions()
{
    return vec_all_regions;
}

/**
 * @brief Datastructures::get_region_name
 * hakee id:tä vastaavan alueen nimen
 * @param id alueen id, jonka nimi halutaan
 * @return id:tä vastaavan alueen nimi,
 * NO_NAME, jos aluetta ei ole olemassa
 */
Name Datastructures::get_region_name(RegionID id)
{
    if(!regionExists(id)){
        return NO_NAME;
    }
    return regions.at(id)->regionName;
}

/**
 * @brief Datastructures::get_region_coords
 * hakee id:tä vastaavan alueen koordinaatit
 * @param id alueen id, jonka koordinaatit halutaan
 * @return vectorin, jossa alueen koordinaati
 * {NO_COORD}, jos aluetta ei ole olemassa
 */
std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    if(!regionExists(id)){
        return {NO_COORD};
    }
    return regions.at(id)->regionCoords;
}

/**
 * @brief Datastructures::add_subregion_to_region
 * lisää alueella alialueen
 * @param id alialueen id
 * @param parentid parent alueen id
 * @return false, jos annetut alueet eivät ole olemassa,
 * true, jos lisäys onnistui
 */
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    if(!(regionExists(id) and regionExists(parentid))){
        return false;
    } if(regions.at(id)->parentRegion == NO_REGION){

        RegionID parent = parentid;
        RegionID child = id;

        regions.at(id)->parentRegion = parentid;
        regions.at(parent)->subRegions.insert(id);



        while(parent != NO_REGION){
            regions.at(parent)->subRegions.insert(regions.at(child)->subRegions.begin(), regions.at(child)->subRegions.end());
            child = parent;
            parent = regions.at(parent)->parentRegion;
        }


        return true;
    }
    return false;

}

/**
 * @brief Datastructures::add_station_to_region
 * lisää aseman alueeseen
 * @param id aseman id
 * @param parentid alueen id
 * @return true, jos lisäys onnistui,
 * false, jos asemaa tai aluetta ei ole olemassa tai asemalla on jo alue
 */
bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    if(!(stationExists(id) and regionExists(parentid))){return false;}
    else if(stations.at(id)->region != NO_REGION){return false;}

    stations.at(id)->region = parentid;
    return true;
}

/**
 * @brief Datastructures::station_in_regions
 * hakee kaikki alueet joihin asema kuuluu suoraan ja epäsuorasti
 * @param id aseman id, jonka alueet halutaan
 * @return vectorin, jossa alueiden id:t
 * {NO_REGION}, jos asemaa ei ole olemassa
 */
std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    if(!stationExists(id)){return {NO_REGION};}
    RegionID regid = stations.at(id)->region;
    vector<RegionID> vec;
    while(regid != NO_REGION){
        vec.push_back(regid);
        regid = regions.at(regid)->parentRegion;
    }
    return vec;
}

//---------------------------------------------------------------------------------------------

/**
 * @brief Datastructures::all_subregions_of_region
 * hakee kaikki alueen alialueet
 * @param id alueen id, jonka alialueet halutaan
 * @return vectorin, jossa kaikkien alialueiden id:t
 */
std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID id)
{

    if(!regionExists(id)){
        return {NO_REGION};
    }
    vector<RegionID> sub_regs;
    sub_regs.insert(sub_regs.begin(), regions.at(id)->subRegions.begin(), regions.at(id)->subRegions.end());
    return sub_regs;

}

/**
 * @brief Datastructures::stations_closest_to
 * etsii koordinaattia enintään kolme lähintä asemaa
 * @param xy koordinaatti, jonka lähimmät asemat halutaan
 * @return vector, jossa koordinaattia kolme lähintä asemaa,
 * jos asemia on ainakin kolme
 */
std::vector<StationID> Datastructures::stations_closest_to(Coord xy)
{
    map<int, StationID> dist_of_stations;
    vector<StationID> stations_closest;
    int dist;
    for(auto it = vec_all_stations.begin(); it != vec_all_stations.end(); ++it){
        dist = calc_distance(xy, *it);
        dist_of_stations.insert({dist, *it});

        if(dist_of_stations.size() == 4){
            dist_of_stations.erase(--dist_of_stations.end());
        }

    }

    for(auto jt = dist_of_stations.begin(); jt != dist_of_stations.end(); ++jt){
        stations_closest.push_back(jt->second);
    }

    return stations_closest;
}

/**
 * @brief Datastructures::remove_station
 * poistaa annetun aseman
 * @param id aseman id, joka halutaan poistaa
 * @return true, jos asema on olemassa
 * false, jos annettu asemaa ei ole olemassa
 */
bool Datastructures::remove_station(StationID id)
{

    if(stationExists(id)){

        auto it = remove(vec_all_stations.begin(), vec_all_stations.end(), id);
        vec_all_stations.erase(it);

        stations.erase(id);

        return true;
    }
    return false;




}

/**
 * @brief Datastructures::common_parent_of_regions
 * etsii annettujen alueiden yhteisen vanhemman
 * @param id1 alueen 1 id
 * @param id2 alueen 2 id
 * @return ensimmäisen hieratkiassa yhteisen vanhemman
 * jos alueet eivät ole olemassa tai yhteistä vanhempaa ei ole
 * palautetaan NO_REGION
 */
RegionID Datastructures::common_parent_of_regions(RegionID id1, RegionID id2){

    if(!(regionExists(id1) and regionExists(id2))){
        return NO_REGION;
    }

    RegionID regid1 = id1;
    vector<RegionID> vec_id1;
    while(regid1 != NO_REGION){
        regid1 = regions.at(regid1)->parentRegion;
        vec_id1.push_back(regid1);
    }

    RegionID regid2 = id2;
    vector<RegionID> vec_id2;
    while(regid2 != NO_REGION){
        regid2 = regions.at(regid2)->parentRegion;
        vec_id2.push_back(regid2);
    }
    for(auto it = vec_id1.begin(); it != vec_id2.end(); ++it){
        if(find(vec_id2.begin(), vec_id2.end(), *it) != vec_id2.end()){
            return *it;
        }
    }

    return NO_REGION;

}






/**
 * @brief Datastructures::stationExists
 * tarkistaa onko annettu asema olemassa
 * @param id asema, jona olemassa olo halutaan tietää
 * @return true, jos asema on olemassa
 * false, jos ei
 */
bool Datastructures::stationExists(StationID id){
    return stations.find(id) != stations.end();
}


/**
 * @brief Datastructures::regionExists
 * tarkistaa onko annettu alue olemassa
 * @param id alue, jona olemassa olo halutaan tietää
 * @return true, jos alue on olemassa
 * false, jos ei
 */
bool Datastructures::regionExists(RegionID id){
    return regions.find(id) != regions.end();
}


/**
 * @brief Datastructures::calc_distance
 * laskee annetun koordinaatin ja aseman välisen etäisyyden
 * @param xy koordinaatti
 * @param id asema
 * @return annetun koordinaatin ja aseman etäisyys
 */
int Datastructures::calc_distance(Coord xy, StationID id){
    Coord station_xy =  stations.at(id)->stationCoord;
    return sqrt(pow(xy.x-station_xy.x,2)+pow(xy.y-station_xy.y,2));

}
