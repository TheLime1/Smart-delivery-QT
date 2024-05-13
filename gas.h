#ifndef GAS_H
#define GAS_H

class gas
{
public:
    gas();
    double calc_distance();
    double haversine(double lat1, double lon1, double lat2, double lon2);
    double fuel_consumption();
    float calc_cost();
    float calc_CO2();
};

#endif // GAS_H
